#include "../includes/Resource.hpp"

namespace Webserv {
	Resource::Resource(std::string path) : _contentType("text/plain")
	{
		this->_fd = open(path.c_str(), O_RDONLY);
		if (this->_fd < 0)
			throw ResourceNotOpenException();
		if (fcntl(this->_fd, F_SETFL, O_NONBLOCK) < 0)
			throw SetNonBlockFailedException();
		if (read(0, NULL, 0))
			throw UnableToReadResourceException();
	}
	Resource::~Resource() {}
	void	Resource::readContent()
	{
		struct	pollfd to_poll;
		char	buf[2048];
		int		rdsize;

		to_poll.fd = this->_fd;
		to_poll.events = POLLIN;
		poll(&to_poll, 1, 100);
		while (to_poll.revents & POLLIN)
		{
			if ((rdsize = read(this->_fd, buf, 2048)) > 0)
			{
				buf[rdsize] = 0;
				this->_content += buf;
				poll(&to_poll, 1, 100);
			}
			else if (!rdsize)
				break ;
			else
				throw UnableToReadResourceException();
		}
	}

	std::string	Resource::getContent() {return (this->_content);}
	int	Resource::getFd() {return (this->_fd);}
	void	Resource::setFd(int newfd) {this->_fd = newfd;}

}
