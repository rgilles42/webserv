/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 16:50:57 by rgilles           #+#    #+#             */
/*   Updated: 2022/01/18 10:49:59 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP
# include <sys/socket.h>
# include <sys/types.h>
# include <exception>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <stdint.h>
# include <stdio.h>
# include <errno.h>
# include <fcntl.h>

# include "./utils/Address.hpp"

class Socket {
public:
	typedef Webserv::Utils::Address address_type;

	Socket(void);
	Socket(const char* addr, unsigned short port, int blocking = 0);
	Socket(const Socket& src);
	Socket& operator=(const Socket& src);
	~Socket();

	int				bind(void);
	int				listen(void);
	Socket			accept(void);
	ssize_t			read(void *buf, size_t count);
	ssize_t			write(const void *buf, size_t count);
	int				close(void);
	int&			fd(void);
	struct sockaddr&	saddr(void);
	const address_type	address(void) const;

	struct SocketNotCreatedException : public std::exception
	{
		virtual const char* what() const throw()
		{
			return("Unable to create socket");
		}
	};
	struct SocketSetOptFailedException : public std::exception
	{
		virtual const char* what() const throw()
		{
			return("Unable to set socket options");
		}
	};
	struct SetNonBlockFailedException : public std::exception
	{
		virtual const char* what() const throw()
		{
			return("Unable to set socket to non-blocking");
		}
	};
	struct BindFailedException : public std::exception
	{
		virtual const char* what() const throw()
		{
			return("Unable to bind");
		}
	};
	struct ListenFailedException : public std::exception
	{
		virtual const char* what() const throw()
		{
			return("Unable to listen");
		}
	};
	struct AcceptFailedException : public std::exception
	{
		virtual const char* what() const throw()
		{
			return("Unable to accept");
		}
	};
private:
	int					_fd;
	struct sockaddr		_saddr;
	socklen_t			_len;
	address_type		_address;

};

#endif
