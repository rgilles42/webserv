/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 16:35:23 by pkevin            #+#    #+#             */
/*   Updated: 2022/02/11 16:10:38 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POLL_HPP
# define POLL_HPP

#include <iostream>
#include <map>
#include <vector>
#include <string.h>
#include <poll.h>
#include <unistd.h>
#include <vector>
#include <errno.h>

#include "../utils/Socket.hpp"

namespace Webserv
{
	class Poll {
		public:
			typedef Webserv::Utils::Socket		socket_type;
			typedef pollfd						pollfd_type;
			typedef std::vector<pollfd_type>	poll_fd_vector;
			typedef poll_fd_vector::iterator	iterator;

		private:
			poll_fd_vector	allPollFD;
			poll_fd_vector	usedPollFD;

		public:
			/* Constructors */
			Poll(void);
			Poll(const Poll& other);
			~Poll(void);

			Poll	&operator=(Poll const &other);

			void	add_fd(int fd, short events);
			void	remove_fd(int fd);
			void	modif_event(int fd, short new_event);

			void	init(std::vector<socket_type> sockets_servers);
			int		exec(void);

			void	clear(void);

			const poll_fd_vector&	getPollAllFD(void) const;
			const poll_fd_vector&	getPollUsedFD(void) const;

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