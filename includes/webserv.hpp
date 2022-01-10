/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 15:47:11 by ppaglier          #+#    #+#             */
/*   Updated: 2022/01/10 14:48:52 by ppaglier         ###   ########.fr       */
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
# define BUFFER_SIZE 4096
# define MAX_CONNECTION 50

# include "./Resource.hpp"
# include "./utils/common.hpp"
# include "./Socket.hpp"

# include "./http/HttpVersion.hpp"
# include "./http/HttpStatusCode.hpp"
# include "./http/HttpRequest.hpp"
# include "./http/HttpResponse.hpp"

# include "./utils/Byte.hpp"
# include "./utils/Token.hpp"
# include "./utils/Lexer.hpp"
# include "./utils/Block.hpp"
# include "./utils/Parser.hpp"
# include "./utils/MimeTypes.hpp"
# include "./utils/Address.hpp"
# include "./utils/OutStream.hpp"
# include "./utils/Singleton.hpp"

# include "./Logger.hpp"
# include "./Config.hpp"
# include "./core/Core.hpp"

// namespace Webserv {

// 	Webserv::Core &mainCore = Webserv::Utils::Singleton<Webserv::Core>::getInstance();

// } // namespace Webserv

#endif
