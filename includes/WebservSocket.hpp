/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservSocket.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgilles <rgilles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 16:50:57 by rgilles           #+#    #+#             */
/*   Updated: 2021/11/15 18:01:26 by rgilles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVSOCKET_HPP
# define WEBSERVSOCKET_HPP
# include <sys/socket.h>

class WebservSocket {
public:
	WebservSocket(std::string addr = "0.0.0.0", std::string port = "80");
	WebservSocket(const WebservSocket& src);
	WebservSocket& operator=(const WebservSocket& src);
	~WebservSocket();
	struct SocketNotCreatedException : public std::exception
	{
		virtual const char* what() const throw()
		{
			return ("Unable to create socket");
		}
	};

	ssize_t	read(void *buf, size_t count);
	int		get(void)
private:
	int					_fd;
	struct sockaddr_in	_addr
};

#endif

int operator=(const WebservSocket& src)
{
	return (src.get());
}
int fd = socket;

read(soccket)


socket.read(buffer, buffer wise)
