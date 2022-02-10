#include "../../includes/events/ServerEvent.hpp"

namespace Webserv
{

	ServerEvent::ServerEvent(const socket_type &new_sock, config_type& _config, env_type& environnement, logger_type& log): sock(new_sock), config(_config), env(environnement), logger(log)
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
		this->logger << std::make_pair(this->logger.DEBUG, "New Client detected: ") << this->sock.getAddress().getStrAddress() << ":" << this->sock.getAddress().getIntPort() << std::endl;
		ClientEvent *new_clientEvent = new ClientEvent(client_sock, this->sock, this->config, this->env, this->logger);

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