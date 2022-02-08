#ifndef METHODS_HPP
# define METHODS_HPP

# include <fcntl.h>
# include <unistd.h>
# include <cstdio>

# include "../utils/Singleton.hpp"
# include "../http/HttpRequest.hpp"
# include "../http/HttpResponse.hpp"
# include "../utils/Resource.hpp"
# include "../core/Config.hpp"
# include "../events/Poll.hpp"
# include "../http/Server.hpp"
# include "../http/Route.hpp"

namespace Webserv
{
namespace Methods {
	class Methods : public Webserv::Utils::Singleton<Methods> {
		public:
			typedef Webserv::Utils::Resource	resource_type;
			typedef Webserv::Http::HttpRequest	http_request_type;
			typedef Webserv::Http::HttpResponse	http_response_type;
			typedef Webserv::Http::Server		http_server_type;
			typedef Webserv::Http::Route		http_route_type;

		private:
			static int	getMethod(const http_request_type &req, http_route_type& route);
			static int	postMethod(const http_request_type &req, http_response_type &response, const http_server_type &srv, http_route_type& route);
			static int	deleteMethod(const http_request_type& req, http_response_type &response, http_route_type& route);

			static int	isCGI(const http_request_type &req, http_route_type& route);

		public:
			Methods(void);
			~Methods(void);

			static int	exec_method(const http_request_type &req, http_response_type &response, const http_server_type &srv, http_route_type& route);

			struct	MethodsFcntlError: public std::exception
			{
				virtual const char* what() const throw()
				{
					return ("Methods: Fnctl failed");
				}
			};
	};
}	// namespace Methods
}	// namespace Webserv

#endif