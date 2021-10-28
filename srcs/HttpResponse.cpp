/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 16:45:31 by ppaglier          #+#    #+#             */
/*   Updated: 2021/10/28 18:07:49 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/HttpResponse.hpp"

// Request Methods
void		HttpResponse::setProtocol(const std::string &protocol) {
	this->protocol = protocol;
}

void		HttpResponse::setStatusCode(const std::string &statusCode) {
	this->statusCode = statusCode;
}

const std::string   HttpResponse::getProtocol(void) const {
	return this->protocol;
}

const std::string   HttpResponse::getStatusCode(void) const {
	return this->statusCode;
}

// Headers Methods
void		HttpResponse::setContent(const std::string &content) {
	this->content = content;
}

void		HttpResponse::setContentType(const std::string &contentType) {
	this->headers.set("Content-Type", contentType);
}

void		HttpResponse::setContentHash(const uint32_t &contentHash) {
	this->headers.set("Content-Hash", SSTR(contentHash));
}

void		HttpResponse::setContentLength(const size_t &contentLength) {
	this->headers.set("Content-Length", SSTR(contentLength));
}

// Utils Methods
void		HttpResponse::prepareResponse() {
	this->setProtocol();
	this->setStatusCode();

	this->headers.set("Server", DEFAULT_SERVER);
	this->headers.set("Last-Modified", getFormatedDate(time(NULL) - 100));
	this->headers.set("Connection", "keep-alive");

	// this->headers.set("ETag", "5e889dfb-264");

	this->setContentType();
	this->setContentLength();

	// this->headers.set("Content-Encoding", "gzip");

	this->headers.set("Date", getFormatedDate(time(NULL)));

	// this->headers.set("Transfer-Encoding", "chunked"); // Cannot be present while Content-Lenght is present
}

void	HttpResponse::fromString(const std::string &response) {

	std::string			line(response);

	// Parse response protocol
	this->setProtocol(line.substr(0, line.find(' ')));
	line = line.erase(0, line.find(' ') + 1);

	// Parse response status
	this->setStatusCode(line.substr(0, line.find(LINE_BREAK)));
	line = line.erase(0, line.find(LINE_BREAK) + LINE_BREAK.length());

	// Parse response headers
	this->headers.fromString(line.substr(0, line.find(LINE_BREAK + LINE_BREAK)));
	line = line.erase(0, line.find(LINE_BREAK + LINE_BREAK) + (LINE_BREAK + LINE_BREAK).length());

	// Parse response content
	this->content = line;
}

std::string	HttpResponse::toString(void) const {
	std::string	formatedResponse = "";

	formatedResponse += this->getProtocol() + " " + this->getStatusCode() + LINE_BREAK;
	formatedResponse += this->headers.toString() + LINE_BREAK;
	formatedResponse += this->content;
	return formatedResponse;
}
