/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkevin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 16:35:23 by pkevin            #+#    #+#             */
/*   Updated: 2021/11/29 11:37:41 by pkevin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POLL_HPP
# define POLL_HPP

#include <iostream>
#include <map>
#include <vector>
#include <ctring>
#include <poll.h>
#include <unistd.h>
#include <fnctl.h>
#include "FD_TYPES.hpp"

namespace webserv
{
	class Poll
	{
		public:
			enum eventTypes
			{
				onConnection,
				onReadyRead,
				onReadyWrite
			};

			enum fd_types
			{
				Socket,
				CGI
			}

		private:
			struct pollfd					poll_fds;
			size_t							nb_fds;
			size_t							nb_srv;
			std::map<Poll::eventTypes, std::vector<void (*)(int)> >	events;
			std::map<int, Poll::fd_types>	fds;
			bool							is_enable;

			bool	is_socket_server(int socket);

		public:
			/* Constructors */
			Poll(void);
			Poll(std::vector<Socket> sockets_servers)	//TO DO - def
			Poll(std::vector<Socket> sockets_servers);	//TO DO - def
			~Poll(void);

			void 	registerEvent(Poll::eventTypes type, void (*ft)(int));
			void	add_fd(int fd, short events);

			void	init(std::vector<Socket> sockets_servers)	//def
			int		exec(void);

			struct  pollFailed : public std::exception
            {
                virtual const char* what() const throw()
                {
                    return ("Poll: failed execution");
                }
            };

	};
}

#endif
