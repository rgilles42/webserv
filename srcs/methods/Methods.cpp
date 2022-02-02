# include "../../includes/methods/Methods.hpp"

namespace Webserv {
namespace Methods {
	Methods::Methods(void) {}

	Methods::~Methods(void) {}

	int    Methods::exec_method(http_request_type req, resource_type* rcs/*, Webserv::Http::Server srv*/)
	{
		if (req.getMethod().getMethod() == http_request_type::method_type::GET)
			return getMethod(req, rcs);
		else if (req.getMethod().getMethod() == http_request_type::method_type::POST)
			return postMethod(req, rcs);
		else if (req.getMethod().getMethod() == http_request_type::method_type::DELETE)
			return deleteMethod(req, rcs);
		return -1;
	}
/*--------------------------------------------------------------------------------------------------------------*/
	int   Methods::getMethod(http_request_type req, resource_type* rcs/*, Webserv::Http::Server srv*/)
	{
		(void)req;
		if (rcs->isCGI())
		{
			return 0;
		}
		if (rcs->isDir())
		{
			std::cout<<"Error 403"<<std::endl;
			return -2;	//change throw
		}
		return 0;
	}

	int    Methods::postMethod(http_request_type req, resource_type* rcs)
	{
		(void)req;
		if (rcs->isCGI())
		{
			return 0;
		}
		if (rcs->isDir())
		{
			std::cout<<"Error 403"<<std::endl;
			return -2;	//change throw
		}
		return 0;
	}

	int   Methods::deleteMethod(http_request_type req, resource_type* rcs)
	{
		(void)req;
		//int ret;

		if (rcs->isCGI())
		{
			return 0;
		}
		if (rcs->isDir())
		{
			std::cout<<"Error 403"<<std::endl;
			return -2;	//change throw
		}
//		ret = remove(/*path*/);	// Delete file
		return 0;
	}
}	// namespace Methods
}   // namespace Webserv