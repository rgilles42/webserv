/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgilles <rgilles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 16:51:02 by rgilles           #+#    #+#             */
/*   Updated: 2021/12/13 11:01:54 by rgilles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/Socket.hpp"

Socket::Socket() : _fd(-1), _len(sizeof(struct sockaddr)) {}

Socket::Socket(const char* addr, unsigned short port, int blocking) : _fd(socket(AF_INET, SOCK_STREAM, 0)), _len(sizeof(struct sockaddr))
{
	int					optval = 1;
	struct sockaddr_in&	address = reinterpret_cast<struct sockaddr_in&>(this->_addr);

	if (this->_fd < 0)
		throw SocketNotCreatedException();
	if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
		throw SocketSetOptFailedException();
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(addr);
	address.sin_port = htons(port);
	if (!blocking)
		if (fcntl(this->_fd, F_SETFL, O_NONBLOCK) != 0)
			throw SetNonBlockFailedException();

	this->_address.fromString(addr);
	this->_address.setPort(port);
}

Socket::Socket(const Socket& src) {*this = src;}

Socket&	Socket::operator=(const Socket& src)
{
	char*		local;
	uintptr_t	temp;
	char*		source;

	if (this != &src)
	{
		this->_fd = src._fd;
		this->_len = src._len;
		temp = reinterpret_cast<uintptr_t>(&this->_addr);
		local = reinterpret_cast<char*>(temp);
		temp = reinterpret_cast<uintptr_t>(&src._addr);
		source = reinterpret_cast<char*>(temp);
		_address = src._address;
		for (socklen_t i = 0; i < this->_len; i++)
			local[i] = source[i];
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

Socket	Socket::accept(int blocking)
{
	Socket		newSocket;

	newSocket._fd = ::accept(this->_fd, &newSocket._addr, &newSocket._len);
	if (newSocket._fd < 0)
		throw AcceptFailedException();
	if (!blocking)
		if (fcntl(newSocket._fd, F_SETFL, O_NONBLOCK) != 0)
			throw SetNonBlockFailedException();
	return (newSocket);
}

ssize_t	Socket::read(void* buf, size_t count)
{
	int ret;
	ret = ::read(this->_fd, buf, count);
	return (ret);
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

struct sockaddr& Socket::addr(void)
{
	return(this->_addr);
}

const Socket::address_type Socket::address(void) const {
	return this->_address;
}