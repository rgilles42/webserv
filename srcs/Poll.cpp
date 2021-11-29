/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkevin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 16:46:20 by pkevin            #+#    #+#             */
/*   Updated: 2021/11/29 11:38:00 by pkevin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Poll.hpp"

namespace webserv
{

Poll::Poll(void)
{
    this->n_fds = 0;
    this->events.insert(webserv::Poll::eventTypes.onReadConnection, )
}

Poll::Poll(std::vector<Socket> sockets_servers)
{
    this->init(sockets_server, functions);
}

Poll::~Poll(void)
{
    if (poll_fds)
        delete [] poll_fds;
    events.clear();
}

void    Poll::registerEvent(eventType type, void (*f)(int))
{
    events.insert(std::make_pair(type, &f));
}

void Poll::init(std::vector<Socket> sockets_servers)
{
    this->n_fds = sockets_server.size();
    this->n_srv = n_fds;
    this->poll_fds = new pollfd[n_fds * 2];

    for (int i = 0; i < this->n_fds; i++)
    {
        poll_fds[i].fd = sockets_servers[i];
        poll_fds[i].events = POLLIN;
    }
    for (int i = this->n_fds; i < thi->n_fds * 2; i++)
    {
        poll_fds[i].fd = -1;
        poll_fds[i].events = 0;
    }
}

int Poll::start(void)
{
    int ret = 0;
    int j = 0;

    void (*f) (int);
    this->is_enable = true;
    while (is_enable)
    {
        ret = poll(poll_fds, n_fds, -);
        if (ret < 0)
            perror("poll");
        else
        {
            for (int i = 0; i < nb_fds; i++)
			{
				if (!poll_fds[i].revents)	//if no event detected - continue
					continue;
				if (poll_fds[i].revents & POLLIN && is_socket_srv(poll_fds[i].fd)
				{
                    /* TO DO - replace by Socket fct */
					/* Create socket_client*/
					socklen_t client_lenght = sizeof(client_addr);
					SOCKET client_socket = accept(poll_fds[i].fd, (SOCKADDR*)&client_addr, &client_lenght);

                    /* Change non-blocking socket_client*/
					if (fcntl(client_socket, F_SETFL, O_NONBLOCK) < 0)
					{
						perror("fnctl");
					}
                    /* End TO DO */
                    /* Add client_socket in poll_fds */
					poll_fds[nb_fds].fd = client_socket;
					poll_fds[nb_fds].events = POLLIN;
					nb_fds++;
                    j = 0;
                    std::vector<void (*)(int)> vect1 =  = this->events[Poll::onConnection];
                    while (j < vect1.size())
                    {
                        f = vect1[j];
                        f(poll_fds[i].fd);
                        j++;
                    }
				}
				else if (poll_fds[i].revents & POLLIN)
				{
					/* Read client socket - call functions*/
                    j = 0;
                    std::vector<void (*)(int)> vect2 = this->events[Poll::onReadConnection];
                    while (j < vect2.size())
                    {
                        f = vect2[j];
                        f(poll_fds[i].fd);
                        j++;
                    }
					/* Change events socket_client to write*/
					poll_fds[i].events = POLLOUT;
				}
				else if (poll_fds[i].revents & POLLOUT)
				{
					/* Send response*/
                    j = 0;
                    std::vector<void (*)(int)> vect3 = this->events[Poll::onReadyWrite];
                    while (j < vect3.size())
                    {
                        f = vect3[j];
                        f(poll_fds[i].fd);
                        j++;
                    }
                    /* TO DO - replace by Socket fct*/
					shutdown(poll_fds[i].fd, 2);
					closesocket(poll_fds[i].fd);
                    /* End TO DO*/
                    /* Remove client_socket from poll_fds */
					poll_fds[i].fd = -1;
					poll_fds[i].events = 0;
					nb_fds--;
				}
			}
		}
    }
}

bool    Poll::is_socket_server(int socket)
{
    for (size_t i = 0; i < this->n_srv; i++)
        if (poll_fds[i] == socket)
            return (1);
    return (0);
}

}
