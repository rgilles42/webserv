/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 16:51:02 by rgilles           #+#    #+#             */
/*   Updated: 2022/01/29 20:01:40 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Socket.hpp"

Socket::Socket() : _fd(-1), _len(sizeof(struct sockaddr)) {}

Socket::Socket(const char* addr, unsigned short port, int blocking) : _fd(socket(AF_INET, SOCK_STREAM, 0)), _len(sizeof(struct sockaddr))
{
	struct sockaddr_in&	address = reinterpret_cast<struct sockaddr_in&>(this->_addr);

	if (this->_fd < 0)
		throw SocketNotCreatedException();
	this->setsockopt(SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(addr);
	address.sin_port = htons(port);
	if (!blocking) {
		this->fcntl(F_SETFL, O_NONBLOCK);
	}
	this->_address.fromString(addr);
	this->_address.setPort(port);
}

Socket::Socket(const Socket& src) {*this = src;}

Socket&	Socket::operator=(const Socket& src)
{
	if (this != &src)
	{
		this->_fd = src._fd;
		this->_len = src._len;
		std::memcpy(&this->_addr, &src._addr, this->_len);
		this->_address = src._address;
	}
	return (*this);
}

Socket::~Socket() {}

int	Socket::bind()
{
	int	bindret = ::bind(this->_fd, &this->_addr, sizeof(this->_addr));
	if (bindret)
		throw Socket::BindFailedException();
	return (bindret);
}

int	Socket::listen()
{
	int listenret = ::listen(this->_fd, 50);
	if (listenret)
		throw Socket::ListenFailedException();
	return (listenret);
}

int	Socket::setsockopt(int level, int optname)
{
	int	optval = 1;
	int	setsockoptret = ::setsockopt(this->_fd, level, optname, &optval, sizeof(optval));
	if (setsockoptret)
		throw Socket::SocketSetOptFailedException();
	return (setsockoptret);
}


int	Socket::fcntl(int cmd)
{
	int fcntlret = ::fcntl(this->_fd, cmd);
	if (fcntlret)
		throw Socket::SetNonBlockFailedException();
	return (fcntlret);
}

int	Socket::fcntl(int cmd, long arg)
{
	int fcntlret = ::fcntl(this->_fd, cmd, arg);
	if (fcntlret)
		throw Socket::SetNonBlockFailedException();
	return (fcntlret);
}

int	Socket::fcntl(int cmd, struct flock *lock)
{
	int fcntlret = ::fcntl(this->_fd, cmd, lock);
	if (fcntlret)
		throw Socket::SetNonBlockFailedException();
	return (fcntlret);
}

Socket	Socket::accept(int blocking)
{
	Socket		newSocket;

	newSocket._fd = ::accept(this->_fd, &newSocket._addr, &newSocket._len);
	if (newSocket._fd < 0)
		throw AcceptFailedException();
	if (!blocking) {
		newSocket.fcntl(F_SETFL, O_NONBLOCK);
	}
	return (newSocket);
}

ssize_t	Socket::read(void* buf, size_t count)
{
	return (::read(this->_fd, buf, count));
}

ssize_t	Socket::write(const void* buf, size_t count)
{
	return (::write(this->_fd, buf, count));
}

int	Socket::close(void)
{
	return (::close(this->_fd));
}

int& Socket::fd(void)
{
	return (this->_fd);
}

const sockaddr& Socket::addr(void) const
{
	return(this->_addr);
}

const Socket::address_type Socket::address(void) const {
	return this->_address;
}