#include "../../includes/events/ClientEvent.hpp"

namespace Webserv
{
	ClientEvent::ClientEvent(Socket &new_sock/*, Webserv::Http::Server &srv*/): sock(new_sock)/*, m_srv(srv)*/
	{
		this->cgi = NULL;
		this->events_flags = POLLIN;
	}

	ClientEvent::~ClientEvent(void)
	{
		if (this->sock.fd())
			this->sock.close();
		if (this->rcs)
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
		if (this->create_req.checkBuffer() >= 1)
		{
			if (this->create_req.parseRequests() == true)
			{
				this->req = this->create_req.getAllRequests()[0];
				this->rcs = new Resource("./default_pages/index.php", true);
				if (this->rcs->isCGI())
				{
					this->cgi = new CGIEvent(this->create_req.getAllRequests()[0]);
					this->rcs->setFd(this->cgi->getReadFD());
				}
	//			Webserv::Methods::Methods::getInstance().exec_method(this->req, this->rcs/*, this->srv**/);
				this->events_flags = POLLOUT;
			}
		}
	}

	void	ClientEvent::write_event(void)	//TO DO replace
	{
		int status = 0;
		std::cout<<"Client write event"<<std::endl;
		if (this->rcs->isCGI() && !this->cgi->CGIIsEnd())
		{
			if (this->cgi->writeIsEnd())
			{
				status = this->cgi->exec();	//peut etre recup le ret
				perror(strerror(status));
				std::cout<<"status: "<<status<<std::endl;
			}
			else
				this->cgi->write_event();
			return;
		}
		if (this->rcs->loadResource())
		{
			std::cout<<"status: "<<status<<std::endl;
			Webserv::Http::HttpResponse response(*this->rcs);
			this->sock.write(response.toString().c_str(), response.toString().length());
			std::cout<<"delete rcs"<<std::endl;
			if (this->rcs)
				delete this->rcs;
			this->rcs = NULL;
			std::cout<<"delete cgi"<<std::endl;
			if (this->cgi)
				delete this->cgi;
			this->events_flags = POLLIN;
		}
		else
		{
			std::cout<<"hmm"<<std::endl;
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