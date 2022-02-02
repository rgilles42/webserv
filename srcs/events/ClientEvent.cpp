#include "../../includes/events/ClientEvent.hpp"

namespace Webserv
{
	ClientEvent::ClientEvent(socket_type &client_sock, socket_type &server_sock, config_type& _config): sock(client_sock), srv_sock(server_sock), config(_config)
	{
		this->cgi = NULL;
		this->events_flags = POLLIN;
	}

	ClientEvent::~ClientEvent(void)
	{
		if (this->sock.getFd())
			this->sock.close();
		if (this->rcs)
			delete this->rcs;
	}

	void	ClientEvent::read_event(void)	//TO DO replace by ConstructRequest and add Methods
	{
		std::cout << "Client read event: " << this->srv_sock.getAddress().getStrAddress() << ":" << this->srv_sock.getAddress().getIntPort() <<std::endl;
		char buffer[2048];
		size_t	size;

		size = this->sock.read(buffer, 2048);
		if (size == 0) {
			this->events_flags = POLLOUT | POLLHUP;
			return ;
		}
		buffer[size] = '\0';
		request_string += buffer;
		this->create_req.addMessage(buffer);
		if (this->create_req.checkBuffer() >= 1)
		{
			if (this->create_req.parseRequests() == true)
			{
				std::cout<<"Request create"<<std::endl;
				http_request_list&	requests = this->create_req.getAllRequests();
				http_request_list::const_iterator request = requests.begin();

				while (request != requests.end()) {
					// https_response_type response;
					// if (!request->has("Host")) {
					// 	response.status("400 Bad Request");
					// 	this->responses.push_back(response);
					// 	request++;
					// 	continue ;
					// }
					http_server_type	srv = this->config.getServer(this->srv_sock.getAddress().getStrAddress(), this->srv_sock.getAddress().getIntPort(), request->get("Host"));
					std::cout << "Server was choice:"<< srv.getServerName() << std::endl;
					http_route_type	route = getRoute(request->getBaseUrl(), srv.getRoutes(), srv.getDefaultRoute());
					std::cout << "Route was choice:"<< route.getRoot() << "|" << route.getCurrentPath() << "|" << std::endl;
					std::cout << "path: " << route.getFilePath(request->getBaseUrl()) << std::endl;
					// exit(0);
					try {
						this->rcs = new resource_type(route.getFilePath(request->getBaseUrl()), false);
						if (this->rcs->isCGI())
						{
							this->cgi = new CGIEvent(this->create_req.getAllRequests()[0]);
							this->rcs->setFd(this->cgi->getReadFD());
						}
					} catch (const std::exception& e) {
						std::cout << e.what() << std::endl;
					}
		//			Webserv::Methods::Methods::getInstance().exec_method(this->req, this->rcs/*, srv**/);
					request++;
				}
				requests.clear();
				this->events_flags = POLLOUT | POLLHUP;
			}
		}
	}

	void	ClientEvent::write_event(void)	//TO DO replace
	{
		std::cout<<"Client write event"<<std::endl;

		// response_vector::const_iterator response = this->responses.begin();

		// while (response != responses.end()) {
		// 	this->sock.write(response->toString().c_str(), response->toString().length());
		// 	response++;
		// }
		// responses.clear();
		// this->events_flags = POLLIN;

		int status = 0;
		if (!this->rcs) {
			this->events_flags = POLLIN | POLLHUP;
			Webserv::Http::HttpResponse response;
			response.setStatusCode(Webserv::Http::HttpResponse::status_code_type::client_error_not_found);
			this->sock.write(response.toString().c_str(), response.toString().length());
			return ;
		}
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
			Webserv::Http::HttpResponse response;
			response.setResource(*this->rcs);
			this->sock.write(response.toString().c_str(), response.toString().length());
			std::cout<<"delete rcs"<<std::endl;
			if (this->rcs)
				delete this->rcs;
			this->rcs = NULL;
			std::cout<<"delete cgi"<<std::endl;
			if (this->cgi)
				delete this->cgi;
			this->cgi = NULL;
			this->events_flags = POLLIN | POLLHUP;
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
		return this->sock.getFd();	//TO DO replace
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

		if (max != 0 && max < paths.size() && !route.getRoutes().empty()) {
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