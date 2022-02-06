#ifndef METHODS_HPP
# define METHODS_HPP

# include <fcntl.h>
# include <unistd.h>

# include "../utils/Singleton.hpp"
# include "../http/HttpRequest.hpp"
# include "../http/HttpResponse.hpp"
# include "../utils/Resource.hpp"
# include "../core/Config.hpp"
# include "../events/Poll.hpp"
# include "../http/Server.hpp"

namespace Webserv
{
namespace Methods {
	class Methods : public Webserv::Utils::Singleton<Methods> {
		public:
			typedef Webserv::Utils::Resource	resource_type;
			typedef Webserv::Http::HttpRequest	http_request_type;
			typedef Webserv::Http::HttpResponse	http_response_type;
			typedef Webserv::Http::Server		http_server_type;

		private:
			static int	getMethod(http_request_type req, http_response_type &response, http_server_type srv);
			static int	postMethod(http_request_type req, http_response_type &response, http_server_type srv);
			static int	deleteMethod(http_request_type req, http_response_type &response, http_server_type srv);

		public:
			Methods(void);
			~Methods(void);

			static int	exec_method(http_request_type req, http_response_type &response, http_server_type srv);

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