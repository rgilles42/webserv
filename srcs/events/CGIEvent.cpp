#include "../../includes/events/CGIEvent.hpp"

namespace Webserv
{

	CGIEvent::CGIEvent(void) : tmp_in(NULL), tmp_out(NULL), writeEnd(false), CGIEnd(false), status(0)
	{
		this->fd_in = -1;
		this->fd_out = -1;
	}

	CGIEvent::~CGIEvent(void)
	{
		this->close_pipefd();
	}

	void	CGIEvent::write_event(void)
	{
		poll_type	write_poll;
		poll_type::poll_fd_vector::const_iterator poll_it;
		ssize_t	ret = 0;

		write_poll.add_fd(this->fd_in, POLLOUT);
		if (this-> req.getMethod().toString() == "POST" && this->req.getBody().size() != 0)
		{
			http_request_type::body_type body = this->req.getBody();
			std::cerr << body.length() << std::endl;
			while (1)
			{
				write_poll.exec();
				poll_it = write_poll.getPollUsedFD().begin();
				if (poll_it != write_poll.getPollUsedFD().end()) {
					if ((poll_it->revents & POLLOUT) == POLLOUT)
					{
						ret = write(poll_it->fd, body.c_str(), body.length());
						body = body.substr(ret);
					}
				}
				if (body.empty()) {
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

		this->tmp_in = tmpfile();
		if (!this->tmp_in) {
			throw CGIException("tmp_in error");
		}
		this->tmp_out = tmpfile();
		if (!this->tmp_in) {
			throw CGIException("tmp_out error");
		}

		this->fd_in = fileno(this->tmp_in);
		this->fd_out = fileno(this->tmp_out);

		if (fcntl(this->fd_in, F_SETFL, O_NONBLOCK) < 0)
			throw CGINonBlockingFailed();
		if (fcntl(this->fd_out, F_SETFL, O_NONBLOCK) < 0)
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
		int	ret;

		this->init_env();

		this->write_event();
		lseek(this->fd_in, 0, SEEK_SET);

		pid_t	pid = fork();
		if (pid < 0)
			throw CGIForkFailed();
		else if (pid == 0)
		{
			/* Redirect stdin */
			if (dup2(this->fd_in, STDIN_FILENO) == -1) {
				exit(EXIT_FAILURE);
			}

			/* Redirect stdout */
			if (dup2(this->fd_out, STDOUT_FILENO) == -1) {
				exit(EXIT_FAILURE);
			}

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
				waitpid(pid, &ret, 0);
				lseek(this->fd_out, 0, SEEK_SET);

				if (this->fd_in > 0) {
					close(this->fd_in);
					this->fd_in = -1;
				}

				if (WIFEXITED(ret))
					this->status = WEXITSTATUS(ret);
				this->CGIEnd = true;
			}
		return (this->status);
	}

	void	CGIEvent::close_pipefd(void)
	{
		if (this->fd_in > 0) {
			close(this->fd_in);
			this->fd_in = -1;
		}
		if (this->fd_out > 0) {
			close(this->fd_out);
			this->fd_out = -1;
		}

		if (this->tmp_in) {
			fclose(this->tmp_in);
			this->tmp_in = NULL;
		}
		if (this->tmp_out) {
			fclose(this->tmp_out);
			this->tmp_out = NULL;
		}
	}

	int		CGIEvent::getReadFD()
	{
		return this->fd_out;
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
