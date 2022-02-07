#include "../../includes/events/ClientEvent.hpp"

namespace Webserv
{
	ClientEvent::ClientEvent(socket_type &client_sock, socket_type &server_sock, config_type& _config, env_type& environnement): sock(client_sock), srv_sock(server_sock), config(_config), env(environnement)
	{
		this->cgi = NULL;
		this->events_flags = POLLIN;
	}

	ClientEvent::~ClientEvent(void)
	{
		if (this->sock.getFd())
			this->sock.close();
	}

	void	ClientEvent::read_event(void)	//TO DO replace by ConstructRequest and add Methods
	{
		std::cout << "Client read event: " << this->srv_sock.getAddress().getStrAddress() << ":" << this->srv_sock.getAddress().getIntPort() <<std::endl;
		char buffer[2048];
		size_t	size;
		int	ret;

		size = this->sock.read(buffer, 2048);
		if (size < 0)
			throw ClientEventReadFailed();
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
				std::cout<<"Request: "<<request_string<<std::endl;
				http_request_list&	requests = this->create_req.getAllRequests();
				http_request_list::const_iterator request = requests.begin();

				while (request != requests.end())
				{
					std::cout<<"BODY REQUEST: "<<request->getBody()<<std::endl;
					http_response_type response;
					if (!request->hasHeader("Host"))
					{
					 	response.setStatusCode(http_response_type::status_code_type::client_error_bad_request);
					 	this->responses.push_back(response);
					 	request++;
					 	continue ;
					}
					http_server_type	srv = this->config.getServer(this->srv_sock.getAddress().getStrAddress(), this->srv_sock.getAddress().getIntPort(), request->getHeader("Host"));
					ret = Webserv::Methods::Methods::exec_method(*request, response, srv);
					if (ret < 0)
					{
						response.setStatusCode(http_response_type::status_code_type::client_error_bad_request);
						this->responses.push_back(response);
					 	request++;
					 	continue ;
					}
					if (ret > 0)
					{
						this->responses.push_back(response);
					 	request++;
					 	continue ;
					}
					http_route_type		route = getRoute(request->getBasePath(), srv.getRoutes(), srv.getDefaultRoute());
					resource_type		rcs;
					try
					{
						rcs = resource_type(route.getFilePath(request->getBasePath()), false);
						if (rcs.isCGI())
						{
							CGIEvent cgi(*request, srv, this->env);
							cgi.exec();
							rcs.setFd(this->cgi->getReadFD());
						}
					}
					catch (const std::exception& e)
					{
					 	response.setStatusCode(http_response_type::status_code_type::client_error_not_found);
					 	this->responses.push_back(response);
						std::cout << this->responses.size() << "first:" << e.what() << std::endl;
					 	request++;
					 	continue ;
					}
					try
					{
						while (!rcs.isFullyRead()) {
							rcs.loadResource();
						}
						response.setResource(rcs);
						this->responses.push_back(response);
					}
					catch(const std::exception& e)
					{
					 	response.setStatusCode(http_response_type::status_code_type::server_error_internal_server_error);
					 	this->responses.push_back(response);
						std::cout << this->responses.size() << "second:" << e.what() << std::endl;
					 	request++;
					 	continue ;
					}
					request++;
				}
				requests.clear();
				this->events_flags = POLLOUT | POLLHUP;
			}
		}
	}

	void	ClientEvent::write_event(void)
	{
		std::cout<<"Client write event"<<std::endl;

		response_vector::const_iterator response = this->responses.begin();
		if (response != responses.end())
		{
			this->sock.write(response->toString().c_str(), response->toString().length());
			this->responses.erase(response);
			return;
		}
		responses.clear();
		this->events_flags = POLLIN | POLLHUP;
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
