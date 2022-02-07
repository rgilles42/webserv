/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgilles <rgilles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 16:35:23 by pkevin            #+#    #+#             */
/*   Updated: 2022/02/07 14:18:53 by rgilles          ###   ########.fr       */
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

#include "../utils/Socket.hpp"

namespace Webserv
{
	class Poll {
		public:
			typedef Webserv::Utils::Socket					socket_type;
			typedef std::vector<struct pollfd>::iterator	iterator;

		private:
			std::vector<struct pollfd>		vect_pollfd;
			size_t							nb_fds;

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
			void	exec(void);

			void	clear(void);

			std::vector<struct pollfd>::iterator begin(void);
			std::vector<struct pollfd>::iterator end(void);


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