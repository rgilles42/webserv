#include "../../includes/events/ClientEvent.hpp"

namespace Webserv
{
	ClientEvent::ClientEvent(Socket &new_sock/*, Webserv::Http::Server &srv*/): sock(new_sock)/*, m_srv(srv)*/
	{
		this->events_flags = POLLIN;
	}

	ClientEvent::~ClientEvent(void)
	{
		if (this->sock.fd())
			close(sock.fd());
		delete this->rcs;
	}

	void	ClientEvent::read_event(void)	//TO DO replace by ConstructRequest and add Methods
	{
		std::cout<<"Client read_event"<<std::endl;
		char buffer[2048];
		size_t	size;

		size = this->sock.read(buffer, 2048);
		buffer[size] = '\0';
		request_string += buffer;
		this->create_req.addMessage(buffer);
		if (this->create_req.parseRequests() == true)
		{
//			this->req = this->create_req.getAllRequests();
			this->rcs = new Resource("./default_pages/index.html");
			this->events_flags = POLLOUT;
		}
		else
		{
			std::cout<<"|START|"<<request_string<<"|END|"<<std::endl;
		}
	}

	void	ClientEvent::write_event(void)	//TO DO replace
	{
		std::cout<<"Client write event"<<std::endl;
//		Resource currentResource("./default_pages/index.html");
		if (this->rcs->loadResource())
		{
			Webserv::Http::HttpResponse response(*this->rcs);
			this->sock.write(response.toString().c_str(), response.toString().length());
			this->events_flags = POLLIN;
		}

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