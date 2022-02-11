#include "../../includes/events/ClientEvent.hpp"

namespace Webserv
{
	ClientEvent::ClientEvent(socket_type &client_sock, socket_type &server_sock, config_type& _config, env_type& environnement, logger_type& log): sock(client_sock), srv_sock(server_sock), config(_config), env(environnement), logger(log)
	{
		this->events_flags = POLLIN;
	}

	ClientEvent::~ClientEvent(void)
	{
		if (this->sock.getFd()) {
			this->sock.close();
		}
	}

	void	ClientEvent::read_event(void)
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
					resource_type		rcs;
					errno = 0;
					http_response_type response;
					if (!request->hasHeader("Host"))
					{
					 	response.setStatusCode(http_response_type::status_code_type::client_error_bad_request);
					 	this->responses.push_back(response);
					 	this->written_sizes.push_back(0);
					 	request++;
					 	continue ;
					}
					http_server_type	srv = this->config.getServer(this->srv_sock.getAddress().getStrAddress(), this->srv_sock.getAddress().getIntPort(), request->getHeader("Host"));
					http_route_type		route = getRoute(request->getBasePath(), srv.getRoutes(), srv.getDefaultRoute());
					bool				isCGI = false;

					if (route.getReturn().first != http_route_type::http_status_code_type::unknown || !route.getReturn().second.empty()) {
						if (!route.getReturn().second.empty() && route.getReturn().first == http_route_type::http_status_code_type::unknown) {
							response.setRedirect(route.getReturn().second, http_route_type::http_status_code_type::redirection_found);
						} else {
							response.setRedirect(route.getReturn().second, route.getReturn().first);
						}
						this->responses.push_back(response);
						this->written_sizes.push_back(0);
						request++;
						continue ;
					}
					if (route.getClientMaxBodySize().toUnit(http_route_type::client_max_body_size_type::Byte::U_B) < request->getBody().length()) {
						this->setToError(response, route, rcs, http_response_type::status_code_type::client_error_payload_too_large);
						this->responses.push_back(response);
						this->written_sizes.push_back(0);
						request++;
						continue ;
					}
					struct stat s;
					int err = stat(route.getFilePath(request->getBasePath()).c_str(), &s);
					if (err == 0 && S_ISDIR(s.st_mode) && *(request->getBasePath().rbegin()) != '/'){
						std::cout << request->getBasePath() << std::endl;
						response.setRedirect(request->getBasePath() + "/" + (!request->getQuery().empty() ? "?" + request->getQuery() : "" ), http_route_type::http_status_code_type::redirection_moved_permanently);
						this->responses.push_back(response);
						this->written_sizes.push_back(0);
						request++;
						continue ;
					}
					try
					{
						ret = Webserv::Methods::Methods::exec_method(*request, response, srv, route);
					}
					catch (const Webserv::Methods::Methods::ForbiddenMethodException& e)
					{
						this->setToError(response, route, rcs, http_response_type::status_code_type::client_error_method_not_allowed);
						this->responses.push_back(response);
						this->written_sizes.push_back(0);
						request++;
						continue ;
					}
					catch (const Webserv::Methods::Methods::MethodsFcntlError& e)
					{
						this->setToError(response, route, rcs, http_response_type::status_code_type::server_error_internal_server_error);
						this->responses.push_back(response);
						this->written_sizes.push_back(0);
						request++;
						continue ;
					}
					if (ret < 0)
					{
						this->setToError(response, route, rcs, http_response_type::status_code_type::server_error_internal_server_error);
						this->responses.push_back(response);
						this->written_sizes.push_back(0);
					 	request++;
					 	continue ;
					}
					else if (ret == 1)
					{
						this->responses.push_back(response);
						this->written_sizes.push_back(0);
					 	request++;
					 	continue ;
					}
					else if (ret == 2)
						isCGI = true;
					CGIEvent cgi;
					try
					{
						rcs.init(route.getFilePath(request->getBasePath()), isCGI, route);
						if (rcs.isCGI())
						{
							cgi.init(*request, srv, this->env, route);
							if (cgi.exec() != 0) {
								this->setToError(response, route, rcs, http_response_type::status_code_type::server_error_internal_server_error);
								this->responses.push_back(response);
								this->written_sizes.push_back(0);
								request++;
								continue;
							}
							rcs.setFd(cgi.getReadFD());
						}
					}
					catch (const CGIEvent::Cgi500Exception& e)
					{
						this->setToError(response, route, rcs, http_response_type::status_code_type::server_error_internal_server_error);
						this->responses.push_back(response);
						this->written_sizes.push_back(0);
						request++;
						continue;
					}
					catch (const resource_type::Resource404Exception& e)
					{
						this->setToError(response, route, rcs, http_response_type::status_code_type::client_error_not_found);
						this->responses.push_back(response);
						this->written_sizes.push_back(0);
						request++;
						continue ;
					}
					catch (const resource_type::Resource403Exception& e)
					{
						this->setToError(response, route, rcs, http_response_type::status_code_type::client_error_forbidden);
						this->responses.push_back(response);
						this->written_sizes.push_back(0);
						request++;
						continue ;
					}
					catch (const resource_type::Resource500Exception& e)
					{
						this->setToError(response, route, rcs, http_response_type::status_code_type::server_error_internal_server_error);
						this->responses.push_back(response);
						this->written_sizes.push_back(0);
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
						rcs.closeResource();
						this->responses.push_back(response);
						this->written_sizes.push_back(0);
					}
					catch(const resource_type::Resource500Exception& e)
					{
						rcs.closeResource();
					 	this->setToError(response, route, rcs, http_response_type::status_code_type::server_error_internal_server_error);
					 	this->responses.push_back(response);
					 	this->written_sizes.push_back(0);
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
		written_response_sizes::iterator written_size = this->written_sizes.begin();
		if (response != responses.end())
		{
			if (*written_size < response->toString().length())
			{
				size_t	ret = this->sock.write(&response->toString().c_str()[*written_size], response->toString().length() - *written_size);
				*written_size += ret;
			}
			if (*written_size >= response->toString().length())
			{
				this->responses.erase(response);
				this->written_sizes.erase(written_size);
			}
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

	Webserv::Config::server_type::route_type	ClientEvent::getRoute(const std::string& url, const Webserv::Config::server_type::routes_map& routes, const Webserv::Config::server_type::route_type& defaultRoute)
	{
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

	std::vector<std::string>	ClientEvent::split(const std::string& str, char delim)
	{
		std::vector<std::string> strings;
		size_t start;
		size_t end = 0;
		while ((start = str.find_first_not_of(delim, end)) != std::string::npos) {
			end = str.find(delim, start);
			strings.push_back(str.substr(start, end - start));
		}
		return strings;
	}

	void	ClientEvent::setToError(http_response_type& response, http_route_type& route, resource_type& rcs, http_response_type::status_code_type code)
	{
		http_route_type::error_pages_pair pairError = route.getErrorPage(code);
		if (pairError.second != "")
		{
			try
			{
				rcs.init(pairError.second, false, route);
				while (!rcs.isFullyRead())
					rcs.loadResource();
				response.setResource(rcs, code);
			}
			catch (const std::exception& e)
			{
				response.setStatusCode(code);
			}
		}
		else
			response.setStatusCode(code);
	}
}	// namespace Webserv
