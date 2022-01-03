/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgilles <rgilles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 16:50:50 by yun               #+#    #+#             */
/*   Updated: 2021/12/24 16:30:05 by rgilles          ###   ########.fr       */
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

namespace Webserv
{
	class CGI
	{
		private:
			std::string	location_cgi;
			std::string location_file;
			std::string	cgi_message;
			int		fd_in[2];
			int		fd_out[2];
			char	*args[3];
			std::map<std::string, std::string> m_env;
//			Request request;
//			VirtualServer server;

			void	free_dtab(char **tab);

		public:
			/* Constructor / Destructor */

			CGI(/*Rquest req*, VirtualServer srv*/,std::string path_cgi, std::string location_file);
			~CGI();

			/* Getter */

			std::string	getResult();	//Return cgi result after read
			int		getWriteFD();		//Return fd to write bodu request
			int		getReadFD();		//Return fd to read result cgi
			char	**env();			//Return convert env with char ** format

			/* Modifers data functions */

			void	init_env_var();	//init env
			void	add_env_var(std::string name, std::string value);	//Add var in env map

			/* Utils functions*/

			int		exec();		//Execve cgi
			void	readFD();	//Read fd_out[0] and stock in std::string (cgi_message += buffer)


			/* Error struct*/

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

			struct	fcntlCGIFailed : public std::exception
			{
				virtual const char* what() const throw()
				{
					return ("CGI: Fcntl failed");
				}
			};

			struct	readCGIFailed : public std::exception
			{
				virtual const char* what() const throw()
				{
					return ("CGI: Read failed");
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
