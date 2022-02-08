/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIEvent.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/09 15:33:08 by yun               #+#    #+#             */
/*   Updated: 2022/02/02 20:48:11 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIEVENT_HPP
# define CGIEVENT_HPP

//# include <poll.h>
# include <iostream>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <errno.h>

# include "../utils/Env.hpp"
# include "../http/HttpRequest.hpp"
# include "../http/Server.hpp"
# include "./Poll.hpp"
# include "../http/Route.hpp"
# include "../utils/common.hpp"

namespace Webserv
{

	class CGIEvent
	{
		private:
			int					fd_in[2];   //use to write request body
			int					fd_out[2];  //use to redirect cgi output
			Http::HttpRequest	req;
			Http::Server		srv;
			Webserv::Utils::Env	env;
			Webserv::Http::Route route;
			bool				writeEnd;
			char				**args;
			bool				CGIEnd;
			int					status;

			void	close_pipefd(void);
			void	init_env();
			void	init_args();

		public:
			CGIEvent(const Webserv::Http::HttpRequest &request, const Webserv::Http::Server &server, const Webserv::Utils::Env&	environnement, const Webserv::Http::Route& route);
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
