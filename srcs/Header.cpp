/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 16:13:37 by ppaglier          #+#    #+#             */
/*   Updated: 2021/10/26 17:52:21 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Header.hpp"

void		Header::initForSend() {
	this->statusCode = "200 OK";
	this->setKeyValue("Server", "Webserv/1.0.0");
	this->setKeyValue("Last-Modified", getFormatedDate(time(NULL) - 100));
	this->setKeyValue("Connection", "keep-alive");
	// this->setKeyValue("ETag", "5e889dfb-264");
	this->setKeyValue("Content-Type", "text/plain");
	this->setKeyValue("Content-Length", SSTR(0));

	// this->setKeyValue("Content-Encoding", "gzip");

	this->setKeyValue("Date", getFormatedDate(time(NULL)));

	// this->setKeyValue("Transfer-Encoding", "chunked"); // Cannot be present while Content-Lenght is present
}

void		Header::setKeyValue(const std::string &key, const std::string &value) {
	this->headers[key] = value;
}

const std::string	Header::getKeyValue(const std::string &key) {
	return this->headers[key];
}

void		Header::setStatusCode(const std::string &statusCode) {
	this->statusCode = statusCode;
}

void		Header::setContentType(const std::string &contentType) {
	this->setKeyValue("Content-Type", contentType);
}

void		Header::setContentHash(const uint32_t &contentHash) {
	this->setKeyValue("Content-Hash", SSTR(contentHash));
}

void		Header::setContentLength(const size_t &contentLength) {
	this->setKeyValue("Content-Length", SSTR(contentLength));
}

std::string	Header::getFormatedHeader(void) const {
	std::map<std::string, std::string>::const_iterator it = this->headers.begin();
	std::map<std::string, std::string>::const_iterator end = this->headers.end();
	std::string	formatedHeaders = "";

	formatedHeaders += std::string(HTTP_VERSION) + " " + this->statusCode + "\r\n";
	while (it != end) {
		formatedHeaders += it->first + ": " + it->second + "\r\n";
		it++;
	}
	formatedHeaders += "\r\n";
	return formatedHeaders;
}

