#ifndef CLIENTEVENT_HPP
# define CLIENTEVENT_HPP

# include "../Socket.hpp"
# include "../Resource.hpp"
# include "../http/HttpResponse.hpp"
# include"../http/Server.hpp"
# include "../http/HttpRequest.hpp"
# include "../utils/common.hpp"
# include "../methods/Methods.hpp"
# include "../Config.hpp"
# include "./CGIEvent.hpp"
# include "../http/Server.hpp"
# include "IEvents.hpp"
# include <string>
# include <poll.h>

namespace Webserv
{
	class ClientEvent : public IEvents
	{

		private:
			Webserv::Http::HttpRequestBuilder	create_req;
			Webserv::Http::HttpRequest			req;
			Resource							*rcs;
			Socket								sock;
			Socket								&srv_sock;
			Webserv::Config						&config;
			CGIEvent							*cgi;
//			std::vector<Webserv::Http::Server>	srv;
			Webserv::Http::Server				srv;
			short								events_flags;
			Webserv::Http::Route				route;

			std::string				request_string;
			Webserv::Resource		currentResource;

			ClientEvent(void);

		public:

			ClientEvent(Socket &client_sock, Socket &server_sock , Webserv::Config	&_config);
			virtual ~ClientEvent();

			void	write_event(void);
			void	read_event(void);

			short	getEventsFlags(void);
			int		getFD(void);

			Webserv::Config::server_type				getServer(const Socket& sock, const Webserv::Http::HttpRequest& request, Webserv::Config::server_vector& servers);
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
