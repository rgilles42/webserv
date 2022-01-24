#ifndef SERVEREVENT_HPP
# define SERVEREVENT_HPP

# include <poll.h>
# include <iostream>

# include "./IEvents.hpp"
# include "../Socket.hpp"
# include "./ClientEvent.hpp"
# include "../core/Core.hpp"
# include "../Config.hpp"

namespace Webserv
{
	class ServerEvent : public IEvents
	{
		private:
			Socket			sock;
			Webserv::Config	&config;
			short			events_flags;

		public:
			ServerEvent(const Socket &new_socket, Webserv::Config &_config);
			~ServerEvent();

			void	write_event(void);
			void	read_event(void);

			short	getEventsFlags(void);
			int		getFD(void);
	};
}
#endif