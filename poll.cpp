/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   poll.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yun <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/14 14:57:07 by yun               #+#    #+#             */
/*   Updated: 2021/11/14 15:18:21 by yun              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/websev.hpp"

volatile sig_atomic_t stop;

void handleSignals(sig_atomic_t signum) {
	(void)signum;
	std::cout << signum << std::endl;
	stop = 1;
}

int	is_socket_srv(int fd, std::vector<SOCKET> socket_server)
{
	for (int i = 0; i < socket_server.size())
	{
		if (fd == socket_server[i])
			return 1;
	}
	return 0;
}

int	polling_socket(std::vector<SOCKET> socket_srv/*, std::vector<Server> server*/)
{
	struct pollfd *poll_fds = new pollfd[srv.size() * 2];
	int	nb_fds = srv.size();
	int	flag;

	/* Init pollfd */

	for (int i = 0; i < socket_srv.size())
	{
		poll_fds[i].fd=  socket_srv[i];
		poll_fds.events = POLLIN;
	}

	for (int i = nb_fds; i <  socket_srv.size() * 2; i++)
	{
		poll_fds[i] = -1;
		poll_fds.events = 0;
	}


	/* Loop */

	signal(SIGINT, handleSignals)
	while (!stop)
	{
		flag = poll(pollfd, nb_fds, -1);	//Wait events  with infinite time (-1)
		if (flag < 0)
		{
			perror("poll");
		}
		else
		{
			for (int i = 0; i < nb_fds; i++)
			{
				if (!poll_fds[i].revents)	//if no event detected - continue
					continue;
				if (poll_fds[i].revents & POLLIN && is_socket_srv(poll_fds[i].fd, socket_server))
				{
					/* add client socket in pollfd */
					socklen_t client_lenght = sizeof(client_addr);
					SOCKET client_socket = accept(poll_fds[i].fd, (SOCKADDR*)&client_addr, &client_lenght);

					if (fcntl(c_socket, F_SETFL, fcntl(c_socket, F_GETFL, 0) | O_NONBLOCK) < 0)	//change non-blocking
					{
						perror("fnctl");
					}
					poll_fds[nb_fds].fd = client_socket;
					poll_fds[nb_fds].events = POLLIN;
					nb_fds++;
				}
				else if (poll_fds[i].revents & POLLIN)
				{
					/* Read client socket */

					/* */
					poll_fds[i].events = POLLOUT;
				}
				else if (poll_fds[i].revents & POLLOUT)
				{
					/* Send response*/

					/* close client socket */
					shutdown(poll_fds[i].fd, 2);
					closesocket(poll_fds[i].fd);
					poll_fds[i].fd = -1;
					poll_fds[i].events = 0;
					nb_fds--;
				}
			}
		}
	}

}
