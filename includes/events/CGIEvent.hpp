#ifndef CGIEVENT_HPP
# define CGIEVENT_HPP

#include "../http/HttpRequest.hpp"
#include "../http/Server.hpp"
#include "../utils/Env.hpp"
#include <poll.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

namespace Webserv
{

	class CGIEvent
	{
		private:
			int					fd_in[2];   //use to write request body
			Http::HttpRequest	req;
			Http::Server		srv;
			int					fd_out[2];  //use to redirect cgi output
			Webserv::Utils::Env	env;
			pid_t				pid;
			bool				writeEnd;
			char				**args;
			bool				CGIEnd;
			int					status;

			int					wr_size;

			void	close_pipefd(void);
			void	init_env();
			void	init_args();

		public:
			CGIEvent(Webserv::Http::HttpRequest &request);
			~CGIEvent();

			int		exec(void);

			void	write_event(void);

			int		getReadFD(void);

			bool	writeIsEnd();
			bool	CGIIsEnd();

			struct CGIPipeFailed : public std::exception
			{
				virtual const char* what() const throw()
				{
					return("CGI: pipe failed");
				}
			};
			struct CGINonBlockingFailed: public std::exception
			{
				virtual const char* what() const throw()
				{
					return("CGI: Fcntl failed");
				}
			};
			struct CGIOpenFailed : public std::exception
			{
				virtual const char* what() const throw()
				{
					return("CGI: Can't acces to file");
				}
			};
			struct CGIDupFailed : public std::exception
			{
				virtual const char* what() const throw()
				{
					return("CGI: Dup failed");
				}
			};
	};

}

#endif
