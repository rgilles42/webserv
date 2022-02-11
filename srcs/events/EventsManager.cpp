#include "../../includes/events/EventsManager.hpp"

namespace Webserv
{
	EventsManager::EventsManager(void) {}

	EventsManager::~EventsManager(void) {
		for (event_map_type::iterator it = this->events_map.begin(); it != this->events_map.end(); ++it)
		{
			delete it->second;
		}
	}

	void	EventsManager::add_event(int fd, IEvents &event_object) {
		this->events_map.insert(std::pair<int, IEvents*>(fd, &event_object));
	}

	void	EventsManager::remove_event(int fd) {
		event_map_type::iterator it = this->events_map.find(fd);

		if (it != this->events_map.end()) {
			delete it->second;
			this->events_map.erase(it);
		}
	}

	IEvents	*EventsManager::get_event(int fd) {
		return this->events_map[fd];
	}

	const EventsManager::event_map_type&	EventsManager::getEvents(void) const {
		return this->events_map;
	}

}	//namespace Webserv