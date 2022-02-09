/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Resource.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgilles <rgilles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 15:59:30 by rgilles           #+#    #+#             */
/*   Updated: 2022/02/09 15:34:50 by rgilles          ###   ########.fr       */
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
# include "../http/Route.hpp"	// For Mimetypes
# include "MimeTypes.hpp"	// For Mimetypes not used for the moment aparently

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
				typedef Webserv::Http::Route		http_route_type;

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

				struct Resource404Exception : public ResourceException {
					Resource404Exception(std::string msg) : ResourceException(msg) {}
				};
				struct PathDoesNotExistException : public Resource404Exception {
					PathDoesNotExistException(void) : Resource404Exception("No such file or directory") {}
				};
				struct NotFileOrDirException : public Resource404Exception {
					NotFileOrDirException(void) : Resource404Exception("Specified resource is neither a regular file nor a directory.") {}
				};

				//////////////////////////

				struct Resource403Exception : public ResourceException {
					Resource403Exception(std::string msg) : ResourceException(msg) {}
				};
				struct AccessForbiddenException : public Resource403Exception {
					AccessForbiddenException(void) : Resource403Exception("Access Forbidden") {}
				};

				//////////////////////////
				
				struct Resource500Exception : public ResourceException {
					Resource500Exception(std::string msg) : ResourceException(msg) {}
				};
				struct UnableToStatPathException : public Resource500Exception {
					UnableToStatPathException(void) : Resource500Exception("Unable to stat path") {}
				};
				struct ResourceNotOpenException : public Resource500Exception {
					ResourceNotOpenException(void) : Resource500Exception("Unable to open resource") {}
				};
				struct SetNonBlockFailedException : public Resource500Exception {
					SetNonBlockFailedException(void) : Resource500Exception("Unable to set resource to non-blocking") {}
				};
				struct UnableToReadResourceException : public Resource500Exception {
					UnableToReadResourceException(void) : Resource500Exception("Unable to read resource") {}
				};

			protected:
				bool		readFileChunk(void);
				void		generateAutoIndex(void);

				path_type			_path;
				path_type			_cgi_path;
				bool				_isDir;
				bool				_isCGI;
				fd_type				_fd;
				size_type			_size;
				read_bytes_type		_readBytes;
				content_type		_more_headers;
				content_type		_content;
				content_type_type	_contentType;
				bool				_isFullyRead;

			private:
				Resource(const Resource& src);

			public:
				Resource(void);
				~Resource();

				Resource&		operator=(const Resource& lhs);

				void			init(const path_type& path, const bool& isCGI, const http_route_type& route);

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