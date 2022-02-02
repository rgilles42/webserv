/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientEvent.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/09 15:33:08 by yun               #+#    #+#             */
/*   Updated: 2022/02/02 20:43:11 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTEVENT_HPP
# define CLIENTEVENT_HPP

# include <vector>
# include <string>
# include <poll.h>

# include "CGIEvent.hpp"
# include "IEvents.hpp"
# include "../core/Config.hpp"
# include "../utils/common.hpp"
# include "../utils/Socket.hpp"
# include "../utils/Resource.hpp"
# include "../http/Server.hpp"
# include "../http/HttpResponse.hpp"
# include "../http/HttpRequest.hpp"
# include "../methods/Methods.hpp"

namespace Webserv
{
	class ClientEvent : public IEvents {
		public:
			typedef Webserv::Utils::Socket				socket_type;
			typedef Webserv::Config						config_type;
			typedef Webserv::Http::HttpRequestBuilder	http_request_builder_type;
			typedef Webserv::Http::HttpRequest			http_request_type;
			typedef http_request_builder_type::request_list		http_request_list;
			typedef Webserv::Utils::Resource			resource_type;
			typedef CGIEvent							cgi_event_type;
			typedef Webserv::Http::Server				http_server_type;
			typedef Webserv::Http::Route				http_route_type;
			typedef Webserv::Http::HttpResponse			http_response_type;
			typedef std::vector<http_response_type>		response_vector;

		private:
			http_request_builder_type	create_req;
			socket_type					sock;
			socket_type					&srv_sock;
			config_type					&config;
			short						events_flags;
			response_vector				responses;


			resource_type				*rcs;
			cgi_event_type				*cgi;

			http_request_type			req;
			http_server_type			srv;
			http_route_type				route;

			std::string					request_string;
			resource_type				currentResource;

			ClientEvent(void);

		public:

			ClientEvent(socket_type &client_sock, socket_type &server_sock , config_type& _config);
			virtual ~ClientEvent();

			void	write_event(void);
			void	read_event(void);

			short	getEventsFlags(void);
			int		getFD(void);

			Webserv::Config::server_type::route_type	getRoute(const std::string& url, const Webserv::Config::server_type::routes_map& routes, const Webserv::Config::server_type::route_type& defaultRoute);
			std::vector<std::string>	split(const std::string& str, char delim);

			struct	clientEventReadFailed : public std::exception
			{
				virtual const char* what() const throw()
				{
					return ("Client: read failed");
				}
			};
	};
}

#endif
