# include "../../includes/methods/Methods.hpp"

namespace Webserv {
    Methods::Methods(void) {}

    Methods::~Methods(void) {}

    void    Methods::exec_method(std::string method, Webserv::Http::HttpRequest req, Webserv::Resource rcs/*, Webserv::Http::Server srv*/)
    {
        if (method == "GET")
            getMethod(req, rcs);
        else if (method == "POST")
            postMethod(req, rcs);
        else if (method == "DELETE")
            deleteMethod(req, rcs);
        else
            throw methodsBadName();
    }
/*--------------------------------------------------------------------------------------------------------------*/
    void    Methods::getMethod(Webserv::Http::HttpRequest req, Webserv::Resource rcs)
    {

    }

    void    Methods::postMethod(Webserv::Http::HttpRequest req, Webserv::Resource rcs)
    {

    }

    void    Methods::deleteMethod(Webserv::Http::HttpRequest req, Webserv::Resource rcs)
    {
        
    }

}   // namespace Webserv