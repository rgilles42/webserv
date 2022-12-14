/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Resource.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 15:58:38 by rgilles           #+#    #+#             */
/*   Updated: 2022/02/11 22:07:06 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils/Resource.hpp"

namespace Webserv {

	namespace Utils {

		Resource::Resource(void)	: _size(-1), _readBytes(0), _isFullyRead(false) {}
		Resource::~Resource() {}

		Resource&	Resource::operator=(const Resource& rhs)
		{
			if (this != &rhs)
			{
				this->_path = rhs._path;
				this->_isDir = rhs._isDir;
				this->_isCGI = rhs._isCGI;
				this->_fd = rhs._fd;
				this->_size = rhs._size;
				this->_readBytes = rhs._readBytes;
				this->_content = rhs._content;
				this->_contentType = rhs._contentType;
				this->_isFullyRead = rhs._isFullyRead;
			}
			return (*this);
		}

		void	Resource::init(const path_type& path, const bool& isCGI, const http_route_type& route)
		{
			struct stat	s;

			this->_path = path;
			this->_isCGI = isCGI;
			errno = 0;
			if (stat(this->_path.c_str(), &s) < 0)
			{
				if (errno == ENOENT)
					throw PathDoesNotExistException();
				if (errno == EACCES)
					throw AccessForbiddenException();
				throw UnableToStatPathException();
			}
			if (S_ISREG(s.st_mode))
			{
				this->_isDir = false;
				if (!isCGI)
				{
					this->_fd = open(this->_path.c_str(), O_RDONLY);
					if (this->_fd < 0)
					{
						if (errno == EACCES)
							throw AccessForbiddenException();
						throw ResourceNotOpenException();
					}
					if (fcntl(this->_fd, F_SETFL, O_NONBLOCK) < 0)
						throw SetNonBlockFailedException();
					if (read(this->_fd, NULL, 0))
						throw UnableToReadResourceException();
					this->_size = s.st_size;
					this->_contentType = route.getMimeTypes().getType(this->_path, "text/plain");
				}
				else
					this->_contentType = "";
			}
			else if (S_ISDIR(s.st_mode))
			{
				if (route.getAutoindex())
				{
					this->_isDir = true;
					this->_contentType = "text/html";
					this->generateAutoIndex();
				}
				else
					throw AccessForbiddenException();
			}
			else
				throw NotFileOrDirException();
		}

		bool		Resource::loadResource(void)
		{
			if (!this->_isDir) {
				return (readFileChunk());
			}
			return (this->_isFullyRead);
		}

		void		Resource::closeResource(void)
		{
			if (!this->_isDir && !this->_isCGI) {
				close(this->_fd);
			}
		}

		const Resource::content_type&	Resource::getContent(void) const
		{
			return (this->_content);
		}

		const Resource::content_type_type&	Resource::getContentType(void) const
		{
			return (this->_contentType);
		}

		const Resource::fd_type&	Resource::getFd(void) const
		{
			return (this->_fd);
		}

		const Resource::size_type&	Resource::getSize(void) const
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

		void	Resource::setFd(const fd_type& newfd)
		{
			this->_fd = newfd;
		}

		//void	Resource::setContentType


	/******************************************/

		bool	Resource::readFileChunk()
		{
			poll_type	res_poll;
			poll_type::poll_fd_vector::const_iterator	poll_it;
			char		buf[BUFFER_SIZE + 1];
			int			rdsize = 0;
			long long	totalReadBytes = 0;

			res_poll.add_fd(this->_fd, POLLIN);
			while (true)
			{
				res_poll.exec();
				poll_it = res_poll.getPollUsedFD().begin();
				if (poll_it != res_poll.getPollUsedFD().end()) {
					if ((poll_it->revents & POLLIN) == POLLIN && (rdsize = read(this->_fd, buf, BUFFER_SIZE)) > 0)
					{
						buf[rdsize] = 0;
						this->_content.append(buf, rdsize);
						this->_readBytes += rdsize;
						totalReadBytes += rdsize;
					}
					else if (rdsize < 0)
						throw UnableToReadResourceException();
					else
					{
						this->_size = totalReadBytes;
						this->_isFullyRead = true;
						break ;
					}
				}
			}
			return (this->_isFullyRead);
		}

		void	Resource::generateAutoIndex()
		{
			std::ostringstream	content;
			DIR					*dir;
			struct dirent		*ent;

			errno = 0;
			if ((dir = opendir (this->_path.c_str())) != NULL)
			{
				content << "<!doctype html><html><meta charset=\"utf-8\"/> <style>span.folder-icon::before{content: url(\'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAFo9M/3AAAACXBIWXMAAA3XAAAN1wFCKJt4AAABR0lEQVQ4y2P4//8/AwgzrK63/n9pQ+V/htl5uv/BIiAeWGRvn+v/O/v74JhhZoHh/xMLUuAYrAeIt8JNQ1YO1gIzDG4osn6wGcenek18fmntf2z46i6gEVvaHf5f396CEzMsKjf7f3ZFPk7MMDFD5+ThGVH/seGt3b5gf6yG+uX/qgZtNph/4P4COQSXI0EewPA4MgaFIwM+H4B8iBFSyBgUSQz4vAgKAoygRcagqGMA+RVXOIDCCOhXBkZQGBycGvb/0LQIMD44Nfz/imrr/3Py9X4B8Q90PDdf7/uSMqPNIL0MhyZ5fHp4bNb/+4enkIRBekB6GXZ2O/+/tbuTLAzSy7Ch2e7/1c11ZGGQXoZl1Rb/L6wpIQuD9DLMLTb+f3pJJlkYpJdhSo7e/2Nz4snCIL0MPWnaNj0pmie7UzT/k4JBekB6AVZNyOQlBCjdAAAAAElFTkSuQmCC\');}span.file-icon::before{content: url(\'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAFo9M/3AAAACXBIWXMAAA3XAAAN1wFCKJt4AAABSUlEQVQ4y52SP2uDUBTF75fr2g/mlsHBQTFDBgdBDQYEp4AEFInt1o4iGM0/E3EQh1vPA1PbpqVx+PHuu3ge95wrRVH0FAQB02Kx4OVyyYSb6MRxzIBc1+XVaiVIkoRJlmW2bVtQFMWnBmw2mxe6XC63z1HT8XgUzwPUBF0YhkIv3vB9n+u6vkvTNEymafL5fGZd1xkjbbdbAWr0SVVV3u/37DiOQNM0cfYeRR9zv6Vpynme/wDTEmYfexmDpAiD/DYkDAjj8/n8Zn7IDX0MSt8d4BwcwOGXpABWMKSGJdFfFhGBiBYF1oOIxzFjdYSB7mWAbJARrdfrV1zatuWu6/4FvoUGWvI8j6/X6ySgJcuyuKqqSUBLhmHw6XSaBLSELR4Oh0lAS4qicFmWk4CWZrMZZ1nGu93uIaCBlvr/7Lkv3iVJ4keABtoPjIF1YvwFbwgAAAAASUVORK5CYII=\');}</style> <head> <title>Index of ";
				content << this->_path;
				content << "</title> </head> <body> <h1>Index of ";
				content << this->_path;
				content << "</h1>";
				content << "<table> <tbody> <tr> <th></th> <th>Name</th> <th>Last modified</th> <th>Size</th> </tr>";
				while ((ent = readdir (dir)) != NULL)
				{
					struct stat	s;
					std::string	subpath = this->_path + "/" + ent->d_name;
					stat(subpath.c_str(), &s);
					if ((S_ISREG(s.st_mode) || S_ISDIR(s.st_mode)))
					{
						content << "<tr> <td><a href=\"./" << ent->d_name;
						if (S_ISDIR(s.st_mode))
							content << "/\"><span class=\"folder-icon\"/></a></td>";
						else
							content << "\"><span class=\"file-icon\"/></a></td>";
						content << "<td><a href=\"./" << ent->d_name;
						if (S_ISDIR(s.st_mode))
							content << "/\">" << ent->d_name << "/";
						else
							content << "\">" << ent->d_name;
						content << "</a></td><td>" << ctime(&s.st_mtime) << "</td><td>";
						if (S_ISDIR(s.st_mode))
							content << "  - ";
						else
							content << s.st_size;
						content << "</td></tr>";
					}
				}
				content << "</tbody> </table></body></html>";
				closedir(dir);
			}
			else
				if (errno == EACCES)
					throw AccessForbiddenException();
			this->_content += content.str();
			this->_size = this->_content.size();
			this->_isFullyRead = true;
		}

	} // namespace Utils

} // namespace Webserv