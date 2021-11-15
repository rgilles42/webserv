/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkevin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 16:35:23 by pkevin            #+#    #+#             */
/*   Updated: 2021/11/15 16:36:05 by pkevin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POLL_HPP
# define POLL_HPP

#include <iostream>
#include <map>
#include <vector>
#include <ctring>
#include <pollh

using namespace webserv
{
	class Poll
	{
		private:
			struct pollfd					poll_fds;
			size_t							n_fds;
			size_t							n_srv;
			std::map<std::string, std::vector<>>	call_functions

			bool	is_socket_server(int socket);
		public:
			Poll(void);
			Poll(std::vector<Socket> sockets_servers, std::map<std::string, std::vector<> >)
			~Poll(void);

			void	init(std::vector<Socket> sockets_servers, std::map<std::string, std::vector<> >)
			int		start(void);
	};
}

#endif
