# include "../../includes/methods/Methods.hpp"

namespace Webserv {
    Methods::Methods(void) {}

    Methods::~Methods(void) {}

    void    Methods::exec_method(Webserv::Http::HttpRequest req, Webserv::Resource rcs/*, Webserv::Http::Server srv*/)
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