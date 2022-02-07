/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 16:51:02 by rgilles           #+#    #+#             */
/*   Updated: 2022/01/30 02:36:40 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../includes/utils/Socket.hpp"

namespace Webserv {

	namespace Utils {

		Socket::Socket() : _fd(-1), _socklen(sizeof(sockaddr_type)) {}

		Socket::Socket(const sa_addr_type& addr, const sa_port_type& port, int blocking) : _fd(-1), _socklen(sizeof(sockaddr_type)) {

			this->_fd = ::socket(AF_INET, SOCK_STREAM, 0);

			if (this->_fd < 0)
				throw SocketNotCreatedException();

			this->setFamily(AF_INET);
			this->setAddress(addr);
			this->setPort(port);

//			this->setsockopt(SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT);

			if (!blocking) {
				this->fcntl(F_SETFL, O_NONBLOCK);
			}
		}

		Socket::Socket(const Socket& other) {
			*this = other;
		}

		Socket::~Socket() {}

		Socket&	Socket::operator=(const Socket& other) {
			if (this != &other)
			{
				this->_fd = other._fd;
				this->_socklen = other._socklen;
				std::memcpy(&this->_sockaddr, &other._sockaddr, other._socklen);
				this->_address = other._address;
			}
			return (*this);
		}

		void	Socket::setFamily(const sa_family_type& family) {
			sockaddr_in_type& sockAddrIn = reinterpret_cast<sockaddr_in_type&>(this->_sockaddr);

			sockAddrIn.sin_family = family;
		}

		void	Socket::setAddress(const sa_addr_type& addr) {
			sockaddr_in_type& sockAddrIn = reinterpret_cast<sockaddr_in_type&>(this->_sockaddr);

			sockAddrIn.sin_addr.s_addr = inet_addr(addr.c_str());
			this->_address.fromString(addr);
		}

		void	Socket::setPort(const sa_port_type& port) {
			sockaddr_in_type& sockAddrIn = reinterpret_cast<sockaddr_in_type&>(this->_sockaddr);

			sockAddrIn.sin_port = htons(port);
			this->_address.setPort(port);
		}

		int	Socket::setsockopt(int level, int optname) {
			int	optval = 1;
			int	setsockoptret = ::setsockopt(this->_fd, level, optname, &optval, sizeof(optval));
			if (setsockoptret)
				throw Socket::SetSockOptFailedException();
			return (setsockoptret);
		}

		int	Socket::fcntl(int cmd) {
			int fcntlret = ::fcntl(this->_fd, cmd);
			if (fcntlret)
				throw Socket::FcntlFailedException();
			return (fcntlret);
		}

		int	Socket::fcntl(int cmd, long arg) {
			int fcntlret = ::fcntl(this->_fd, cmd, arg);
			if (fcntlret)
				throw Socket::FcntlFailedException();
			return (fcntlret);
		}

		int	Socket::fcntl(int cmd, struct flock *lock) {
			int fcntlret = ::fcntl(this->_fd, cmd, lock);
			if (fcntlret)
				throw Socket::FcntlFailedException();
			return (fcntlret);
		}

		int	Socket::connect() {
			int	connectret = ::connect(this->_fd, &this->_sockaddr, this->_socklen);
			if (connectret)
				throw Socket::ConnectFailedException();
			return (connectret);
		}

		int	Socket::bind() {
			int	bindret = ::bind(this->_fd, &this->_sockaddr, this->_socklen);
			if (bindret)
				throw Socket::BindFailedException();
			return (bindret);
		}

		int	Socket::listen() {
			int listenret = ::listen(this->_fd, 50);
			if (listenret)
				throw Socket::ListenFailedException();
			return (listenret);
		}

		Socket	Socket::accept(int blocking) {
			Socket		newSocket;

			newSocket._fd = ::accept(this->_fd, &newSocket._sockaddr, &newSocket._socklen);
			if (newSocket._fd < 0)
				throw AcceptFailedException();
			if (!blocking) {
				newSocket.fcntl(F_SETFL, O_NONBLOCK);
			}
			return (newSocket);
		}

		ssize_t	Socket::read(void* buf, size_t count) {
			return (::read(this->_fd, buf, count));
		}

		ssize_t	Socket::write(const void* buf, size_t count) {
			return (::write(this->_fd, buf, count));
		}

		ssize_t	Socket::recv(void* buf, size_t count, int flags) {
			return (::recv(this->_fd, buf, count, flags));
		}

		ssize_t	Socket::send(const void* buf, size_t count, int flags) {
			return (::send(this->_fd, buf, count, flags));
		}

		int		Socket::close(void) {
			return (::close(this->_fd));
		}

		const Socket::fd_type&			Socket::getFd(void) const {
			return (this->_fd);
		}

		const Socket::sockaddr_type&	Socket::getSockAddr(void) const {
			return (this->_sockaddr);
		}

		const Socket::sockaddr_in_type&	Socket::getSockAddrIn(void) const {
			return (reinterpret_cast<const sockaddr_in_type&>(this->_sockaddr));
		}

		const Socket::socklen_type&		Socket::getSockLen(void) const {
			return (this->_socklen);
		}

		const Socket::address_type&		Socket::getAddress(void) const {
			return (this->_address);
		}

	} // namespace Utils

} // namespace Webserv