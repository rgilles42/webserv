/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Resource.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgilles <rgilles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 15:59:30 by rgilles           #+#    #+#             */
/*   Updated: 2022/01/19 21:48:03 by rgilles          ###   ########.fr       */
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
# include <dirent.h>
# include <cerrno>
# include <ctime>
# include "utils/MimeTypes.hpp"

namespace Webserv {
	class Resource {
	public:
		Resource();
		Resource(const std::string& path, const bool isCGI = false);
		~Resource();
		Resource&	operator=(const Resource& lhs);
		

		bool		loadResource();

		void		closeResource();

		std::string	getContent() const;
		std::string getContentType() const;

		int			getFd() const;
		long long	getSize() const;
		bool		isDir() const;
		bool		isCGI() const;
		bool		isFullyRead() const;
		void		setFd(int newfd);

		struct PathDoesNotExistException : public std::exception
		{
			virtual const char* what() const throw()
			{
				return("No such file or directory");
			}
		};
		struct AccessForbiddenException : public std::exception
		{
			virtual const char* what() const throw()
			{
				return("Access Forbidden");
			}
		};
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
		struct NotFileOrDirException : public std::exception
		{
			virtual const char* what() const throw()
			{
				return("Specified resource is neither a regular file nor a directory.");
			}
		};
		
	private:
		Resource(const Resource& src);

		bool		readFileChunk();
		void		generateAutoIndex();

		std::string	_path;
		bool		_isDir;
		bool		_isCGI;
		int			_fd;
		long long	_size;
		long long	_readBytes;
		std::string	_content;
		std::string	_contentType;
		bool		_isFullyRead;
	};
}

#endif