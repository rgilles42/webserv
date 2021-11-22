/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkevin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 16:35:23 by pkevin            #+#    #+#             */
/*   Updated: 2021/11/18 09:13:29 by yun              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POLL_HPP
# define POLL_HPP

#include <iostream>
#include <map>
#include <vector>
#include <ctring>
#include <pollh

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
		private:
			struct pollfd					poll_fds;
			size_t							n_fds;
			size_t							n_srv;
			std::map<Poll::eventTypes, std::vector<void (*)(int)> >	events;
			bool							is_enable;


			bool	is_socket_server(int socket);
		public:
			Poll(void);
			Poll(std::vector<Socket> sockets_servers)	//TO DO - def
			Poll(std::vector<Socket> sockets_servers);	//TO DO - def
			~Poll(void);

			void 	registerEvent(Poll::eventTypes type, void (*ft)(int));

			void	init(std::vector<Socket> sockets_servers)	//def
			int		start(void);

	};
}

#endif
