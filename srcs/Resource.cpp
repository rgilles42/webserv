/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Resource.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgilles <rgilles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 15:58:38 by rgilles           #+#    #+#             */
/*   Updated: 2021/12/24 16:29:20 by rgilles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Resource.hpp"
#include "../includes/utils/MimeTypes.hpp"


namespace Webserv {
	Resource::Resource(std::string path) : _size(0)
	{
		struct stat s;

		if (lstat(path.c_str(), &s) < 0)
			throw UnableToStatPathException();
		if (S_ISREG(s.st_mode))
		{
			this->_isDir = false;
			this->_fd = open(path.c_str(), O_RDONLY);
			if (this->_fd < 0)
				throw ResourceNotOpenException();
			if (fcntl(this->_fd, F_SETFL, O_NONBLOCK) < 0)
				throw SetNonBlockFailedException();
			if (read(0, NULL, 0))
				throw UnableToReadResourceException();
			this->_contentType = Utils::getContentTypeByFile(path, "text/plain");
			this->readContent();
		}
		else if (S_ISDIR(s.st_mode))
		{
			this->_isDir = true;
			//this->generateAutoindex();

		}
	}
	Resource::~Resource()
	{
		if (!_isDir)
			close(this->_fd);
	}
	void	Resource::readContent()
	{
		struct		pollfd to_poll;
		char		buf[2048];
		int			rdsize;

		to_poll.fd = this->_fd;
		to_poll.events = POLLIN;
		while (true)
		{
			if (poll(&to_poll, 1, 100) == 1 && to_poll.revents & POLLIN)
			{
				if ((rdsize = read(this->_fd, buf, 500)) > 0)
				{
					buf[rdsize] = 0;
					this->_content += buf;
					this->_size += rdsize;
				}
				else if (!rdsize)
					break ;
				else
					throw UnableToReadResourceException();
			}
		}
	}

	std::string	Resource::getContent() const {return (this->_content);}
	std::string	Resource::getContentType() const {return (this->_contentType);}
	int	Resource::getFd() const {return (this->_fd);}
	long long	Resource::getSize() const {return (this->_size);}
	void	Resource::setFd(int newfd) {this->_fd = newfd;}

}
