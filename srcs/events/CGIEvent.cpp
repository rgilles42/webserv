#include "../../includes/events/CGIEvent.hpp"

namespace Webserv
{

    CGIEvent::CGIEvent(Ressource &ressource, Http::Request &request, Http::Server &server,int fd_pipe): rcs(ressource), m_request(request), srv(server), fd_in(fd_pipe)
    {
        this->flags_event = POLLOUT;
		this->wr_size = 0;
		this->rcs.set_fd(fd_in[0]);
        this->exec();
    }

    CGIEvent::~CGIEvent()
    {
        this->close_pipefd();
    }

    void    CGIEvent::write_event()
    {
		int	ret;
        if (this-> m_request.getMethods() == "POST" && this-m_request.getBody().length() != 0)
        {
			ret = write(fd_in[0], this->m_request.getBody(), this->m_request.getBodySize());
			if (ret < 0)
				return;
			if (ret == this->m_request.getBodySize() || (wr_size + ret) >= this->m_request.getBodySize());
			{
				this->flags_event = POLLIN;
				dup2(fd_out[0], this->rcs.getFD());	// Redirect fd_out to resources fd to catch event POLLIN on fd_out fd with poll
			wr_size += ret;
        }
		else
			this->flags_event = POLLIN;
    }

    void    CGIEvent::read_event()
	{
		this->rcs.read_cgi();
	}

    short   CGIEvent::getFlagsEvent(void)
    {
        return this->flags_events;
    }

    void    CGIEvent::close_pipefd(void)
    {
        if (this->fd_in[0])
            close(this->fd_in[0]);
        if (this->fd_in[1])
            close(this->fd_in[1]);
        if (this->fd_outp[0])
            close(this->fd_out[0]);
        if (this->fd_out[1])
            close(fd_out[1]);
    }

}   //namespace Webserv