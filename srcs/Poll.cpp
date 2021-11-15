/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkevin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 16:46:20 by pkevin            #+#    #+#             */
/*   Updated: 2021/11/15 16:46:25 by pkevin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Pollhpp

Poll::Poll(void)
{
    this->poll_fds = NULL;
    this->n_fds = 0;
    this->call_functions = NULL;
}

Poll::Poll(std::vector<Socket> sockets_servers, std::map<std::string, std::vector<> >)
{
    this->init(sockets_server, functions);
}

Poll::~Poll(void)
{
    if (poll_fds)
        delete [] poll_fds;
}

void Poll::init(std::vector<Socket> sockets_servers, std::map<std::string, std::vector<> >)
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

    //TO DO change with new stop condition
    while (42)
    {
        ret = poll(poll_fds, n_fds);
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
					if (fcntl(c_socket, F_SETFL, fcntl(c_socket, F_GETFL, 0) | O_NONBLOCK) < 0)
					{
						perror("fnctl");
					}
                    /* End TO DO */
                    /* Add client_socket in poll_fds */
					poll_fds[nb_fds].fd = client_socket;
					poll_fds[nb_fds].events = POLLIN;
					nb_fds++;
				}
				else if (poll_fds[i].revents & POLLIN)
				{
					/* Read client socket - call functions*/
                    j = 0;
                    while (call_functions["read"][j])
                    {
                        call_functions["read"][j](poll_fds[i].fd;
                        j++;
                    }
					/* Change events socket_client to write*/
					poll_fds[i].events = POLLOUT;
				}
				else if (poll_fds[i].revents & POLLOUT)
				{
					/* Send response*/
                    j = 0;
                    while (call_functions["write"][j])
                    {
                        call_functions["write"][j](poll_fds[i].fd);
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