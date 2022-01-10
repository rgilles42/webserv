/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 14:05:38 by ppaglier          #+#    #+#             */
/*   Updated: 2022/01/10 14:52:50 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/core/Core.hpp"

namespace Webserv {

	Core::Core(void) : Singleton<Core>() {
		this->isInit = false;
		this->logger.setPrefix("\x1b[33m[Webserv]\x1b[0m");
	}

	Core::~Core(void) {}

	const Core::logger_type&	Core::getLogger(void) const {
		return this->logger;
	}

	void		Core::setCustomConfigFile(const std::string& configFile) {
		this->customConfigFile = configFile;
	}

	void		Core::setCustomMimeTypesFile(const std::string& mimeTypesFile) {
		this->customMimeTypesFile = mimeTypesFile;
	}

	bool		Core::preInit(int argc, char *argv[], char *envp[]) {
		this->args.fromArg(argc, argv);

		args_type::map_type::const_iterator it = this->args.getArgs().begin();
		while (it != this->args.getArgs().end()) {
			if (it->first == "-c") {
				if (it->second.empty()) {
					this->logger << std::make_pair(this->logger.ERROR, args_type::BadFormatException(it->first).what()) << std::endl;
					exit(EXIT_FAILURE);
				}
				this->setCustomConfigFile(it->second);
			} else if (it->first == "-?" || it->first == "-h") {
				std::cout << this->getHelp()  << std::endl;
				exit(EXIT_SUCCESS);
			} else {
				this->logger << std::make_pair(this->logger.ERROR, args_type::UnknownArgException(it->first).what())  << std::endl;
				exit(EXIT_FAILURE);
			}
			it++;
		}
		this->env.fromEnvp(envp);
		return true;
	}

	bool		Core::init(void) {
		std::string configFile = DEFAULT_CONFIG_LOCATION;
		std::string mimeTypesFile = DEFAULT_MIME_TYPES_LOCATION;
		if (!this->customConfigFile.empty()) {
			configFile = this->customConfigFile;
		}
		if (!this->customMimeTypesFile.empty()) {
			mimeTypesFile = this->customMimeTypesFile;
		}
		this->config.reset();
		this->config.addConfigFile(configFile);
		this->config.addConfigFile(mimeTypesFile);
		if (!this->config.processFiles()) {
			return false;
		}
		this->isInit = true;
		return true;
	}

	const bool&	Core::isReady(void) const {
		return this->isInit;
	}

	std::string		Core::getHelp(void) const {
		std::ostringstream ss;

		ss << "webserv version: webserv/1.0.0" << std::endl;
		ss << "Usage: webserv [-?h] [-c filename] [-c=filename]" << std::endl << std::endl;
		ss << "Options:" << std::endl;
		ss << " -?,-h		: this help" << std::endl;
		ss << " -c filename	: set configuration file (default: ./conf/webserv.conf)" << std::endl;
		ss << " -c=filename	: set configuration file (default: ./conf/webserv.conf)" << std::endl;

		return ss.str();
	}

	void		Core::add_server_event(Socket &sock)	// Add Server Event in EventsManager
	{
		ServerEvent	*srv = new ServerEvent(sock);

		this->events_manager->add_event(srv->getFD(), *srv);
	}

	void		Core::add_client_event(int fd, ClientEvent &client_e)	// Add Client Event in EventsManager
	{
		this->events_manager->add_event(fd, client_e);
	}

/*	void		Core::add_cgi_event(Ressource &rcs, int fd_in[2], std::string methods)	// Add CGI Event in EventsManager
	{
		CGI	*cgi = new CGI(rcs, fd_in, methods);
		this->events_manager->add_event(cgi->getWriteFD() ,cgi);
	}*/

	void		Core::remove_event(int fd)	// Remove Event in EventsManager
	{
		this->events_manager->remove_event(fd);
	}

	void		Core::setup_events()	// Add all events fd in Poll
	{
		this->poll_events->clear();		// Clear old events fd
		std::map<int, IEvents *>::iterator	ite = this->events_manager->end();

		for (std::map<int, IEvents *>::iterator	it = this->events_manager->begin(); it != ite; it++)
			this->poll_events->add_fd(it->first, it->second->getEventsFlags());
	}

} // namespace Webserv
