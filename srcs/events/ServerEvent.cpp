#include "../../includes/events/ServerEvent.hpp"

namespace Webserv
{

	ServerEvent::ServerEvent(const socket_type &new_sock, config_type& _config): sock(new_sock), config(_config)
	{
		this->events_flags= POLLIN;
	}

	ServerEvent::~ServerEvent(void)
	{
		if (this->sock.getFd())
			this->sock.close();
	}

	void	ServerEvent::read_event(void)
	{
		socket_type	client_sock = this->sock.accept();
		std::cout << "Server read event: " << this->sock.getAddress().getStrAddress() << ":" << this->sock.getAddress().getIntPort() <<std::endl;

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
		return this->sock.getFd();
	}
}