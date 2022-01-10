#ifndef CLIENTEVENT_HPP
# define CLIENTEVENT_HPP

#include "../Socket.hpp"
#include "../Resource.hpp"
#include "../http/HttpResponse.hpp"
#include "../utils/common.hpp"
#include "IEvents.hpp"
#include <string>
#include <poll.h>

namespace Webserv
{
	class ClientEvent : public IEvents
	{

		private:
			Socket					sock;
			short					events_flags;
//			Methods::MethodsManager	*m_methods;

			std::string				request_string;
			Webserv::Resource		currentResource;

			ClientEvent(void);

		public:

			ClientEvent(Socket &new_sock);
//			ClientEvent(Http::Client &ref);
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
