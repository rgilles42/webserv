#include "../../includes/events/CGIEvent.hpp"

namespace Webserv
{

	CGIEvent::CGIEvent(void) : writeEnd(false), CGIEnd(false), status(0)
	{
		this->fd_in[0] = -1;
		this->fd_in[1] = -1;
		this->fd_out[0] = -1;
		this->fd_out[1] = -1;
	}

	CGIEvent::~CGIEvent(void)
	{
		this->close_pipefd();
	}

	void    CGIEvent::write_event(void)
	{
		Poll	write_poll;
		std::vector<struct pollfd>::iterator it;
		ssize_t	ret = 0;

		write_poll.add_fd(this->fd_in[1], POLLOUT);
		if (this-> req.getMethod().toString() == "POST" && this->req.getBody().size() != 0)
		{
			while (1)
			{
				write_poll.exec();
				it = write_poll.begin();
				if ((it->revents & POLLOUT) == POLLOUT)
				{
					ret = write(this->fd_in[1], this->req.getBody().c_str(), this->req.getBody().length());
					(void)ret;
					break ;
				}
			}
		}
		this->writeEnd = true;
	}

	void	CGIEvent::init(const http_request_type& request, const http_server_type& server, const env_type& environnement, const http_route_type& r)
	{

		this->req = request;
		this->route = r;
		this->srv = server;
		this->env = environnement;
		if (pipe(this->fd_in) < 0)
			throw CGIPipeFailed();
		if (pipe(this->fd_out) < 0)
			throw CGIPipeFailed();

		if (fcntl(fd_in[0], F_SETFL, O_NONBLOCK) < 0)
			throw CGINonBlockingFailed();
		if (fcntl(fd_out[0], F_SETFL, O_NONBLOCK) < 0)
			throw CGINonBlockingFailed();
		this->writeEnd = false;
		this->CGIEnd = false;
		this->status = 0;
	}

	void    CGIEvent::init_env()
	{
		/*--------*/
		/* SERVER */
		/*--------*/

		this->env.set("SERVER_SOFTWARE", "Webserv/HTTP/1.1");
		this->env.set("SERVER_NAME", this->srv.getServerName());
		this->env.set("GATEWAY_INTERFACE", "CGI/1.1");
		this->env.set("SERVER_PORT", SSTR(this->srv.getListen().getIntPort()));

		/*---------*/
		/* Request */
		/*---------*/

		this->env.set("SERVER_PROTOCOL", "HTTP/1.1");
		this->env.set("REQUEST_METHOD",this->req.getMethod().toString());
		this->env.set("QUERY_STRING", this->req.getQuery());

		this->env.set("SCRIPT_NAME", 		route.getFilePath(this->req.getBasePath()));
		this->env.set("SCRIPT_FILENAME",	route.getFilePath(this->req.getBasePath()));
		this->env.set("PATH_INFO", route.getFilePath(this->req.getBasePath()));
		this->env.set("REQUEST_URI", route.getFilePath(this->req.getBasePath()));
		this->env.set("REDIRECT_STATUS", "");

		this->env.set("REMOTE_HOST", this->req.getHostname());	// Nom hote client
		this->env.set("REMOTE_ADDR", this->req.getIp());		// IP Client
		this->env.set("AUTH_SCRIPT", "");
		this->env.set("REMOTE_USER", "");
		this->env.set("CONTENT_TYPE", this->req.getHeader("Content-Type"));
		this->env.set("CONTENT_LENGTH", SSTR(this->req.getBody().length()));

		/*--------*/
		/* Client */
		/*--------*/

		this->env.set("HTTP_ACCEPT", this->req.getHeader("Accept"));
		this->env.set("HTTP_ACCEPT_LANGUAGE", this->req.getHeader("Accept-Language"));
		this->env.set("HTTP_USER_AGENT", this->req.getHeader("User-Agent"));
		this->env.set("HTTP_COOKIE", "");
		this->env.set("HTTP_REFERER", "");
	}

	int	CGIEvent::exec()
	{
		int ret;

		this->init_env();

		this->write_event();

		pid_t	pid = fork();
		if (pid < 0)
			throw CGIForkFailed();
		else if (pid == 0)
		{
			// Init pipe for child
			close(this->fd_in[1]);
			close(this->fd_out[0]);

			/* Redirect stdin */
			if (dup2(this->fd_in[0], STDIN_FILENO) == -1) {
				close(this->fd_in[0]);
				close(this->fd_out[1]);
				exit(EXIT_FAILURE);
			}

				/* Redirect stdout */
			if (dup2(this->fd_out[1], STDOUT_FILENO) == -1) {
				close(this->fd_in[0]);
				close(this->fd_out[1]);
				exit(EXIT_FAILURE);
			}
			close(this->fd_in[0]);
			close(this->fd_out[1]);

				// Init args
			std::vector<std::string>	argsVec;

			argsVec.push_back(this->route.getCgiPass());
			argsVec.push_back(this->route.getFilePath(this->req.getBasePath()));

			char** args = new char*[argsVec.size() + 1];
			if (!args) {
				exit(EXIT_FAILURE);
			}
			size_t i = 0;
			std::vector<std::string>::const_iterator it = argsVec.begin();
			while (it != argsVec.end()) {
				args[i] = new char[it->length() + 1];
				if (!args[i]) {
						for (size_t j = 0; j < i; j++)
						{
							if (args[j]) {
								delete args[j];
							}
						}
						delete[] args;
						exit(EXIT_FAILURE);
					}
					std::strcpy(args[i], it->c_str());
					it++;
					i++;
				}
				args[i] = NULL;

				// Init envp
				char**	envp = this->env.toEnvp();
				if (!envp) {
					for (size_t j = 0; j < i; j++)
					{
						if (args[j]) {
							delete args[j];
						}
					}
					delete[] args;
					exit(EXIT_FAILURE);
				}

				/* Execve CGI */
				ret = execve(args[0], args, envp);
				if (args) {
					for (size_t j = 0; j < i; j++)
					{
						if (args[j]) {
							delete args[j];
						}
					}
					delete[] args;
				}
				this->env.freeEnvp(envp);
				exit(ret);
			}
			else
			{
				// Init pipe for parent
				close(this->fd_in[0]);
				close(this->fd_out[1]);

				waitpid(pid, &ret, 0);

				close(this->fd_in[1]);

				this->fd_in[0] = -1;
				this->fd_in[1] = -1;

				if (WIFEXITED(ret))
				{
					this->status = WEXITSTATUS(ret);
					std::cout << "BUT WHAT IS THE STATUS CODE, YOU MAY ASK? WELL, IT IS " << this->status << std::endl;
				}
				this->CGIEnd = true;
			}
		return (this->status);
	}

	void    CGIEvent::close_pipefd(void)
	{
		if (this->fd_in[0] > 0) {
			close(this->fd_in[0]);
			this->fd_in[0] = -1;
		}
		if (this->fd_in[1] > 0) {
			close(this->fd_in[1]);
			this->fd_in[1] = -1;
		}
		if (this->fd_out[0] > 0) {
			close(this->fd_out[0]);
			this->fd_out[0] = -1;
		}
		if (this->fd_out[1] > 0) {
			close(this->fd_out[1]);
			this->fd_out[1] = -1;
		}
	}

	int		CGIEvent::getReadFD()
	{
		return this->fd_out[0];
	}

	bool	CGIEvent::writeIsEnd()
	{
		return this->writeEnd;
	}

	bool	CGIEvent::CGIIsEnd()
	{
		return this->CGIEnd;
	}

}   //namespace Webserv
