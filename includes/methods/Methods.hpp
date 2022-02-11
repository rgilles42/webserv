#ifndef METHODS_HPP
# define METHODS_HPP

# include <fcntl.h>
# include <unistd.h>
# include <cstdio>
# include <sys/stat.h>

# include "../utils/Singleton.hpp"
# include "../http/HttpRequest.hpp"
# include "../http/HttpResponse.hpp"
# include "../utils/Resource.hpp"
# include "../core/Config.hpp"
# include "../events/Poll.hpp"
# include "../http/Server.hpp"
# include "../http/Route.hpp"
# include "../utils/FileParser.hpp"		// For FileParser


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
			typedef Webserv::Utils::FileParser	file_parser_type;

			typedef Webserv::Poll				poll_type;

		private:
			static int			getMethod(const http_request_type &req, http_route_type& route);
			static int			postMethod(const http_request_type &req, http_response_type &response, const http_server_type &srv, http_route_type& route);
			static int			deleteMethod(const http_request_type& req, http_response_type &response, http_route_type& route);

			static int			isCGI(const http_request_type &req, http_route_type& route);
			static bool			isMethodAllowed(const std::vector<std::string>& methods, const std::string& methodname);
			static std::string	parseMultiform(const std::string& body, std::string& content);
			static int			makePath(std::string path, mode_t mode);

		public:
			Methods(void);
			~Methods(void);

			static int	exec_method(const http_request_type &req, http_response_type &response, const http_server_type &srv, http_route_type& route);

			struct	MethodsFcntlError : public std::exception
			{
				virtual const char* what() const throw()
				{
					return ("Methods: Fnctl failed");
				}
			};
			struct	ForbiddenMethodException : public std::exception
			{
				virtual const char* what() const throw()
				{
					return ("Forbidden Method on route");
				}
			};
			struct	NoUploadPathException : public std::exception
			{
				virtual const char* what() const throw()
				{
					return ("No upload path");
				}
			};
			struct	NoSuchFileOrDirException : public std::exception
			{
				virtual const char* what() const throw()
				{
					return ("No such file or directory");
				}
			};
			struct	ForbiddenException : public std::exception
			{
				virtual const char* what() const throw()
				{
					return ("Access Forbidden");
				}
			};
	};
}	// namespace Methods
}	// namespace Webserv

#endif