#ifndef METHODS_HPP
# define METHODS_HPP

# include "../utils/Singleton.hpp"
# include "../http/HttpRequest.hpp"
# include "../utils/Resource.hpp"
# include "../core/Config.hpp"

namespace Webserv
{
namespace Methods {
	class Methods : public Webserv::Utils::Singleton<Methods> {
		public:
			typedef Webserv::Utils::Resource	resource_type;
			typedef Webserv::Http::HttpRequest	http_request_type;

		private:
			int	getMethod(http_request_type req, resource_type* rcs);
			int	postMethod(http_request_type req, resource_type* rcs);
			int	deleteMethod(http_request_type req, resource_type* rcs);

		public:
			Methods(void);
			~Methods(void);

			int    exec_method(http_request_type req, resource_type* rcs/* Webserv::Http::Server srv*/);

			struct	methodsBadName: public std::exception
			{
				virtual const char* what() const throw()
				{
					return ("Methods: Bad method name");
				}
			};
	};
}	// namespace Methods
}	// namespace Webserv

#endif