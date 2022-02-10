# include "../../includes/methods/Methods.hpp"

namespace Webserv {
namespace Methods {
	Methods::Methods(void) {}

	Methods::~Methods(void) {}

	int    Methods::exec_method(const http_request_type &req, http_response_type &response, const http_server_type &srv, http_route_type& route)
	{
		if (req.getMethod().getMethod() == http_request_type::method_type::GET)
			return getMethod(req, route);
		else if (req.getMethod().getMethod() == http_request_type::method_type::POST)
			return postMethod(req, response, srv, route);
		else if (req.getMethod().getMethod() == http_request_type::method_type::DELETE)
			return deleteMethod(req, response, route);
		return -1;
	}
/*--------------------------------------------------------------------------------------------------------------*/
	int   Methods::getMethod(const http_request_type &req, http_route_type& route)
	{
		if (route.getEnableLimitExcept() && !Methods::isMethodAllowed(route.getLimitExcept(), "GET"))
			throw ForbiddenMethodException();
		return isCGI(req, route);
	}

	int    Methods::postMethod(const http_request_type &req, http_response_type &response, const http_server_type& srv, http_route_type& route)	// TO DO Modif for check if need CGI
	{
		if (route.getEnableLimitExcept() && !Methods::isMethodAllowed(route.getLimitExcept(), "POST"))
			throw ForbiddenMethodException();
		Poll	write_poll;
		int	fd_upload = -1;;
		size_t	bytes_written = 0;
		std::string	path_upload;
		std::vector<struct pollfd>::iterator it;
		ssize_t ret;

		if (isCGI(req, route) == 2)
			return 2;
		if (req.getBody().size() != 0)
		{
			path_upload = srv.getUploadStore();
			std::cout<<"Path upload: "<<path_upload<<std::endl;
			if (path_upload == "")
				throw	NoUploadPathException();
			std::string	content;
			std::string path = path_upload + "/" + Methods::parseMultiform(req.getBody(), content);
			fd_upload = open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
			if (fd_upload < 0)
				throw MethodsFcntlError();
			if (fcntl(fd_upload, F_SETFL, O_NONBLOCK) < 0)
			{
				close(fd_upload);
				throw MethodsFcntlError();
			}
			write_poll.add_fd(fd_upload, POLLOUT);
			while (1)
			{
				write_poll.exec();
				it = write_poll.begin();
				if ((it->revents & POLLOUT) == POLLOUT)
				{
					ret = write(fd_upload, &content.c_str()[bytes_written], content.size() - bytes_written);
					if (ret < 0)
					{
						close(fd_upload);
						throw MethodsFcntlError();
					}
					bytes_written += ret - 1;
					if (ret == 0 || bytes_written == content.length() - 1)
						break;
				}
			}
			std::cout<<"End upload"<<std::endl;
			if (fd_upload > 0)
				close(fd_upload);
			response.setStatusCode(http_response_type::status_code_type::success_created);
			return (1);
		}
		return 0;
	}

	std::string	Methods::parseMultiform(const std::string& body, std::string& content)
	{
		std::cout << body;
		std::string	filename;
	
		std::string	delim = body.substr(0, body.find("\r\n"));
		std::string file_part;
		int pos = delim.length() + 2;
		while (body.find(delim, pos) != std::string::nval)
		{
			if ((file_part = body.substr(pos, body.find(delim, pos) - pos)).find("filename"))
			{
				std::cout << "Found file part \n\"" << file_part << "\"\n";
				break ;
			}
			pos += file_part.length() + delim.length() + 2;
		} 
		
		content = body;
		return ((filename = "uwu.txt"));
	}

	int	Methods::deleteMethod(const http_request_type &req, http_response_type &response, http_route_type& route)
	{
		if (!route.getEnableLimitExcept() || (route.getEnableLimitExcept() && !Methods::isMethodAllowed(route.getLimitExcept(), "DELETE")))
			throw ForbiddenMethodException();
		if (isCGI(req, route) == 2)
			return 2;
		errno = 0;
		if (remove(route.getFilePath(req.getBasePath()).c_str()) != 0)
		{
			if (errno == ENOENT)
				response.setStatusCode(http_response_type::status_code_type::client_error_not_found);
			else if (errno == EACCES)
				response.setStatusCode(http_response_type::status_code_type::client_error_forbidden);
		}
		else
			response.setStatusCode(http_response_type::status_code_type::success_ok);
		return (1);
	}

	int	Methods::isCGI(const http_request_type &req, http_route_type& route)
	{
		size_t save_index;
		size_t index;

				std::string path = route.getFilePath(req.getBasePath());
				index = path.find('.');
				if (index == std::string::npos)
					return 0;
				while (index != std::string::npos)
				{
					save_index = index;
					index = path.find('.', index + 1);
				}
			std::string ext = &path[save_index + 1];
		if (route.getCgiPass().length() > 0)
		{
			if (route.getCgiExt().size() > 0)
			{
				std::string path = route.getFilePath(req.getBasePath());
				index = path.find('.');
				if (index == std::string::npos)
					return 0;
				while (index != std::string::npos)
				{
					save_index = index;
					index = path.find('.', index + 1);
				}
				std::string ext = &path[save_index + 1];
				//std::cout<<"ext: "<<ext<<std::endl;
				std::vector<std::string> vect_ext = route.getCgiExt();
				for (std::vector<std::string>::iterator it = vect_ext.begin(); it != vect_ext.end(); it++)
				{
					if (*it == ext)
						return 2;
				}
			}
		}
		return 0;
	}

	bool	Methods::isMethodAllowed(const std::vector<std::string>& methods, const std::string& methodname)
	{
		for (std::vector<std::string>::const_iterator it = methods.begin(); it != methods.end(); it++)
		{
			if (*it == methodname)
				return (true);
		}
		return (false);
	}
}	// namespace Methods
}   // namespace Webserv