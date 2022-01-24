#ifndef CLIENTEVENT_HPP
# define CLIENTEVENT_HPP

# include "../Socket.hpp"
# include "../Resource.hpp"
# include "../http/HttpResponse.hpp"
# include"../http/Server.hpp"
# include "../http/HttpRequest.hpp"
# include "../utils/common.hpp"
//#include "../methods/Methods.hpp"
# include "../Config.hpp"
# include "./CGIEvent.hpp"
# include "IEvents.hpp"
# include <string>
# include <poll.h>

namespace Webserv
{
	class ClientEvent : public IEvents
	{

		private:
			Webserv::Config						&config;
			Webserv::Http::HttpRequestBuilder	create_req;
			Webserv::Http::HttpRequest			req;
			Resource							*rcs;
			Socket								sock;
			Socket								srv_sock;
			CGIEvent							*cgi;
			short								events_flags;

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
