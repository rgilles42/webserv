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
		return isCGI(req, route);
	}

	int    Methods::postMethod(const http_request_type &req, http_response_type &response, const http_server_type& srv, http_route_type& route)	// TO DO Modif for check if need CGI
	{
		Poll	write_poll;
		int	fd_upload = -1;;
		size_t	bytes_write = 0;
		std::string	path_upload;
		std::vector<struct pollfd>::iterator it;
		ssize_t ret;

		if (isCGI(req, route) == 2)
			return 2;
		std::cout<<"POST Method call"<<std::endl;
		std::cout<<"reqBody: "<<req.getBody()<<std::endl;
		if (req.getBody().size() != 0)
		{
			std::cout<<"Create file"<<std::endl;
			path_upload = srv.getUploadStore();
			std::cout<<"Path upload: "<<path_upload<<std::endl;
			fd_upload = open("./test.txt", O_WRONLY | O_CREAT | O_APPEND, 066);
			if (fd_upload < 0)
				throw MethodsFcntlError();
			if (fcntl(fd_upload, F_SETFL, O_NONBLOCK) < 0)
				throw MethodsFcntlError();
			write_poll.add_fd(fd_upload, POLLOUT);
			while (1)
			{
				std::cout<<"AH"<<std::endl;
				write_poll.exec();
				std::cout<<"Mince"<<std::endl;
				it = write_poll.begin();
				if ((it->revents & POLLOUT) == POLLOUT)
				{
					ret = write(fd_upload, &req.getBody()[bytes_write], req.getBody().size() - bytes_write);
					if (ret < 0)
					{
						std::cout<<"Error cgi write"<<std::endl;
						exit(-1);
					}
					bytes_write += ret - 1;
					if (ret == 0 || bytes_write == req.getBody().length() - 1)
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

	int	Methods::deleteMethod(const http_request_type &req, http_response_type &response, http_route_type& route)
	{
		if (isCGI(req, route) == 2)
			return 2;
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
			//std::cout<<"ext: "<<ext<<std::endl;
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
}	// namespace Methods
}   // namespace Webserv