#pragma once
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

		std::string	getContent();
		std::string getContentType();

		int			getFd();
		void		setFd(int newfd);

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

		int			_fd;
		std::string	_content;
		std::string	_contentType;
	};
}