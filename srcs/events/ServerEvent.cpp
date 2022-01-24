#include "../../includes/events/ServerEvent.hpp"

namespace Webserv
{

	ServerEvent::ServerEvent(const Socket &new_sock, Webserv::Config &_config): sock(new_sock), config(_config)
	{
		this->events_flags= POLLIN;
	}

	ServerEvent::~ServerEvent(void)
	{
		if (this->sock.fd())
			this->sock.close();
	}

	void	ServerEvent::read_event(void)
	{
		std::cout<<"Server read event"<<std::endl;
		Socket	client_sock = this->sock.accept();

		ClientEvent *new_clientEvent = new ClientEvent(client_sock, this->sock, this->config);

		Webserv::Core::getInstance().add_client_event(new_clientEvent->getFD(), *new_clientEvent);
	}

	void	ServerEvent::write_event(void) {}

	short	ServerEvent::getEventsFlags(void)
	{
		return this->events_flags;
	}

	int		ServerEvent::getFD(void)
	{
		return this->sock.fd();
	}
}