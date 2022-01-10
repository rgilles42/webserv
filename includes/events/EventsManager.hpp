#ifndef EVENTSMANAGER_HPP
# define EVENTSMANAGER_HPP

#include "IEvents.hpp"
#include <map>

namespace Webserv
{
	/* Manager events - create new events object - call functions and more */
	class EventsManager
	{
		private:
			std::map<int, IEvents *> events_map;

		public:
			EventsManager();
			~EventsManager();

			void	add_event(int fd, IEvents &event_object);
			void	remove_event(int fd);

			IEvents	*get_event(int fd);

			std::map<int, IEvents *>::iterator	begin();
			std::map<int, IEvents *>::iterator	end();
	};
}

#endif