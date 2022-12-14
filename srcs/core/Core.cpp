/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 14:05:38 by ppaglier          #+#    #+#             */
/*   Updated: 2022/02/11 22:49:38 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/core/Core.hpp"

namespace Webserv {

	Core::Core(void) : Singleton<Core>(), stop(false) {
		this->isInit = false;
		this->logger.setPrefix("\x1b[33m[Webserv]\x1b[0m");
		// TODO: if we want to disable DEBUG logs, just uncomment or make a flag use this code
		// this->logger.setLogMap(logger_type::DEBUG, NULL);
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
		this->logger << std::make_pair(logger_type::DEBUG, "PreInit: starting..") << std::endl;
		try
		{
			this->args.fromArg(argc, argv);
		}
		catch (const args_type::ArgException& e)
		{
			this->logger << std::make_pair(this->logger.ERROR, e.what()) << std::endl;
			exit(EXIT_FAILURE);
		}

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
		this->logger << std::make_pair(logger_type::DEBUG, "PreInit: done!") << std::endl;
		return true;
	}

	std::vector<std::string>	split(const std::string& str, char delim) {
		std::vector<std::string> strings;
		size_t start;
		size_t end = 0;
		while ((start = str.find_first_not_of(delim, end)) != std::string::npos) {
			end = str.find(delim, start);
			strings.push_back(str.substr(start, end - start));
		}
		return strings;
	}

	Webserv::Config::server_type::route_type	getRoute(const std::string& url, const Webserv::Config::server_type::routes_map& routes, const Webserv::Config::server_type::route_type& defaultRoute) {
		std::vector<std::string> paths = split(url, '/');
		Webserv::Config::server_type::route_type route = defaultRoute;

		size_t max = 0;
		Webserv::Config::server_type::routes_map::const_iterator itRoutes = routes.begin();
		while (itRoutes != routes.end()) {
			std::vector<std::string> routePaths = split(itRoutes->first, '/');
			size_t i = 0;
			while (i < routePaths.size() && i < paths.size()) {
				if (routePaths[i] != paths[i]) {
					break ;
				}
				if (max <= i) {
					route = itRoutes->second;
					max = i + 1;
				}
				i++;
			}
			itRoutes++;
		}
		if (max < paths.size()) {
			std::vector<std::string>::const_iterator it = paths.begin() + max;
			std::string newUrl;
			while (it != paths.end()) {
				newUrl += "/" + *it;
				it++;
			}
			route = getRoute(newUrl, route.getRoutes(), route);
		}

		return route;
	}

	bool		Core::init(void) {
		this->logger << std::make_pair(logger_type::DEBUG, "Init: starting..") << std::endl;
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
		try {
			if (!this->config.processFiles()) {
				return false;
			}
		} catch (const Config::ConfigException& e) {
			this->logger << std::make_pair(logger_type::ERROR, e.what()) << std::endl;
			return false;
		}
		this->servers = this->config.getServers();
		server_vector::const_iterator it = this->servers.begin();
		std::vector<std::string> listens;
		while (it != this->servers.end()) {
			std::ostringstream ss;
			ss << it->getListen().getStrAddress() << ":" << it->getListen().getIntPort();
			std::string listen = ss.str();
			this->logger << std::make_pair(logger_type::DEBUG, "Server detected: (") << it->getServerName() << ")" << it->getListen().getStrAddress() << ":" << it->getListen().getIntPort() << std::endl;
			if (std::find(listens.begin(), listens.end(), listen) == listens.end()) {
				listens.push_back(listen);
				socket_type newSocket(it->getListen().getStrAddress().c_str(), it->getListen().getIntPort());
				this->logger << std::make_pair(logger_type::DEBUG, "Socket created: ") << newSocket.getAddress().getStrAddress() << ":" << newSocket.getAddress().getIntPort() << std::endl;
				this->serversSockets.push_back(newSocket);
			}
			it++;
		}
		socket_vector::const_iterator it2 = this->serversSockets.begin();
		while (it2 != this->serversSockets.end()) {
			this->add_server_event(*it2);
			it2++;
		}
		this->isInit = true;
		this->logger << std::make_pair(logger_type::DEBUG, "Init: done!") << std::endl;
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

	void		Core::exec(void)
	{
		if (!this->isReady())
			return ;

		this->logger << std::make_pair(logger_type::DEBUG, "Starting servers: starting..") << std::endl;
		socket_vector::iterator it;

		errno = 0;
		it = this->serversSockets.begin();
		while (it != this->serversSockets.end()) {
			try
			{
				if (it->bind() < 0) {
					this->logger << std::make_pair(logger_type::ERROR, ExecException(strerror(errno)).what()) << std::endl;
					return ;
				}
			}
			catch (const std::exception& e)
			{
				this->logger << std::make_pair(logger_type::ERROR, ExecException(strerror(errno)).what()) << std::endl;
				return ;
			}
			it++;
		}

		it = this->serversSockets.begin();
		while (it != this->serversSockets.end()) {
			try
			{
				if (it->listen(MAX_CONNECTION) < 0) {
					this->logger << std::make_pair(logger_type::ERROR, ExecException(strerror(errno)).what()) << std::endl;
					return ;
				}
			}
			catch (const std::exception& e)
			{
				this->logger << std::make_pair(logger_type::ERROR, ExecException(strerror(errno)).what()) << std::endl;
				return ;
			}
			it++;
		}
		this->logger << std::make_pair(logger_type::INFO, "Server has succesfully started. Have fun!") << std::endl;
		/* END RM */
		struct sigaction sigIntHandler;
		sigIntHandler.sa_handler = sigint_handler;
		sigemptyset(&sigIntHandler.sa_mask);
		sigIntHandler.sa_flags = SA_RESTART;
		sigaction(SIGINT, &sigIntHandler, NULL);

		poll_type::poll_fd_vector::const_iterator poll_it;
		// try
		// {
			extern volatile std::sig_atomic_t	stop;
			while (!stop)
			{
				this->setup_events();
				try {
					this->poll_events.exec();
				}
				catch (const std::exception& e)
				{
					this->logger << std::make_pair(logger_type::ERROR, ExecException("poll_events exec error").what()) << std::endl;
					continue ;
				}

				for (poll_it = this->poll_events.getPollUsedFD().begin(); poll_it != this->poll_events.getPollUsedFD().end(); poll_it++)
				{
					if ((poll_it->revents & POLLIN) == POLLIN)
					{
						this->logger << std::make_pair(logger_type::DEBUG, "POLLIN Event on fd: ") << poll_it->fd<<std::endl;
						try {
							this->events_manager.get_event(poll_it->fd)->read_event();
						}
						catch (const std::exception& e)
						{
							this->logger << std::make_pair(logger_type::DEBUG, std::string(e.what()) + " on fd: ") << poll_it->fd<<std::endl;
							this->events_manager.remove_event(poll_it->fd);
						}
					}
					else if ((poll_it->revents & POLLHUP) == POLLHUP || (poll_it->revents & POLLERR) == POLLERR)
					{
						this->logger << std::make_pair(logger_type::DEBUG, "POLLHUP | POLLER Event on fd: ") << poll_it->fd<<std::endl;
						this->events_manager.remove_event(poll_it->fd);
					}
					else if ((poll_it->revents & POLLOUT) == POLLOUT)
					{
						this->logger << std::make_pair(logger_type::DEBUG, "POLLOUT Event on fd: ") << poll_it->fd<<std::endl;
						this->events_manager.get_event(poll_it->fd)->write_event();
					}
					else if ((poll_it->revents & POLLNVAL) == POLLNVAL)
					{
						this->logger << std::make_pair(logger_type::DEBUG, "POLLNVAL Event on fd: ") << poll_it->fd<<std::endl;
						this->events_manager.remove_event(poll_it->fd);
					}
					else if (poll_it->revents != 0 && errno != EINTR)
					{
						this->logger << std::make_pair(logger_type::DEBUG, "Other Event on fd: ") << poll_it->fd<<std::endl;
					}
				}
				if (stop)
					this->logger << std::make_pair(logger_type::INFO, "Shutting down...") << std::endl;
			}
		// }
		// catch (std::exception &e)
		// {
		// 	this->logger << std::make_pair(logger_type::ERROR, ExecException("exec loop error" + std::string(e.what())).what()) << std::endl;
		// }
	}

	void		Core::add_server_event(const socket_type &sock)	// Add Server Event in EventsManager
	{
		ServerEvent	*srv = new ServerEvent(sock, this->config, this->env, this->logger);

		this->events_manager.add_event(srv->getFD(), *srv);
	}

	void		Core::add_client_event(int fd, ClientEvent &client_e)	// Add Client Event in EventsManager
	{
		this->events_manager.add_event(fd, client_e);
	}

	void		Core::remove_event(int fd)	// Remove Event in EventsManager
	{
		this->events_manager.remove_event(fd);
	}

	void		Core::setup_events()	// Add all events fd in Poll
	{
		this->poll_events.clear();		// Clear old events fd
		event_manager_type::event_map_type::const_iterator	it;

		for (it = this->events_manager.getEvents().begin(); it != this->events_manager.getEvents().end(); it++)
			this->poll_events.add_fd(it->first, it->second->getEventsFlags());
	}

	Core::env_type	Core::getEnv()
	{
		return this->env;
	}
} // namespace Webserv
