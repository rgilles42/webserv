/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 15:47:11 by ppaglier          #+#    #+#             */
/*   Updated: 2021/10/28 19:56:23 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# if defined (_WIN32) || defined (_WIN64)
	#  include <winsock2.h>
	typedef int socklen_t;
# elif defined (__unix__)
	#  include <sys/types.h>
	#  include <sys/socket.h>
	#  include <netinet/in.h>
	#  include <arpa/inet.h>
	#  include <unistd.h>
	#  define closesocket(s) close(s)
	typedef int SOCKET;
	typedef struct sockaddr_in SOCKADDR_IN;
	typedef struct sockaddr SOCKADDR;
# endif

# include <stdio.h>
# include <stdlib.h>
# include <cstring>
# include <string>
# include <iostream>
# include <fcntl.h>
# include <signal.h>

# define PORT 80
# define BUFFER_SIZE 50
# define MAX_CONNECTION 50

# include "./Ressource.hpp"
# include "./HttpRequest.hpp"
# include "./HttpResponse.hpp"
# include "./utils.hpp"

#endif