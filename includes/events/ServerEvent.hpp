#ifndef SERVEREVENT_HPP
# define SERVEREVENT_HPP

# include <poll.h>
# include <iostream>

# include "./IEvents.hpp"
# include "../Socket.hpp"
# include "./ClientEvent.hpp"
# include "../core/Core.hpp"

namespace Webserv
{
	class ServerEvent : public IEvents
	{
		private:
			Socket			sock;	//just for test
//			Http::Server	&srv;	TO DO - Need classe with dedicated sock
			short			events_flags;

		public:
			ServerEvent(const Socket &new_sock);	//TO DO replace after test
//			ServerEvent(Http::Server &ref);
			~ServerEvent();

			void	write_event(void);
			void	read_event(void);

			short	getEventsFlags(void);
			int		getFD(void);
	};
}
#endif