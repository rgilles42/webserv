/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgilles <rgilles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 16:46:20 by pkevin            #+#    #+#             */
/*   Updated: 2022/02/08 19:01:42 by rgilles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/events/Poll.hpp"

namespace Webserv
{

Poll::Poll(void)
{
	this->nb_fds = 0;
}

Poll::Poll(const Poll& other) {
	*this = other;
}

Poll::~Poll(void) {}

Poll	&Poll::operator = (Poll const &other)
{
	if (this != &other) {
		this->vect_pollfd = other.vect_pollfd;
		this->nb_fds = other.nb_fds;
	}
	return *this;
}

void    Poll::add_fd(int fd, short events)
{
	struct pollfd new_elem;

	new_elem.fd = fd;
	new_elem.events = events;
	new_elem.revents = 0;
	this->vect_pollfd.push_back(new_elem);
	this->nb_fds++;
}

void	Poll::remove_fd(int fd)
{
	std::vector<struct pollfd>::iterator it = this->vect_pollfd.begin();
	std::vector<struct pollfd>::iterator ite = this->vect_pollfd.end();
	while (it != ite && it->fd != fd)
		it++;
	if (it == ite)
		throw pollFailed();
	this->vect_pollfd.erase(it);
	this->nb_fds--;
}
void	Poll::modif_event(int fd, short new_event)
{
	struct pollfd new_elem;

	new_elem.fd = fd;
	new_elem.events = new_event;
	remove_fd(fd);
	this->vect_pollfd.push_back(new_elem);
}

void Poll::init(std::vector<socket_type> sockets_servers)
{
	size_t	size = sockets_servers.size();

	for (size_t i = 0; i < size; i++)
		add_fd(sockets_servers[i].getFd(), POLLIN);
}

void	Poll::exec(void)
{
	struct pollfd *poll_fds;
	int ret = 0;

	poll_fds = this->vect_pollfd.data();
	ret = poll(poll_fds, this->vect_pollfd.size(), -1);
	if (ret < 0)
		throw pollFailed();
	std::cerr<<"poll ret: "<<ret<<std::endl;
}

std::vector<struct pollfd>::iterator	Poll::begin()
{
	return this->vect_pollfd.begin();
}

std::vector<struct pollfd>::iterator	Poll::end()
{
	return this->vect_pollfd.end();
}

void	Poll::clear(void)
{
	this->vect_pollfd.clear();
}

}
