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

void    Poll::add_fd(int fd, short events, Poll::fd_type type)
{
    this->poll_fds[this->nb_fds].fd = fd;
    this->poll_fds[this->nb_fds].events = events;

    this->fds.insert(std::make_pair(fd, types));
    this->nb_fds++;
}

void Poll::init(std::vector<Socket> sockets_servers)
{
    this->nb_fds = sockets_server.size();
    this->nb_srv = nb_fds;
    this->poll_fds = new pollfd[nb_fds * 3];

    for (int i = 0; i < this->n_fds; i++)
    {
/*        poll_fds[i].fd = sockets_servers[i];
        poll_fds[i].events = POLLIN;*/
        add_fd(socket_server[i], POLLIN, Poll::Socket_server);
    }
    for (int i = this->nb_fds; i < thi->nb_fds * 2; i++)
    {
        poll_fds[i].fd = -1;
        poll_fds[i].events = 0;
    }
}

int Poll::exec(void)
{
    int ret = 0;
    int j = 0;

    void (*f) (int);
    this->is_enable = true;

    ret = poll(poll_fds, nb_fds, -);
    if (ret < 0)
        throw pollFailed();
    else
    {
        for (int i = 0; i < nb_fds; i++)
		{
			if (!poll_fds[i].revents)	//if no event detected - continue
				continue;
			if (poll_fds[i].revents & POLLIN && fds[poll_fds[i] == Poll::Socket_server])
			{
                /* TO DO - replace socket fct*/
			    socklen_t client_lenght = sizeof(client_addr);
				SOCKET client_socket = accept(poll_fds[i].fd, (SOCKADDR*)&client_addr, &client_lenght);

				if (fcntl(client_socket, F_SETFL, O_NONBLOCK) < 0)
					perror("fnctl");
                /* End TO DO*/

                this->add_fd(client_socket, POLLIN, Poll::Socket_client);
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
                // if (fds[poll_fds[i].fd] == Poll::Socket)
                j = 0;
                std::vector<void (*)(int)> vect2 = this->events[Poll::onReadConnection];
                while (j < vect2.size())
                {
                    f = vect2[j];
                    f(poll_fds[i].fd);
                    j++;
                }
				poll_fds[i].events = POLLOUT;
                // else
                /*
                j = 0;
                std::vector<void (*)(int)> vect_cgi2 = this->events[Poll::onReadConnectionCGI];
                while (j < vect_cgi2.size())
                {
                    f = vect_cgi2[j];
                    f(poll_fds[i].fd);
                    j++;
                }
                */
			}
			else if (poll_fds[i].revents & POLLOUT)
			{
                // if (fds[poll_fds[i].fd] == Poll::Socket)
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

                //else
                /*
                std::vector<void (*)(int)> vect_cgi3 = this->events[Poll::onReadyWriteCGI];
                while (j < vect_cgi3.size())
                {
                    f = vect_cgi3[j];
                    f(poll_fds[i].fd);
                    j++;
                }
                /* 

                */
//                fds.erase(poll_fds[i].fd); //Can change
				poll_fds[i].fd = -1;
				poll_fds[i].events = 0;
                while (i + 1 <= n_fds)
                {
                    poll_fds[i].fd = poll_fds[i + 1].fd;
                    poll_fds[i].events = poll_fds[i + 1].events;
                    i++;
                }
				nb_fds--;
			}
		}
    }
}

}