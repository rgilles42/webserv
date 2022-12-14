/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientEvent.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgilles <rgilles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/09 15:33:08 by yun               #+#    #+#             */
/*   Updated: 2022/02/11 16:19:38 by rgilles          ###   ########.fr       */
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
# include "../core/Logger.hpp"
# include "../utils/common.hpp"
# include "../utils/Socket.hpp"
# include "../utils/Resource.hpp"
# include "../http/Server.hpp"
# include "../http/HttpResponse.hpp"
# include "../http/HttpRequest.hpp"
# include "../methods/Methods.hpp"
# include "../http/Route.hpp"

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 4096
# endif

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
			typedef Webserv::Utils::Env					env_type;
			typedef Webserv::Logger						logger_type;
			typedef std::vector<size_t>					written_response_sizes;

		private:
			http_request_builder_type	create_req;
			socket_type					sock;
			socket_type					&srv_sock;
			config_type					&config;
			short						events_flags;
			response_vector				responses;
			written_response_sizes		written_sizes;

			env_type					env;
			logger_type					logger;


			ClientEvent(void);
			void	setToError(http_response_type& response, http_route_type& route, resource_type& rcs, http_response_type::status_code_type code);

		public:

			ClientEvent(socket_type &client_sock, socket_type &server_sock , config_type& _config, Utils::Env& environnement, logger_type& log);
			virtual ~ClientEvent();

			void	write_event(void);
			void	read_event(void);

			short	getEventsFlags(void);
			int		getFD(void);

			Webserv::Config::server_type::route_type	getRoute(const std::string& url, const Webserv::Config::server_type::routes_map& routes, const Webserv::Config::server_type::route_type& defaultRoute);
			std::vector<std::string>	split(const std::string& str, char delim);

			struct	ClientEventReadFailed : public std::exception
			{
				virtual const char* what() const throw()
				{
					return ("Client: read failed");
				}
			};
			struct	ClientClosedConnectionEvent : public std::exception
			{
				virtual const char* what() const throw()
				{
					return ("Client: connection closed");
				}
			};
	};
}

#endif
