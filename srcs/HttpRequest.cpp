/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 16:45:31 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/01 18:20:36 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/HttpRequest.hpp"

HttpRequest::HttpRequest(void) {}

HttpRequest::HttpRequest(const HttpRequest &x) {
	this->method = x.getMethod();
	this->path = x.getPath();
	this->protocol = x.getProtocol();
	this->headers = x.getHeaders();
	this->body = x.getBody();
}

HttpRequest::HttpRequest(const std::string &response) {
	this->fromString(response);
}

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

void		HttpRequest::setBody(const std::string &body) {
	this->body = body;
}

const std::string	HttpRequest::getMethod(void) const {
	return this->method;
}

const std::string	HttpRequest::getPath(void) const {
	return this->path;
}

const std::string	HttpRequest::getProtocol(void) const {
	return this->protocol;
}

const std::string	HttpRequest::getBody(void) const {
	return this->body;
}

const HttpHeaders	HttpRequest::getHeaders(void) const {
	return this->headers;
}

// Utils Methods
void	HttpRequest::fromString(const std::string &request) {

	std::string			str(request);

	// Parse request method
	this->setMethod(str.substr(0, str.find(' ')));
	str = str.erase(0, str.find(' ') + 1);

	// Parse request path
	this->setPath(str.substr(0, str.find(' ')));
	str = str.erase(0, str.find(' ') + 1);

	// Parse request protocol
	this->setProtocol(str.substr(0, str.find(LINE_BREAK)));
	str = str.erase(0, str.find(LINE_BREAK) + LINE_BREAK.length());

	// Parse request headers
	this->headers.fromString(str.substr(0, str.find(LINE_BREAK + LINE_BREAK)));
	str = str.erase(0, str.find(LINE_BREAK + LINE_BREAK) + (LINE_BREAK + LINE_BREAK).length());

	this->body = str;
}

std::string	HttpRequest::toString(void) const {
	std::string	formatedRequest = "";

	formatedRequest += this->getMethod() + " " + this->getPath() + " " + this->getProtocol() + LINE_BREAK;
	formatedRequest += this->headers.toString() + LINE_BREAK;
	formatedRequest += this->body;
	return formatedRequest;
}
