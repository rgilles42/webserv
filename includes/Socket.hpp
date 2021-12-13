/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgilles <rgilles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 16:50:57 by rgilles           #+#    #+#             */
/*   Updated: 2021/11/22 19:22:06 by rgilles          ###   ########.fr       */
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

class Socket {
public:
	Socket(void);
	Socket(const char* addr, unsigned short port);
	Socket(const Socket& src);
	Socket& operator=(const Socket& src);
	~Socket();

	int					bind(void);
	int					listen(void);
	Socket				accept(void);
	ssize_t				read(void *buf, size_t count);
	ssize_t				write(const void *buf, size_t count);
	int					close(void);
	int&				fd(void);
	struct sockaddr&	addr(void);

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
	struct sockaddr		_addr;
	socklen_t			_len;
};

#endif
