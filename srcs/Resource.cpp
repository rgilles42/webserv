/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Resource.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgilles <rgilles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 15:58:38 by rgilles           #+#    #+#             */
/*   Updated: 2022/01/10 18:11:49 by rgilles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Resource.hpp"


namespace Webserv {
	Resource::Resource(const std::string& path) : _path(path), _size(-1), _isFullyRead(false)
	{
		if (lstat(this->_path.c_str(), &this->_s) < 0)
			throw UnableToStatPathException();
		if (S_ISREG(this->_s.st_mode))
		{
			this->_isDir = false;
			if (!isCGIContent())
			{
				this->_isCGI = false;
				this->_fd = open(this->_path.c_str(), O_RDONLY);
				if (this->_fd < 0)
					throw ResourceNotOpenException();
				if (fcntl(this->_fd, F_SETFL, O_NONBLOCK) < 0)
					throw SetNonBlockFailedException();
				if (read(0, NULL, 0))
					throw UnableToReadResourceException();
				this->_size = this->_s.st_size;
				this->_contentType = Utils::getContentTypeByFile(this->_path, "text/plain");
			}
			/*else
			{
				this->_isCGI = true;
				this->_contentType = "text/html";
				this->_CGI = new CGI("/usr/bin/php-cgi", this->_path);
				this->_CGI->init_env_var();
				this->_fd = this->_CGI->getReadFD();
				this->_CGI->exec();
				
			}*/
		}
		else if (S_ISDIR(this->_s.st_mode))
		{
			this->_isDir = true;
		}
	}

	Resource::~Resource()
	{
		if (!this->_isDir)
			close(this->_fd);
		/*if (!this->_isDir && this->_isCGI)
			delete this->_CGI*/
	}

	bool	Resource::loadResource()
	{
		if (!this->_isCGI)
		{
			if (!this->_isDir)
				return (readFileChunk());
			/*else
				return (generateAutoIndex());*/
		}
		return (this->_isFullyRead);
	}

	bool	Resource::readCGIChunk()
	{
		char		buf[2048];
		int			rdsize;
		long long	totalReadBytes = 0;

		if ((rdsize = read(this->_fd, buf, 500)) > 0)
		{
			buf[rdsize] = 0;
			this->_content += buf;
			totalReadBytes += rdsize;
		}
		else if (!rdsize)
		{
			this->_size = totalReadBytes;
			this->_isFullyRead = true;
		}
		else
			throw UnableToReadResourceException();
		return (this->_isFullyRead);
	}

	std::string	Resource::getContent() const
	{
		return (this->_content);
	}

	std::string	Resource::getContentType() const
	{
		return (this->_contentType);
	}

	int	Resource::getFd() const
	{
		return (this->_fd);
	}

	long long	Resource::getSize() const
	{
		return (this->_size);
	}

	bool	Resource::isDir() const
	{
		return (this->_isDir);
	}

	bool	Resource::isCGI() const
	{
		return (this->_isCGI);
	}

	bool	Resource::isFullyRead() const
	{
		return (this->_isFullyRead);
	}

	void	Resource::setFd(int newfd)
	{
		this->_fd = newfd;
	}

/******************************************/

	bool	Resource::isCGIContent()
	{
		if (this->_path.substr(this->_path.find_last_of(".") + 1) == "php") //temporary
			return (true);
		return (false);
	}

	bool	Resource::readFileChunk()
	{
		struct		pollfd to_poll;
		char		buf[2048];
		int			rdsize;
		long long	totalReadBytes = 0;

		to_poll.fd = this->_fd;
		to_poll.events = POLLIN;
		if (poll(&to_poll, 1, 100) == 1 && to_poll.revents & POLLIN)
		{
			if ((rdsize = read(this->_fd, buf, 500)) > 0)
			{
				buf[rdsize] = 0;
				this->_content += buf;
				totalReadBytes += rdsize;
			}
			else if (totalReadBytes >= this->_size)
				this->_isFullyRead = true;
			else
				throw UnableToReadResourceException();
		}
		return (this->_isFullyRead);
	}
}
