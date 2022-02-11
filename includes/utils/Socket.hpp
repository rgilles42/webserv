/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 16:50:57 by rgilles           #+#    #+#             */
/*   Updated: 2022/01/30 02:38:28 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <sys/socket.h>	// For socket, sockaddr, etc..
# include <exception>		// For std::exception
# include <netinet/in.h>	// For sockaddr_in
# include <arpa/inet.h>		// For inet_addr
# include <cstring>			// For memcpy
# include <unistd.h>		// For read, write, close
# include <fcntl.h>			// For fcntl, etc..

# include "Address.hpp"		// For Address

namespace Webserv {

	namespace Utils {

		class Socket {
			public:
				typedef int							fd_type;
				typedef sockaddr					sockaddr_type;
				typedef sockaddr_in					sockaddr_in_type;
				typedef socklen_t					socklen_type;
				typedef Webserv::Utils::Address		address_type;

				typedef sa_family_t					sa_family_type;
				typedef std::string					sa_addr_type;
				typedef address_type::port_type		sa_port_type;

				class SocketException : public std::exception {
					protected:
						std::string	msg;

					public:
						SocketException(const std::string& msg = "") : std::exception() {
							this->msg = msg;
						}
						virtual ~SocketException() throw() {}
						virtual const char	*what() const throw() {
							return this->msg.c_str();
						}
				};

				struct SocketNotCreatedException : public SocketException {
					SocketNotCreatedException(void) : SocketException("Unable to create socket") {}
				};
				struct SetSockOptFailedException : public SocketException {
					SetSockOptFailedException(void) : SocketException("Unable to set socket options") {}
				};
				struct FcntlFailedException : public SocketException {
					FcntlFailedException(void) : SocketException("Unable to use cmd on socket") {}
				};
				struct ConnectFailedException : public SocketException {
					ConnectFailedException(void) : SocketException("Unable to connect") {}
				};
				struct BindFailedException : public SocketException {
					BindFailedException(void) : SocketException("Unable to bind") {}
				};
				struct ListenFailedException : public SocketException {
					ListenFailedException(void) : SocketException("Unable to listen") {}
				};
				struct AcceptFailedException : public SocketException {
					AcceptFailedException(void) : SocketException("Unable to accept") {}
				};

			protected:
				fd_type			_fd;
				sockaddr_type	_sockaddr;
				socklen_type	_socklen;
				address_type	_address;

			public:
				Socket(void);
				Socket(const sa_addr_type& addr, const sa_port_type& port, int blocking = 0);
				Socket(const Socket& other);
				~Socket();

				Socket&		operator=(const Socket& other);

				void		setFamily(const sa_family_type& family);
				void		setAddress(const sa_addr_type& addr);
				void		setPort(const sa_port_type& port);

				int			setsockopt(int level, int optname);
				int			fcntl(int cmd);
				int			fcntl(int cmd, long arg);
				int			fcntl(int cmd, struct flock *lock);

				int			connect(void);

				int			bind(void);
				int			listen(void);
				Socket		accept(int blocking = 0);

				ssize_t		read(void *buf, size_t count);
				ssize_t		write(const void *buf, size_t count);

				ssize_t		recv(void *buf, size_t count, int flags = 0);
				ssize_t		send(const void *buf, size_t count, int flags = 0);

				int			close(void);

				const fd_type&			getFd(void) const;
				const sockaddr_type&	getSockAddr(void) const;
				const sockaddr_in_type&	getSockAddrIn(void) const;
				const socklen_type&		getSockLen(void) const;
				const address_type&		getAddress(void) const;
		};

	} // namespace Utils

} // namespace Webserv

#endif