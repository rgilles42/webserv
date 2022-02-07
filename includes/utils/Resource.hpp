/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Resource.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 15:59:30 by rgilles           #+#    #+#             */
/*   Updated: 2022/02/07 15:35:34 by rgilles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESOURCE_HPP
# define RESOURCE_HPP

# include <sys/types.h>		// For ??
# include <sys/stat.h>		// For stat
# include <fcntl.h>			// For fcntl
# include <string>			// For std::string
# include <unistd.h>		// For read, write, close
# include <dirent.h>		// For Dir
# include <cerrno>			// For errno
# include <ctime>			// For times methods

# include "../events/Poll.hpp"
# include "MimeTypes.hpp"	// For Mimetypes

namespace Webserv {

	namespace Utils {

		class Resource {
			public:
				typedef std::string	path_type;
				typedef long long	size_type;
				typedef long long	read_bytes_type;
				typedef std::string	content_type;
				typedef std::string	content_type_type;
				typedef int			fd_type;
				typedef Webserv::Utils::MimeTypes	mime_types_type;

				class ResourceException : public std::exception {
					protected:
						std::string	msg;

					public:
						ResourceException(const std::string& msg = "") : std::exception() {
							this->msg = msg;
						}
						virtual ~ResourceException() throw() {}
						virtual const char	*what() const throw() {
							return this->msg.c_str();
						}
				};

				struct PathDoesNotExistException : public ResourceException {
					PathDoesNotExistException(void) : ResourceException("No such file or directory") {}
				};
				struct AccessForbiddenException : public ResourceException {
					AccessForbiddenException(void) : ResourceException("Access Forbidden") {}
				};
				struct UnableToStatPathException : public ResourceException {
					UnableToStatPathException(void) : ResourceException("Unable to stat path") {}
				};
				struct ResourceNotOpenException : public ResourceException {
					ResourceNotOpenException(void) : ResourceException("Unable to open resource") {}
				};
				struct SetNonBlockFailedException : public ResourceException {
					SetNonBlockFailedException(void) : ResourceException("Unable to set resource to non-blocking") {}
				};
				struct UnableToReadResourceException : public ResourceException {
					UnableToReadResourceException(void) : ResourceException("Unable to read resource") {}
				};
				struct NotFileOrDirException : public ResourceException {
					NotFileOrDirException(void) : ResourceException("Specified resource is neither a regular file nor a directory.") {}
				};

			protected:
				bool		readFileChunk(void);
				void		generateAutoIndex(void);

				path_type		_path;
				path_type		_cgi_path;
				bool			_isDir;
				bool			_isCGI;
				fd_type			_fd;
				size_type		_size;
				read_bytes_type	_readBytes;
				content_type	_content;
				content_type_type	_contentType;
				bool			_isFullyRead;

			private:
				Resource(const Resource& src);

			public:
				Resource(void);
				Resource(const path_type& path, const bool& isCGI, const MimeTypes& mime);
				~Resource();

				Resource&		operator=(const Resource& lhs);

				bool			loadResource(void);

				void			closeResource(void);

				const content_type&			getContent(void) const;
				const content_type_type&	getContentType(void) const;

				const fd_type&	getFd(void) const;
				const size_type&	getSize(void) const;
				bool			isDir(void) const;
				bool			isCGI(void) const;
				bool			isFullyRead(void) const;
				void			setFd(const fd_type& newfd);
			};

	} // namespace Utils

} // namespace Webserv

#endif