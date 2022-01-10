#include "../../includes/events/ClientEvent.hpp"

namespace Webserv
{
	ClientEvent::ClientEvent(Socket &new_sock): sock(new_sock)
	{
		this->events_flags = POLLIN;
	}
/*	ClientEvent::ClientEvent(Http::Client &ref): c(ref)
	{
		this->events_flags = POLLIN;
	}*/

	ClientEvent::~ClientEvent(void)
	{
		if (this->sock.fd())
			close(sock.fd());
	}

	void	ClientEvent::read_event(void)	//TO DO replace by ConstructRequest and add Methods
	{
		std::cout<<"Client read_event"<<std::endl;
		char buffer[2048];
		size_t	size;

		size = this->sock.read(buffer, 2048);
		// if (size < 0)
		// 	throw clientEventReadFailed();
		buffer[size] = '\0';
		request_string += buffer;
		this->events_flags = POLLOUT;
	}

	void	ClientEvent::write_event(void)	//TO DO replace
	{
//		if (this->rcs.complete())
//		{
				std::cout<<"Client write event"<<std::endl;
                // Resource currentResource("./default_pages/index.html");
                // currentResource.setContent(getFileContents(currentResource.getUri()));
                // currentResource.setContentType(Webserv::Utils::getContentTypeByFile(currentResource.getUri(), "text/plain"));
                // Webserv::Http::HttpResponse response(currentResource);
                Webserv::Http::HttpResponse response;

				this->sock.write(response.toString().c_str(), response.toString().length());
//				::write(this->sock.fd(), response.toString().c_str(), response.toString().length());
				this->events_flags = POLLIN;
//		}
	}

	short	ClientEvent::getEventsFlags(void)
	{
		return this->events_flags;
	}

	int		ClientEvent::getFD(void)
	{
		return this->sock.fd();	//TO DO replace
	}

}	// namespace Webserv