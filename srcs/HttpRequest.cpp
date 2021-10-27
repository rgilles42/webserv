/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 16:45:31 by ppaglier          #+#    #+#             */
/*   Updated: 2021/10/27 18:36:56 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/HttpRequest.hpp"

// Request Methods
void		HttpRequest::setMethod(const std::string &method) {
	this->method = method;
}

void		HttpRequest::setPath(const std::string &path) {
	this->path = path;
}

void		HttpRequest::setProtocol(const std::string &protocol) {
	this->protocol = protocol;
}

void		HttpRequest::setStatusCode(const std::string &statusCode) {
	this->statusCode = statusCode;
}

const std::string   HttpRequest::getMethod(void) const {
	return this->method;
}

const std::string   HttpRequest::getPath(void) const {
	return this->path;
}

const std::string   HttpRequest::getProtocol(void) const {
	return this->protocol;
}

const std::string   HttpRequest::getStatusCode(void) const {
	return this->statusCode;
}

// Headers Methods
bool		HttpRequest::isHeaderKeyValid(const std::string &key) {
	for (size_t i = 0; i < key.length(); i++) {
		if (!IS_AUTHORIZED_CHAR(key[i])) {
			return false;
		}
	}
	return true;
}

void		HttpRequest::setHeaderKeyValue(const std::string &key, const std::string &value) {
	if (!this->isHeaderKeyValid(key)) {
		throw std::runtime_error("isHeaderKeyValid: " + key);
	}
	this->headers[key] = value;
}

const std::string	HttpRequest::getHeaderKeyValue(const std::string &key) const {
	return this->headers.at(key);
}

void		HttpRequest::setContentType(const std::string &contentType) {
	this->setHeaderKeyValue("Content-Type", contentType);
}

void		HttpRequest::setContentHash(const uint32_t &contentHash) {
	this->setHeaderKeyValue("Content-Hash", SSTR(contentHash));
}

void		HttpRequest::setContentLength(const size_t &contentLength) {
	this->setHeaderKeyValue("Content-Length", SSTR(contentLength));
}

// Utils Methods
void		HttpRequest::prepareRequest() {
	this->setProtocol();
	this->setStatusCode();

	this->setHeaderKeyValue("Server", DEFAULT_SERVER);
	this->setHeaderKeyValue("Last-Modified", getFormatedDate(time(NULL) - 100));
	this->setHeaderKeyValue("Connection", "keep-alive");

	// this->setHeaderKeyValue("ETag", "5e889dfb-264");

	this->setContentType();
	this->setContentLength();

	// this->setHeaderKeyValue("Content-Encoding", "gzip");

	this->setHeaderKeyValue("Date", getFormatedDate(time(NULL)));

	// this->setHeaderKeyValue("Transfer-Encoding", "chunked"); // Cannot be present while Content-Lenght is present
}

std::string	HttpRequest::formatToString(void) const {
	std::map<std::string, std::string>::const_iterator it = this->headers.begin();
	std::map<std::string, std::string>::const_iterator end = this->headers.end();
	std::string	formatedRequest = "";

	formatedRequest += this->getProtocol() + " " + this->getStatusCode() + LINE_BREAK;
	while (it != end) {
		formatedRequest += it->first + ": " + it->second + LINE_BREAK;
		it++;
	}
	formatedRequest += LINE_BREAK;
	return formatedRequest;
}

void	HttpRequest::formatFromString(const std::string &request) {

	std::string			line;
	std::stringstream	ss(request);
	size_t				i = 0;
	while (std::getline(ss, line)) {
		line = trim(line);
		if (i == 0) {
			this->setMethod(line.substr(0, line.find(' ')));
			line = line.erase(0, line.find(' ') + 1);
			this->setPath(line.substr(0, line.find(' ')));
			line = line.erase(0, line.find(' ') + 1);
			this->setProtocol(line);
		} else {
			std::string key = line.substr(0, line.find(':'));
			line = line.erase(0, line.find(':') + 1);
			std::string value = line;
			this->setHeaderKeyValue(trim(key), trim(value));
		}
		i++;
	}
}

