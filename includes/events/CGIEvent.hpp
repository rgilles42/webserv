#ifndef CGIEVENT_HPP
# define CGIEVENT_HPP

#include "IEvents.hpp"
#include "../Resource.hpp"
#include "../http/HttpRequest.hpp"
#include "../http/Server.hpp"
#include "../utils/Env.hpp"

namespace Webserv
{

	class CGIEvent : public IEvents
	{
		private:
			Resource			rcs;
			int					fd_in[2];   //use to write request body
			Http::HttpRequest	req;
			Http::Server		srv;
			int					fd_out[2];  //use to redirect cgi output
			short				flags_events;
			Webserv::Utils::Env	env;

			int					wr_size;

			void	close_pipefd(void);
			void	init_env();
			void	exec(void);

		public:
			CGIEvent(Resource &ressource, std::string req_method,int fd_pipe[2]);
			virtual ~CGIEvent();

			void	write_event(void);
			void	read_event();
			short	getEventsFlags(void);
	};

}

#endif
