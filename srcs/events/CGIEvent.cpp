#include "../../includes/events/CGIEvent.hpp"

namespace Webserv
{

	CGIEvent::CGIEvent(const Webserv::Http::HttpRequest &request, const Http::Server &server, Webserv::Utils::Env& environnement) : req(request), srv(server), env(environnement), writeEnd(false), args(NULL), CGIEnd(false)
	{
		if (pipe(this->fd_in) < 0)
			throw CGIPipeFailed();
		if (pipe(this->fd_out) < 0)
			throw CGIPipeFailed();

		if (fcntl(fd_in[0], F_SETFL, O_NONBLOCK) < 0)
			throw CGINonBlockingFailed();
		if (fcntl(fd_out[0], F_SETFL, O_NONBLOCK) < 0)
			throw CGINonBlockingFailed();
		this->wr_size = 0;
		this->status = 0;
	}

	CGIEvent::~CGIEvent()
	{
		this->close_pipefd();
		if (this->args)
		{
			if (this->args[1])
				delete this->args[1];
			if (this->args[0])
				delete this->args[0];
			delete[] this->args;
		}
	}

	void    CGIEvent::write_event()
	{
		Poll	write_poll;
		std::vector<struct pollfd>::iterator it;
		ssize_t	ret = 0;

		write_poll.add_fd(fd_in[0], POLLOUT);
		if (this-> req.getMethod().toString() == "POST" && this->req.getBody().size() != 0)
		{
			while (1)
			{
				write_poll.exec();
				it = write_poll.begin();
				if (it->revents & POLLOUT)
				{
					ret = write(fd_in[0], &this->req.getBody().c_str()[this->wr_size - 1], this->req.getBody().size()) - this->wr_size;
					if (ret < 0)
					{
						std::cout<<"Error cgi write"<<std::endl;
						exit(-1);
					}
					this->wr_size += ret;
					if (ret == 0 || this->wr_size == this->req.getBody().length())
						break;
				}
			}
		}
		this->writeEnd = true;
	}

	void	CGIEvent::init_args()
	{
		try {
			this->args = new char*[3];

			std::string		path_cgi = "/usr/local/bin/php-cgi";	//need change
			std::string		file_path = "../default_pages/index.php";	//need changes
			if (open(file_path.c_str(), EACCES) < 0 && errno == EACCES)
				throw CGIOpenFailed();
			args[0] = new char[path_cgi.size() + 1];
 			args[0] = std::strcpy(args[0], path_cgi.c_str());	//cgi-path
			args[1] = new char[file_path.size() + 1];
			args[1] = std::strcpy(args[1] ,file_path.c_str());	// file path
			args[2] = 0;
		}
		catch (const std::exception &e) {
			std::cout<<e.what()<<std::endl;
		}

	}

	void    CGIEvent::init_env()
	{
		/*--------*/
		/* SERVER */
		/*--------*/

		this->env.set("SERVER_SOFTWARE", "Webserv/HTTP/1.1");
//        this->env.set("SERVER_NAME", this->server.name());
		this->env.set("GATEWAY_INTERFACE", "CGI/1.1");

		/*---------*/
		/* Request */
		/*---------*/

		this->env.set("SERVER_PROTOCOL", "HTTP/1.1");
//        this->env.set("SERVER_PORT", srv.port());
        this->env.set("REQUEST_METHODS",this->req.getMethod().toString());
//        this->env.set("PATH_INFO", this->request.path_info());
		this->env.set("PATH_TRANSLATED", ""); //? Need more infos
//		this->env.set("SCRIPT_NAME", this->req.getQuery().c_str());
//        this->env.set("QUERY_STRING", this->request.getQuery().c_str());
//        this->env.set("REMOTE_HOTE", this->request.hote_client());
//        this->env.set("REMOTE_ADDR", this->request.client_add());
		this->env.set("AUTH_SCRIPT", ""); //? Need more infos
//		this->env.set("REMOTE_USER", this->request.client_user()); //if script protect and srv allow identification
//        this->env.set("CONTENT_TYPE", this->request.content_type());
//		this->env.set("CONTENT_LENGHT", this->request.content_lenght());

		/*--------*/
		/* Client */
		/*--------*/

//        this->env.set("HTTP_ACCEPT", this->request.http_accept());
//        this->env.set("HTTP_ACCEPT_LANGUAGE", this->request.http_accept_language());
//        this->env.set("HTTP_USER_AGENT", this->request.navigateur());
//        this->env.set("HTTP_COOKIE", this->request.cookie());   //BONUS
		this->env.set("HTTP_COOKIE", "");
		this->env.set("HTTP_REFERER", ""); //? Need more infos
	}


	int	CGIEvent::exec()
	{
		int ret;
		char	**envp;

		this->init_env();
		this->init_args();
		envp = this->env.toEnvp();

		this->pid = fork();
		if (this->pid < 0)
		{
			std::cout<<"Error fork"<<std::endl;	//need change
			return(1);
		}
		else if (this->pid == 0)
		{
			/* Redirect stdin */
			close(this->fd_in[1]);
			if (dup2(this->fd_in[0], 0) < 0)
			{
				close(this->fd_in[0]);
				close(this->fd_out[0]);
				close(this->fd_out[0]);
				this->env.freeEnvp(envp);
//				throw dupCGIFailed();
			}
			close(this->fd_in[0]);
			/* Redirect stdout */
			close(this->fd_out[0]);
			if (dup2(this->fd_out[1], 1) < 0)
			{
				close(this->fd_in[0]);
				close(this->fd_out[1]);
				this->env.freeEnvp(envp);
			}
			close(fd_out[1]);
			/* Execve CGI */
			ret = execve(this->args[0], this->args, envp);
			if(ret < 0)
				exit(ret);
			exit(0);
		}
		else
		{
			close(fd_in[0]);
			fd_in[0] = -1;
			close(fd_out[1]);
			fd_out[0] = -1;
			this->env.freeEnvp(envp);
			waitpid(this->pid, &ret, 0);
			if (WIFEXITED(ret))
			{
				this->status = WEXITSTATUS(ret);
			}
			this->CGIEnd = true;
 		}
		 return (this->status);
	}

	void    CGIEvent::close_pipefd(void)
	{
		if (this->fd_in[0] > 0)
			close(this->fd_in[0]);
		if (this->fd_in[1] > 0)
			close(this->fd_in[1]);
		if (this->fd_out[0] > 0)
			close(this->fd_out[0]);
		if (this->fd_out[1] > 0)
			close(fd_out[1]);
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
