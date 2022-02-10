#include "../../includes/events/ClientEvent.hpp"

namespace Webserv
{
	ClientEvent::ClientEvent(socket_type &client_sock, socket_type &server_sock, config_type& _config, env_type& environnement, logger_type& log): sock(client_sock), srv_sock(server_sock), config(_config), env(environnement), logger(log)
	{
		this->events_flags = POLLIN;
	}

	ClientEvent::~ClientEvent(void)
	{
		if (this->sock.getFd())
			this->sock.close();
	}

	void	ClientEvent::read_event(void)	//TO DO replace by ConstructRequest and add Methods
	{
		char buffer[BUFFER_SIZE + 1];
		
		ssize_t	size;
		int	ret;

		size = this->sock.read(buffer, BUFFER_SIZE);
		if (size < 0)
			throw ClientEventReadFailed();
		if (size == 0) {
			throw ClientClosedConnectionEvent();
		}
		buffer[size] = '\0';
		this->logger << std::make_pair(this->logger.DEBUG, "Read ") << size << " bytes."  << std::endl;
		this->create_req.addMessage(std::string(buffer, size));
		if (this->create_req.checkBuffer() >= 1)
		{
			if (this->create_req.parseRequests() == true)
			{
				http_request_list&	requests = this->create_req.getAllRequests();
				this->logger << std::make_pair(this->logger.DEBUG, "") << "fd: " << this->getFD() << " New Request received: " << requests.size() << std::endl;
				http_request_list::const_iterator request = requests.begin();

				while (request != requests.end())
				{
					errno = 0;
					http_response_type response;
					if (!request->hasHeader("Host"))
					{
					 	response.setStatusCode(http_response_type::status_code_type::client_error_bad_request);
					 	this->responses.push_back(response);
					 	request++;
					 	continue ;
					}
					http_server_type	srv = this->config.getServer(this->srv_sock.getAddress().getStrAddress(), this->srv_sock.getAddress().getIntPort(), request->getHeader("Host"));
					http_route_type		route = getRoute(request->getBasePath(), srv.getRoutes(), srv.getDefaultRoute());
					bool				isCGI = false;
					try
					{
						ret = Webserv::Methods::Methods::exec_method(*request, response, srv, route);
					}
					catch (const Webserv::Methods::Methods::ForbiddenMethodException& e)
					{
						response.setStatusCode(http_response_type::status_code_type::client_error_forbidden);
						this->responses.push_back(response);
						request++;
						continue ;
					}
					catch (const Webserv::Methods::Methods::MethodsFcntlError& e)
					{
						response.setStatusCode(http_response_type::status_code_type::client_error_forbidden);
						this->responses.push_back(response);
						request++;
						continue ;
					}
					if (ret < 0)
					{
						response.setStatusCode(http_response_type::status_code_type::server_error_internal_server_error);
						this->responses.push_back(response);
					 	request++;
					 	continue ;
					}
					else if (ret == 1)
					{
						this->responses.push_back(response);
					 	request++;
					 	continue ;
					}
					else if (ret == 2)
						isCGI = true;
					resource_type		rcs;
					CGIEvent cgi;
					try
					{
						rcs.init(route.getFilePath(request->getBasePath()), isCGI, route);
						if (rcs.isCGI())
						{
							cgi.init(*request, srv, this->env, route);
							cgi.exec(); // TODO : check return
							rcs.setFd(cgi.getReadFD());
						}
					}
					catch (const resource_type::Resource404Exception& e)
					{
						response.setStatusCode(http_response_type::status_code_type::client_error_not_found);
						this->responses.push_back(response);
						request++;
						continue ;
					}
					catch (const resource_type::Resource403Exception& e)
					{
						response.setStatusCode(http_response_type::status_code_type::client_error_forbidden);
						this->responses.push_back(response);
						request++;
						continue ;
					}
					catch (const resource_type::Resource500Exception& e)
					{
						response.setStatusCode(http_response_type::status_code_type::server_error_internal_server_error);
						this->responses.push_back(response);
						this->logger << std::make_pair(this->logger.ERROR, e.what())  << std::endl;
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
					catch(const resource_type::Resource500Exception& e)
					{
					 	response.setStatusCode(http_response_type::status_code_type::server_error_internal_server_error);
					 	this->responses.push_back(response);
						this->logger << std::make_pair(this->logger.ERROR, e.what())  << std::endl;
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
		this->logger << std::make_pair(this->logger.DEBUG, "Client write event")  << std::endl;
		response_vector::iterator response = this->responses.begin();
		if (response != responses.end())
		{
			this->sock.write(response->toString().c_str(), response->toString().length());
			this->responses.erase(response);
			return;
		}
		this->events_flags = POLLIN;
	}

	short	ClientEvent::getEventsFlags(void)
	{
		return this->events_flags;
	}

	int		ClientEvent::getFD(void)
	{
		return this->sock.getFd();
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
