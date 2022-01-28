#include "../../includes/events/ClientEvent.hpp"

namespace Webserv
{
	ClientEvent::ClientEvent(Socket &client_sock, Socket &server_sock, Webserv::Config &_config): sock(client_sock), srv_sock(server_sock), config(_config)
	{
		this->cgi = NULL;
		this->events_flags = POLLIN;
	}

	ClientEvent::~ClientEvent(void)
	{
		if (this->sock.fd())
			this->sock.close();
		if (this->rcs)
			delete this->rcs;
	}

	void	ClientEvent::read_event(void)	//TO DO replace by ConstructRequest and add Methods
	{
		std::cout << "Client read event: " << this->srv_sock.address().getStrAddress() << ":" << this->srv_sock.address().getIntPort() <<std::endl;
		std::string path;
		char buffer[2048];
		size_t	size;

		size = this->sock.read(buffer, 2048);
		buffer[size] = '\0';
		request_string += buffer;
		this->create_req.addMessage(buffer);
		if (this->create_req.checkBuffer() >= 1)
		{
			if (this->create_req.parseRequests() == true)
			{
				this->req = this->create_req.getAllRequests()[0];
				std::cout<<"Request create"<<std::endl;
				this->srv = this->config.getServer(this->srv_sock.address().getStrAddress(), this->srv_sock.address().getIntPort(), this->req.get("Host"));
				std::cout << "Server was choice:"<< this->srv.getServerName() << std::endl;
				this->route = getRoute(this->req.getBaseUrl(), this->srv.getRoutes(), this->srv.getDefaultRoute());
				std::cout << "Route was choice:"<< this->route.getRoot() << std::endl;
				path = this->route.getRoot() + this->req.getBaseUrl();
				std::cout << "path: " << path << std::endl;
				// exit(0);
				this->rcs = new Resource(path, false);
				if (this->rcs->isCGI())
				{
					this->cgi = new CGIEvent(this->create_req.getAllRequests()[0]);
					this->rcs->setFd(this->cgi->getReadFD());
				}
	//			Webserv::Methods::Methods::getInstance().exec_method(this->req, this->rcs/*, this->srv**/);
				this->events_flags = POLLOUT;
			}
		}
	}

	void	ClientEvent::write_event(void)	//TO DO replace
	{
		int status = 0;
		std::cout<<"Client write event"<<std::endl;
		if (this->rcs->isCGI() && !this->cgi->CGIIsEnd())
		{
			if (this->cgi->writeIsEnd())
			{
				status = this->cgi->exec();	//peut etre recup le ret
				perror(strerror(status));
				std::cout<<"status: "<<status<<std::endl;
			}
			else
				this->cgi->write_event();
			return;
		}
		if (this->rcs->loadResource())
		{
			Webserv::Http::HttpResponse response(*this->rcs);
			this->sock.write(response.toString().c_str(), response.toString().length());
			std::cout<<"delete rcs"<<std::endl;
			if (this->rcs)
				delete this->rcs;
			this->rcs = NULL;
			std::cout<<"delete cgi"<<std::endl;
			if (this->cgi)
				delete this->cgi;
			this->events_flags = POLLIN;
		}
		else
		{
			std::cout<<"hmm"<<std::endl;
		}
	}

	short	ClientEvent::getEventsFlags(void)
	{
		return this->events_flags;
	}

	int		ClientEvent::getFD(void)
	{
		return this->sock.fd();	//TO DO replace
	}

	////////////////////////

		Webserv::Config::server_type::route_type	ClientEvent::getRoute(const std::string& url, const Webserv::Config::server_type::routes_map& routes, const Webserv::Config::server_type::route_type& defaultRoute) {
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

		if (max < paths.size() && !route.getRoutes().empty()) {
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

		std::vector<std::string>	ClientEvent::split(const std::string& str, char delim) {
		std::vector<std::string> strings;
		size_t start;
		size_t end = 0;
		while ((start = str.find_first_not_of(delim, end)) != std::string::npos) {
			end = str.find(delim, start);
			strings.push_back(str.substr(start, end - start));
		}
		return strings;
	}

}	// namespace Webserv