# include "../../includes/methods/Methods.hpp"

namespace Webserv {
namespace Methods {
	Methods::Methods(void) {}

	Methods::~Methods(void) {}

	int    Methods::exec_method(http_request_type req, http_response_type &response)
	{
		if (req.getMethod().getMethod() == http_request_type::method_type::GET)
			return getMethod(req, response);
		else if (req.getMethod().getMethod() == http_request_type::method_type::POST)
			return postMethod(req, response);
		else if (req.getMethod().getMethod() == http_request_type::method_type::DELETE)
			return deleteMethod(req, response);
		return -1;
	}
/*--------------------------------------------------------------------------------------------------------------*/
	int   Methods::getMethod(http_request_type req, http_response_type &response)
	{
		(void)req;
		(void)response;
		return 0;
	}

	int    Methods::postMethod(http_request_type req, http_response_type &response)	// TO DO Modif for check if need CGI
	{
		Poll	write_poll;
		int	fd_upload = -1;;
		size_t	bytes_write = 0;
		std::vector<struct pollfd>::iterator it;
		ssize_t ret;

		std::cout<<"POST Method call"<<std::endl;
		std::cout<<"reqBody: "<<req.getBody()<<std::endl;
		if (req.getBody().size() != 0)
		{
			std::cout<<"Create file"<<std::endl;
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

	int   Methods::deleteMethod(http_request_type req, http_response_type &response)
	{
		(void)req;
		(void)response;
		return 0;
	}
}	// namespace Methods
}   // namespace Webserv