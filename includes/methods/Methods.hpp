#ifndef METHODS_HPP
# define METHODS_HPP

# include "../utils/Singleton.hpp"
# include "../events/CGIEvent.hpp"
# include "../http/HttpRequest.hpp"
# include "../Resource.hpp"

namespace Webserv
{
	class Methods : public Webserv::Utils::Singleton<Methods> {
		private:
			void	getMethod(Webserv::Http::HttpRequest req, Webserv::Resource rcs);
			void	postMethod(Webserv::Http::HttpRequest req, Webserv::Resource rcs);
			void	deleteMethod(Webserv::Http::HttpRequest req, Webserv::Resource rcs);

		public:
			Methods(void);
			~Methods(void);
			void    exec_method(Webserv::Http::HttpRequest req, Webserv::Resource rcs/* Webserv::Http::Server srv*/);

			struct	methodsBadName: public std::exception
			{
				virtual const char* what() const throw()
				{
					return ("Methods: Bad method name");
				}
			};
	};
}	// namespace Webserv

#endif