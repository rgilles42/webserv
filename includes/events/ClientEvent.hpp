#ifndef CLIENTEVENT_HPP
# define CLIENTEVENT_HPP

#include "../Socket.hpp"
#include "../Resource.hpp"
#include "../http/HttpResponse.hpp"
#include"../http/Server.hpp"
#include "../http/HttpRequest.hpp"
#include "../utils/common.hpp"
#include "IEvents.hpp"
#include <string>
#include <poll.h>

namespace Webserv
{
	class ClientEvent : public IEvents
	{

		private:
//			Webserv::Http::Server		m_srv;
			Webserv::Http::HttpRequestBuilder	create_req;
			Webserv::Http::HttpRequest			req;
			Resource							*rcs;
			Socket						sock;
			short						events_flags;
//			Methods::MethodsManager	*m_methods;

			std::string		request_string;

			ClientEvent(void);

		public:

			ClientEvent(Socket &new_sock/*, Webserv::Http::Server &srv*/);
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
