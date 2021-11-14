/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 15:47:23 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/11 19:37:05 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <sstream>
# include <fstream>
# include <streambuf>
# include <string>
# include <ctime>
# include <algorithm>
# include <vector>

# include <iostream>

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

# define SSTR( x ) static_cast< std::ostringstream & >( ( std::ostringstream() << std::dec << x ) ).str()

struct ci_less : std::binary_function<std::string, std::string, bool> {
	// case-independent (ci) compare_less binary function
	struct nocase_compare : public std::binary_function<unsigned char,unsigned char,bool>
	{
		bool operator() (const unsigned char& c1, const unsigned char& c2) const {
			return std::tolower (c1) < std::tolower (c2);
		}
	};
	bool operator() (const std::string & s1, const std::string & s2) const {
		return std::lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end(), nocase_compare ());
	}
};

std::string	&ltrim(std::string &s);
std::string	&rtrim(std::string &s);
std::string	&trim(std::string &s);

std::string	getFormatedDate(const time_t &rawtime);

std::string	getFileContents(const std::string &filename);
std::string	getFileExtension(const std::string &filename);

std::string getContentTypeByFile(const std::string &filename, const std::string &fallback = "text/plain");

bool		isIpv4Address(const std::string &str);
bool		isIpv6Address(const std::string &str);
bool		isIpAddress(const std::string &str);

std::vector<std::string>	split(const std::string &str, const std::string &token);

# include "MimeTypes.hpp"


#endif