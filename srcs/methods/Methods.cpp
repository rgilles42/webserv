# include "../../includes/methods/Methods.hpp"

namespace Webserv {
namespace Methods {
	Methods::Methods(void) {}

	Methods::~Methods(void) {}

	void    Methods::exec_method(Webserv::Http::HttpRequest req, Webserv::Resource *rcs/*, Webserv::Http::Server srv*/)
	{
		if (req.getMethod() == "GET")
			getMethod(req, rcs);
		else if (req.getMethod() == "POST")
			postMethod(req, rcs);
		else if (req.getMethod == "DELETE")
			deleteMethod(req, rcs);
		else
			throw methodsBadName();
	}
/*--------------------------------------------------------------------------------------------------------------*/
	void    Methods::getMethod(Webserv::Http::HttpRequest req, Webserv::Resource *rcs/*, Webserv::Http::Server srv*/)
	{
		if (rcs.isCGI())
		{
			int         fd_pipe[2];
			if (pipe(fd_pipe) < 0)
			{
				std::cout<<"Error pipe"<<std::endl;
				return; //need change by exceptions
			}
			CGIEvent    *new_cgi( = new CGI(rcs, req, fd_pipe/*, *srv*/);
			Webserv::Core::getInstance().add_cgi_event(fd_pipe[0], *new_cgi);
			return;
		}
		if (rcs.isDir())
		{
			std::cout<<"Error 403"<<std::endl;
			return;	//change throw
		}

	}

	void    Methods::postMethod(Webserv::Http::HttpRequest req, Webserv::Resource *rcs)
	{
		if (rcs.isCGI())
		{
			int         fd_pipe[2];
			if (pipe(fd_pipe) < 0)
			{
				std::cout<<"Error pipe"<<std::endl;
				return; //need change by exceptions
			}
			CGIEvent    *new_cgi( = new CGI(rcs, req, fd_pipe/*, *srv*/);
			Webserv::Core::getInstance().add_cgi_event(fd_pipe[0], *new_cgi);
			return;
		}
		if (rcs.isDir())
		{
			std::cout<<"Error 403"<<std::endl;
			return;	//change throw
		}
	}

	void    Methods::deleteMethod(Webserv::Http::HttpRequest req, Webserv::Resource *rcs)
	{
		if (rcs.isCGI())
		{
			int         fd_pipe[2];
			if (pipe(fd_pipe) < 0)
			{
				std::cout<<"Error pipe"<<std::endl;
				return; //need change by exceptions
			}
			CGIEvent    *new_cgi( = new CGI(rcs, req, fd_pipe/*, *srv*/);
			Webserv::Core::getInstance().add_cgi_event(fd_pipe[0], *new_cgi);
			return;
		}
		if (rcs.isDir())
		{
			std::cout<<"Error 403"<<std::endl;
			return;	//change throw
		}
	}
}	// namespace Methods
}   // namespace Webserv