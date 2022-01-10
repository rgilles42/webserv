/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Resource.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 15:59:30 by rgilles           #+#    #+#             */
/*   Updated: 2022/01/10 15:22:19 by ppaglier         ###   ########.fr       */
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
			Resource(const std::string& path);
			~Resource();
			//void		readCGIContent();

			std::string	getContent() const;
			std::string getContentType() const;

			int			getFd() const;
			long long	getSize() const;
			bool		isDir() const;
			bool		isCGI() const;
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
			Resource();
			Resource(const Resource& src);

			bool		isCGIContent();

			void		populateContentFile();
			//void		populateContentCGI();
			void		populateContentFolder();

			//void		generateAutoIndex();

			void		readContent();

			const std::string	_path;
			struct stat			_s;
			bool				_isDir;
			bool				_isCGI;
			int					_fd;
			long long			_size;
			std::string			_content;
			std::string			_contentType;
			//Webserv::CGI*		_CGI;
	};
}

#endif