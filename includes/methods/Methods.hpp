#ifndef METHODS_HPP
# define METHODS_HPP

# include "../utils/Singleton.hpp"
# include "../http/HttpRequest.hpp"
# include "../Resource.hpp"
# include "../Config.hpp"

namespace Webserv
{
namespace Methods {
	class Methods : public Webserv::Utils::Singleton<Methods> {
		private:
			int	getMethod(Webserv::Http::HttpRequest req, Webserv::Resource *rcs);
			int	postMethod(Webserv::Http::HttpRequest req, Webserv::Resource *rcs);
			int	deleteMethod(Webserv::Http::HttpRequest req, Webserv::Resource *rcs);

		public:
			Methods(void);
			~Methods(void);

			int    exec_method(Webserv::Http::HttpRequest req, Webserv::Resource *rcs/* Webserv::Http::Server srv*/);

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