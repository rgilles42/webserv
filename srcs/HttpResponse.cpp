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

HttpResponse::HttpResponse(void) {
	this->initDefaultHeaders();
}

HttpResponse::HttpResponse(const Ressource &ressource) {
	this->initDefaultHeaders();

	this->headers.set("Content-Type", ressource.getContentType());
	this->headers.set("Content-Length", SSTR(ressource.getContent().length()));
	this->body = ressource.getContent();
}

HttpResponse::HttpResponse(const HttpResponse &x) {
	this->protocol = x.protocol;
	this->statusCode = x.statusCode;
	this->headers = x.headers;
	this->body = x.body;
}

HttpResponse::HttpResponse(const std::string &response) {
	this->fromString(response);
}

void		HttpResponse::initDefaultHeaders() {

	this->protocol = "HTTP/1.1";
	this->statusCode = "200 OK";

	this->headers.set("Server", DEFAULT_SERVER);
	this->headers.set("Connection", "keep-alive");

	// this->headers.set("ETag", "5e889dfb-264");

	this->headers.set("Content-Type", "text/plain");
	this->headers.set("Content-Length", "0");

	// this->headers.set("Content-Encoding", "gzip");


	// this->headers.set("Transfer-Encoding", "chunked"); // Cannot be present while Content-Lenght is present
}

// Request Methods
void		HttpResponse::append(const std::string &key, const std::string &value) {
	this->headers.append(key, value);
}

void		HttpResponse::attachment(void) {

}

void		HttpResponse::cookie(const std::string &name, const std::string &value, const std::string &options) {
	(void)name;
	(void)value;
	(void)options;
}

void		HttpResponse::clearCookie(const std::string &name, const std::string &options) {
	(void)name;
	(void)options;
}

void		HttpResponse::download(const std::string &path, const std::string &filename, const std::string &options, const std::string &fn) {
	(void)path;
	(void)filename;
	(void)options;
	(void)fn;
}

void		HttpResponse::end(const std::string &data, const std::string &encoding) {
	(void)data;
	(void)encoding;
}

void		HttpResponse::format(const std::string &object) {
	(void)object;
}

const std::string	HttpResponse::get(const std::string &key) const {
		std::string value = "";
	if (this->headers.has(key)) {
		value = this->headers.get(key);
	}
	return value;
}

void		HttpResponse::json(const std::string &body) {
	this->body = body;
}

void		HttpResponse::links(const std::string &next, const std::string &last) {
	(void)next;
	(void)last;
}

void		HttpResponse::location(const std::string &path) {
	(void)path;
}

void		HttpResponse::redirect(const std::string &path, const std::string &statusCode) {
	(void)path;
	(void)statusCode;
}

void		HttpResponse::send(const std::string &body) {
	(void)body;
}

void		HttpResponse::sendFile(const std::string &path, const std::string &options, const std::string &fn) {
	(void)path;
	(void)options;
	(void)fn;
}

void		HttpResponse::sendStatus(const std::string &statusCode) {
	(void)statusCode;
}

void		HttpResponse::set(const std::string &key, const std::string &value) {
	this->headers.set(key, value);
}

void		HttpResponse::status(const std::string &statusCode) {
	this->statusCode = statusCode;
}

void		HttpResponse::type(const std::string &type) {
	this->headers.set("Content-Type", type);
}

// Utils Methods
void	HttpResponse::fromString(const std::string &response) {

	std::string			str(response);
	std::string			find = "";

	// Parse response protocol
	find = " ";
	this->protocol = str.substr(0, str.find(find));
	str = str.erase(0, str.find(find) + find.length());

	// Parse response status
	find = CRLF;
	this->statusCode = str.substr(0, str.find(find));
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

	formatedResponse += this->protocol + " " + this->statusCode + CRLF;
	formatedResponse += this->headers.toString() + CRLF;
	formatedResponse += this->body;
	return formatedResponse;
}
