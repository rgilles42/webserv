/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 16:46:20 by pkevin            #+#    #+#             */
/*   Updated: 2022/02/11 16:11:30 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/events/Poll.hpp"

namespace Webserv {

	Poll::Poll(void) {}

	Poll::Poll(const Poll& other) {
		*this = other;
	}

	Poll::~Poll(void) {}

	Poll	&Poll::operator=(Poll const &other)
	{
		if (this != &other) {
			this->allPollFD = other.allPollFD;
			this->usedPollFD = other.usedPollFD;
		}
		return *this;
	}

	void	Poll::add_fd(int fd, short events)
	{
		pollfd_type new_elem;

		new_elem.fd = fd;
		new_elem.events = events;
		new_elem.revents = 0;
		this->allPollFD.push_back(new_elem);
	}

	void	Poll::remove_fd(int fd)
	{
		poll_fd_vector::iterator it = this->allPollFD.begin();

		while (it != this->allPollFD.end() && it->fd != fd)
			it++;
		if (it == this->allPollFD.end() && errno != EINTR)
			throw pollFailed();
		this->allPollFD.erase(it);
	}
	void	Poll::modif_event(int fd, short new_event)
	{
		pollfd_type new_elem;

		new_elem.fd = fd;
		new_elem.events = new_event;
		remove_fd(fd);
		this->allPollFD.push_back(new_elem);
	}

	void	Poll::init(std::vector<socket_type> sockets_servers)
	{
		size_t	size = sockets_servers.size();

		for (size_t i = 0; i < size; i++)
			add_fd(sockets_servers[i].getFd(), POLLIN);
	}

	int		Poll::exec(void)
	{
		pollfd_type *poll_fds;
		int ret = 0;

		errno = 0;
		this->usedPollFD.clear();
		poll_fds = this->allPollFD.data();
		ret = poll(poll_fds, this->allPollFD.size(), -1);
		if (ret < 0 && errno != EINTR)
			throw pollFailed();
		poll_fd_vector::const_iterator it = this->allPollFD.begin();
		while (it != this->allPollFD.end()) {
			if (it->revents != 0) {
				this->usedPollFD.push_back(*it);
			}
			it++;
		}
		return (ret);
	}

	const Poll::poll_fd_vector&		Poll::getPollAllFD(void) const {
		return this->allPollFD;
	}

	const Poll::poll_fd_vector&		Poll::getPollUsedFD(void) const {
		return this->usedPollFD;
	}

	void	Poll::clear(void)
	{
		this->allPollFD.clear();
		this->usedPollFD.clear();
	}

}	// namespace Webserv
