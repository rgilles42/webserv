/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 14:05:38 by ppaglier          #+#    #+#             */
/*   Updated: 2022/01/18 15:36:21 by ppaglier         ###   ########.fr       */
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

	Webserv::Config::server_type	getServer(const Socket& sock, const Webserv::Http::HttpRequest& request, Webserv::Config::server_vector& servers) {

		Webserv::Config::server_vector serversTmp = servers;
		Webserv::Config::server_vector::iterator it = serversTmp.begin();
		(void)request;

		while (it != serversTmp.end()) {
			if (sock.address().getIntPort() != it->getListen().getIntPort()) {
				it = serversTmp.erase(it);
				continue ;
			}
			// TODO: care of 0.0.0.0 with this (for now it's safe)
			if (sock.address().getStrAddress() != it->getListen().getStrAddress()) {
				it = serversTmp.erase(it);
				continue ;
			}
			if (request.get("Host") == it->getServerName()) {
				return *it;
			}
			// std::cout << it->getListen().getStrAddress() << ":" << it->getListen().getIntPort() << std::endl;
			it++;
		}
		return serversTmp.front();
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
		this->servers = this->config.getServers();
		server_vector::const_iterator it = this->servers.begin();
		std::vector<std::string> listens;
		while (it != this->servers.end()) {
			std::ostringstream ss;
			ss << it->getListen().getStrAddress() << ":" << it->getListen().getIntPort();
			std::cout<<"Port getListen: "<<it->getListen().getIntPort()<<std::endl;
			std::string listen = ss.str();
			if (std::find(listens.begin(), listens.end(), listen) == listens.end()) {
				listens.push_back(listen);
				socket_type newSocket(it->getListen().getStrAddress().c_str(), it->getListen().getIntPort());
				this->serversSockets.push_back(newSocket);
				// this->logger << listen << std::endl;
			}
			it++;
		}
		socket_vector::const_iterator it2 = this->serversSockets.begin();
		while (it2 != this->serversSockets.end()) {
			this->add_server_event(*it2);
			it2++;
		}
		this->isInit = true;


		// Webserv::Http::HttpRequestBuilder builder;
		// {
		// 	std::string message = "             c'est le body sa mer     ";

		// 	builder.addMessage("GET /index HTTP/1.1\r\n");
		// 	builder.addMessage("Host: code.tutsplus.com\r\n");
		// 	builder.addMessage("Connection: keep-alive\r\n");
		// 	builder.addMessage("Content-Lenght: "+ SSTR(message.length()) +"\r\n");
		// 	builder.addMessage("\r\n");
		// 	builder.addMessage(message);

		// 	builder.addMessage("GET / HTTP/1.1\r\nHost: 127.0.0.1:8080\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\nsec-ch-ua: \" Not A;Brand\";v=\"99\", \"Chromium\";v=\"96\", \"Google Chrome\";v=\"96\"\r\nsec-ch-ua-mobile: ?0\r\nsec-ch-ua-platform: \"macOS\"\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.110 Safari/537.36\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\nSec-Fetch-Site: none\r\nSec-Fetch-Mode: navigate\r\nSec-Fetch-User: ?1\r\nSec-Fetch-Dest: document\r\nAccept-Encoding: gzip, deflate, br\r\nAccept-Language: fr-FR,fr;q=0.9,en-US;q=0.8,en;q=0.7\r\n\r\n");

		// 	builder.addMessage("GET /oui/test/non HTTP/1.1\r\n");
		// 	builder.addMessage("Host: localhost2.com\r\n");
		// 	builder.addMessage("Connection: keep-alive\r\n");
		// 	builder.addMessage("Content-Lenght: "+ SSTR(message.length()) +"\r\n");
		// 	builder.addMessage("\r\n");
		// 	builder.addMessage(message);

		// 	if (builder.checkBuffer() > 0) {
		// 		if (builder.parseRequests()) {
		// 			Webserv::Http::HttpRequestBuilder::request_list &requests = builder.getAllRequests();
		// 			Webserv::Http::HttpRequestBuilder::request_list::const_iterator it = requests.begin();
		// 			while (it != requests.end()) {
		// 				Socket currentSocket("127.0.0.1", 8080);
		// 				std::cout << "For:" << it->get("Host") << std::endl;
		// 				server_type server = getServer(currentSocket, *it, this->servers);
		// 				std::cout << "server:" << server.getServerName() << "|" << server.getListen().getStrAddress() << ":" << server.getListen().getIntPort() << std::endl;
		// 				// server_type::route_type route = getRoute(it->getBaseUrl(), server.getRoutes(), server.getRoutes().begin()->second);
		// 				// std::cout << "route:" << &route << std::endl;

		// 				it++;
		// 			}
		// 		}
		// 	}
		// }

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
		std::cout<<"Exec"<<std::endl;
		std::vector<struct pollfd>::iterator ite;

		socket_vector::iterator it;

		it = this->serversSockets.begin();
		while (it != this->serversSockets.end()) {
			std::cout<<"Portserv: "<<it->address().getIntPort()<<" fd:"<<it->fd()<<std::endl;
			if (it->bind() < 0) {
				perror("socket bind");
			}
			it++;
		}

		it = this->serversSockets.begin();
		while (it != this->serversSockets.end()) {
			if (it->listen() < 0) {
				perror("socket listen");
			}
			it++;
		}

		std::cout<<"Start POLL Event"<<std::endl;
		/* END RM */
		try
		{
			while (true)
			{
				setup_events();
				try {
					this->poll_events.exec();
				}
				catch (const std::exception &e)
				{
					std::cout<<e.what()<<std::endl;
					throw coreExecFailed();
				}
				ite = poll_events.end();
				for (std::vector<struct pollfd>::iterator it = this->poll_events.begin(); it != ite; it++)
				{
					if ((it->revents & (POLLHUP | POLLERR)) > 0)
					{
						std::cout<<"POLLHUP | POLLER Event on fd: "<<it->fd<<std::endl;
						this->events_manager.remove_event(it->fd);
					}
					else if ((it->revents & POLLIN) == POLLIN)
					{
						std::cout<<"POLLIN Event on fd: "<<it->fd<<std::endl;
						this->events_manager.get_event(it->fd)->read_event();
					}
					else if ((it->revents & POLLOUT) == POLLOUT)
					{
						std::cout<<"POLLOUT Event on fd: "<<it->fd<<std::endl;
						this->events_manager.get_event(it->fd)->write_event();
					}
					else if (it->revents == 0)
					{
						std::cout<<"Other event on fd: "<<it->fd<<std::endl;
					}
				}
			}
		}
		catch (std::exception &e)
		{
			std::cout << e.what() <<std::endl;
			throw coreExecFailed();
		}
	}

	void		Core::add_server_event(const Socket &sock)	// Add Server Event in EventsManager
	{
		ServerEvent	*srv = new ServerEvent(sock, this->config);

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
		std::map<int, IEvents *>::iterator	ite = this->events_manager.end();

		for (std::map<int, IEvents *>::iterator	it = this->events_manager.begin(); it != ite; it++)
			this->poll_events.add_fd(it->first, it->second->getEventsFlags());
	}

} // namespace Webserv
