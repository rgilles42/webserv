/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Resource.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgilles <rgilles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 15:59:30 by rgilles           #+#    #+#             */
/*   Updated: 2022/01/10 19:01:44 by rgilles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESOURCE_HPP
# define RESOURCE_HPP

# include <poll.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <string>
# include <unistd.h>
# include "utils/MimeTypes.hpp"
//#include "CGI.hpp"

namespace Webserv {
	class Resource {
	public:
		Resource();
		Resource(const std::string& path);
		~Resource();
		Resource&	operator=(const Resource& lhs);
		

		bool		loadResource();
		bool		readCGIChunk();

		void		closeResource();

		std::string	getContent() const;
		std::string getContentType() const;

		int			getFd() const;
		long long	getSize() const;
		bool		isDir() const;
		bool		isCGI() const;
		bool		isFullyRead() const;
		void		setFd(int newfd);
		void		setBoolCGI(bool new_value);

		struct UnableToStatPathException : public std::exception
		{
			virtual const char* what() const throw()
			{
				return("Unable to stat path");
			}
		};
		struct ResourceNotOpenException : public std::exception
		{
			virtual const char* what() const throw()
			{
				return("Unable to open resource");
			}
		};
		struct SetNonBlockFailedException : public std::exception
		{
			virtual const char* what() const throw()
			{
				return("Unable to set resource to non-blocking");
			}
		};
		struct UnableToReadResourceException : public std::exception
		{
			virtual const char* what() const throw()
			{
				return("Unable to read resource");
			}
		};
		
	private:
		Resource(const Resource& src);

		bool		isCGIContent();
		bool		readFileChunk();
		//void		generateAutoIndex();

		std::string	_path;
		struct stat			_s;
		bool				_isDir;
		bool				_isCGI;
		int					_fd;
		long long			_size;
		long long			_readBytes;
		std::string			_content;
		std::string			_contentType;
		bool				_isFullyRead;
		//Webserv::CGI*		_CGI;
	};
}

#endif