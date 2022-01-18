#include "../../includes/events/ServerEvent.hpp"

namespace Webserv
{
/*	ServerEvent::ServerEvent(Http::Server &ref): srv(ref)
	{
		this->events_flags = POLLIN;
	}*/

	ServerEvent::ServerEvent(Socket &new_sock): sock(new_sock)
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

//		Http::Client *new_client = new Http::Client(&client_sock);
//		ClientEvent *new_clientEvent = new Http::ClientEvent(&new_client);
		ClientEvent *new_clientEvent = new ClientEvent(client_sock/*, this->srv*/);

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