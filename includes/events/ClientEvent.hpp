#ifndef CLIENTEVENT_HPP
# define CLIENTEVENT_HPP

# include "../utils/Socket.hpp"
# include "../utils/Resource.hpp"
# include "../http/HttpResponse.hpp"
# include"../http/Server.hpp"
# include "../http/HttpRequest.hpp"
# include "../utils/common.hpp"
# include "../methods/Methods.hpp"
# include "../core/Config.hpp"
# include "./CGIEvent.hpp"
# include "../http/Server.hpp"
# include "IEvents.hpp"
# include <string>
# include <poll.h>

namespace Webserv
{
	class ClientEvent : public IEvents {
		public:
			typedef Webserv::Utils::Socket				socket_type;
			typedef Webserv::Config						config_type;
			typedef Webserv::Http::HttpRequestBuilder	http_request_builder_type;
			typedef Webserv::Http::HttpRequest			http_request_type;
			typedef Webserv::Utils::Resource			resource_type;
			typedef CGIEvent							cgi_event_type;
			typedef Webserv::Http::Server				http_server_type;
			typedef Webserv::Http::Route				http_route_type;

		private:
			http_request_builder_type	create_req;
			http_request_type			req;
			resource_type				*rcs;
			socket_type					sock;
			socket_type					&srv_sock;
			config_type					&config;
			cgi_event_type				*cgi;
			http_server_type			srv;
			short						events_flags;
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
