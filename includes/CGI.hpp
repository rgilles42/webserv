/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yun <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 16:50:50 by yun               #+#    #+#             */
/*   Updated: 2021/11/24 17:05:17 by yun              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

#include <exception>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <map>
#include <sys/types.h>
#include <sys/wait.h>
//#include <Request.hpp>
//#include <Poll.hpp>

namespace webserv
{
	class CGI
	{
		private:
			std::string	location_cgi;
			std::string location_file;
			std::string	cgi_message;
			int		fd;
			char	*args[3];
			std::map<std::string, std::string> m_env;
//			Request request;
//			VirtualServer srv;
//			Poll	poll_cgi;

			void	free_dtab(char **tab);

		public:
			CGI(/*Request req*/);
			CGI(/*Rquest req*/,std::string path_cgi, std::string location_file);
			~CGI();

			char	**env();
			int		exec();
			void	init_env_var();
			void	add_env_var(std::string name, std::string value);
			void	set_args(std::string location, std::string file);
			void	readFD();

			struct	dupCGIFailed : public std::exception
			{
				virtual const char* what() const throw()
				{
					return ("CGI: Dup failed");
				}
			};
			struct	pipeCGIFailed : public std::exception
			{
				virtual const char* what() const throw()
				{
					return ("CGI: Pipe failed");
				}
			};

			struct	pidCGIFailed : public std::exception
			{
				virtual const char* what() const throw()
				{
					return ("CGI: Pid failed");
				}
			};
	};
}

#endif
