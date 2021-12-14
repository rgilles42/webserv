#pragma once

class Resource {
public:

	Resource(std::string path);
	~Resource();

	std::string	getContent();
	
private:
	int			_fd;
	std::string	_content;
};