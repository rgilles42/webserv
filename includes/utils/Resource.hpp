/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Resource.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 15:59:30 by rgilles           #+#    #+#             */
/*   Updated: 2022/01/30 03:09:49 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESOURCE_HPP
# define RESOURCE_HPP

# include <poll.h>			// For poll, pollfd, etc..
# include <sys/types.h>		// For ??
# include <sys/stat.h>		// For stat
# include <fcntl.h>			// For fcntl
# include <string>			// For std::string
# include <unistd.h>		// For read, write, close

# include "MimeTypes.hpp"	// For Mimetypes not used for the moment aparently

namespace Webserv {

	namespace Utils {

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

				bool		readFileChunk();
				//void		generateAutoIndex();

				std::string	_path;
				struct stat			_s;
				bool				_isDir;
				bool			_isCGI;
				int					_fd;
				long long			_size;
				long long			_readBytes;
				std::string			_content;
				std::string			_contentType;
				bool				_isFullyRead;
		};

	} // namespace Utils

} // namespace Webserv

#endif