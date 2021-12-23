/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Resource.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgilles <rgilles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 15:59:30 by rgilles           #+#    #+#             */
/*   Updated: 2021/12/23 19:14:06 by rgilles          ###   ########.fr       */
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

namespace Webserv {
	class Resource {
	public:
		Resource(std::string path);
		~Resource();
		void		readContent();
		//void		readCGIContent();

		std::string	getContent() const;
		std::string getContentType() const;

		int			getFd() const;
		long long	getSize() const;
		bool		isDir();
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

		bool		_isDir;
		int			_fd;
		long long	_size;
		std::string	_content;
		std::string	_contentType;
	};
}

#endif