/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 16:45:31 by ppaglier          #+#    #+#             */
/*   Updated: 2021/10/28 20:26:02 by ppaglier         ###   ########.fr       */
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

const std::string   HttpRequest::getMethod(void) const {
	return this->method;
}

const std::string   HttpRequest::getPath(void) const {
	return this->path;
}

const std::string   HttpRequest::getProtocol(void) const {
	return this->protocol;
}

// Utils Methods
void	HttpRequest::fromString(const std::string &request) {

	std::string			line(request);

	// Parse request method
	this->setMethod(line.substr(0, line.find(' ')));
	line = line.erase(0, line.find(' ') + 1);

	// Parse request path
	this->setPath(line.substr(0, line.find(' ')));
	line = line.erase(0, line.find(' ') + 1);

	// Parse request protocol
	this->setProtocol(line.substr(0, line.find(LINE_BREAK)));
	line = line.erase(0, line.find(LINE_BREAK) + LINE_BREAK.length());

	// Parse request headers
	this->headers.fromString(line.substr(0, line.find(LINE_BREAK + LINE_BREAK)));
	line = line.erase(0, line.find(LINE_BREAK + LINE_BREAK) + (LINE_BREAK + LINE_BREAK).length());
}

std::string	HttpRequest::toString(void) const {
	std::string	formatedRequest = "";

	formatedRequest += this->getMethod() + " " + this->getPath() + " " + this->getProtocol() + LINE_BREAK;
	formatedRequest += this->headers.toString() + LINE_BREAK;
	return formatedRequest;
}
