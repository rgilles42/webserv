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

HttpResponse::HttpResponse(void) {}

HttpResponse::HttpResponse(const HttpResponse &x) {
	this->protocol = x.getProtocol();
	this->statusCode = x.getStatusCode();
	this->headers = x.getHeaders();
	this->body = x.getBody();
}

HttpResponse::HttpResponse(const std::string &response) {
	this->fromString(response);
}

// Request Methods
void		HttpResponse::setProtocol(const std::string &protocol) {
	this->protocol = protocol;
}

void		HttpResponse::setStatusCode(const std::string &statusCode) {
	this->statusCode = statusCode;
}

const std::string	HttpResponse::getProtocol(void) const {
	return this->protocol;
}

const std::string	HttpResponse::getStatusCode(void) const {
	return this->statusCode;
}

const std::string	HttpResponse::getBody(void) const {
	return this->body;
}

const HttpHeaders	HttpResponse::getHeaders(void) const {
	return this->headers;
}

// Headers Methods
void		HttpResponse::setBody(const std::string &body) {
	this->body = body;
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

	std::string			str(response);
	std::string			find = "";

	// Parse response protocol
	find = ' ';
	this->setProtocol(str.substr(0, str.find(find)));
	str = str.erase(0, str.find(find) + find.length());

	// Parse response status
	find = CRLF;
	this->setStatusCode(str.substr(0, str.find(find)));
	str = str.erase(0, str.find(find) + find.length());

	// Parse response headers
	find = CRLF + CRLF;
	this->headers.fromString(str.substr(0, str.find(find)));
	str = str.erase(0, str.find(find) + find.length());

	// Parse response body
	this->body = str;
}

std::string	HttpResponse::toString(void) const {
	std::string	formatedResponse = "";

	formatedResponse += this->getProtocol() + " " + this->getStatusCode() + CRLF;
	formatedResponse += this->headers.toString() + CRLF;
	formatedResponse += this->body;
	return formatedResponse;
}
