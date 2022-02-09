/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIEvent.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/09 15:33:08 by yun               #+#    #+#             */
/*   Updated: 2022/02/09 15:04:07 by ppaglier         ###   ########.fr       */
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

	class CGIEvent {
		public:
			typedef Webserv::Http::HttpRequest	http_request_type;
			typedef Webserv::Http::Route		http_route_type;
			typedef Webserv::Http::Server		http_server_type;
			typedef Webserv::Utils::Env			env_type;

		private:
			int					fd_in[2];   //use to write request body
			int					fd_out[2];  //use to redirect cgi output
			http_request_type	req;
			http_route_type		route;
			http_server_type	srv;
			env_type			env;
			bool				writeEnd;
			bool				CGIEnd;
			int					status;

			void	close_pipefd(void);
			void	init_env();
			void	init_args();

		public:
			CGIEvent(const http_request_type& request, const http_server_type& server, const env_type&	environnement, const http_route_type& route);
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
