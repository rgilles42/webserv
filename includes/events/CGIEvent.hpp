#ifndef CGIEVENT_HPP
# define CGIEVENT_HPP

#include "IEvents.hpp"
#include "../Resource.hpp"
#include "../http/HttpRequest.hpp"
#include "../http/Server.hpp"

namespace Webserv
{

    class CGIEvent : public IEvents
    {
        private:
            Resource		&rcs;
            int				fd_in[2];   //use to write request body
            Http::HttpRequest   m_req;
            Http::Server	srv;
            int				fd_out[2];  //use to redirect cgi output
            short			flags_events;
			int				wr_size;

            void        close_pipefd(void);
            void        exec(void);

        public:
            CGIEvent(Resource &ressource, std::string req_method,int fd_pipe[2]);
            virtual ~CGIEvent();

            void    write_event(void);
            void    read_event();
            short   getEventsFlags(void);
    };

}

#endif
