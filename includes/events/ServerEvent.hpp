#ifndef SERVEREVENT_HPP
# define SERVEREVENT_HPP

# include <poll.h>
# include <iostream>

# include "./IEvents.hpp"
# include "../utils/Socket.hpp"
# include "./ClientEvent.hpp"
# include "../core/Core.hpp"
# include "../core/Config.hpp"

namespace Webserv
{
	class ServerEvent : public IEvents {
		public:
			typedef Webserv::Utils::Socket	socket_type;
			typedef Webserv::Config			config_type;

		private:
			socket_type				sock;
			config_type&	config;
			short				events_flags;

		public:
			ServerEvent(const socket_type &new_socket, config_type &_config);
			~ServerEvent();

			void	write_event(void);
			void	read_event(void);

			short	getEventsFlags(void);
			int		getFD(void);
	};
}
#endif