#include "../../includes/events/EventsManager.hpp"

namespace Webserv
{
	EventsManager::EventsManager(void) {}

	EventsManager::~EventsManager(void) {}

	void	EventsManager::add_event(int fd, IEvents &event_object)
	{
		this->events_map.insert(std::pair<int, IEvents*>(fd, &event_object));
	}

	void	EventsManager::remove_event(int fd)
	{
		std::map<int, IEvents*>::iterator it = this->events_map.find(fd);
		std::map<int, IEvents*>::iterator ite = this->events_map.end();

		if (it != ite)
			this->events_map.erase(it);
	}

	IEvents	*EventsManager::get_event(int fd)
	{
		return events_map[fd];
	}

	std::map<int, IEvents *>::iterator EventsManager::begin()
	{
		return this->events_map.begin();
	}

	std::map<int, IEvents *>::iterator EventsManager::end()
	{
		return this->events_map.end();
	}
}	//namespace Webserv