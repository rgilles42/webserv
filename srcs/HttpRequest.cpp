/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 16:45:31 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/03 01:30:54 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/HttpRequest.hpp"

HttpRequest::HttpRequest(void) {}

HttpRequest::HttpRequest(const HttpRequest &x) {
	this->method = x.method;
	this->fullPath = x.fullPath;
	this->protocol = x.protocol;
	this->headers = x.headers;
	this->body = x.body;
}

HttpRequest::HttpRequest(const std::string &response) {
	this->fromString(response);
}

// Request Properties
const std::string	HttpRequest::getBaseUrl(void) const {
	// Return the origin from with router is provide the req
	return std::string("To do");
}

const std::string	HttpRequest::getBody(void) const {
	// Return undefined by defaut, need middleware to parse body and get values
	return std::string("To do");
}

const std::string	HttpRequest::getCookies(void) const {
	// Return undefined by defaut, need cookie parser middleware
	return std::string("To do");
}

bool				HttpRequest::isFresh(void) const {
	// Return true if the req is cached in the client false otherwise
	return false;
}

const std::string	HttpRequest::getHostname(void) const {
	// Return the Host or if trust proxy is enable return X-Forwarded-Host
	if (TRUST_PROXY) {
		return this->headers.get("X-Forwarded-Host");
	}
	return this->headers.get("Host");
}

const std::string	HttpRequest::getIp(void) const {
	// Return the ip or if trust proxy is enable return left-most entry of X-Forwarded-For
	if (TRUST_PROXY) {
		return this->headers.get("X-Forwarded-For");
	}
	return std::string("To do");
}

const std::string	HttpRequest::getIps(void) const {
	// Return if trust proxy is enable return every entry of X-Forwarded-For
	if (TRUST_PROXY) {
		return this->headers.get("X-Forwarded-For");
	}
	return std::string("To do");
}

const std::string	HttpRequest::getMethod(void) const {
	return this->method;
}

const std::string	HttpRequest::getOriginalUrl(void) const {
	// Return the full url ('http://www.example.com/admin/new?sort=desc' => '/admin/new?sort=desc')
	return this->fullPath;
}

const std::string	HttpRequest::getParams(void) const {
	// Return all parameters in mapped value (std::map)
	return std::string("To do");
}

const std::string	HttpRequest::getPath(void) const {
	size_t find = this->fullPath.find('?');
	if (find != this->fullPath.npos) {
		return fullPath.substr(0, find);
	}
	return this->fullPath;
}

const std::string	HttpRequest::getProtocol(void) const {
	// Return Http or Https (for TLS requests) if trust proxy is enable will return the value of X-Forwarded-Proto
	// return this->protocol;
	if (TRUST_PROXY) {
		return this->headers.get("X-Forwarded-Proto");
	}
	return std::string("To do");
}

const std::string	HttpRequest::getQuery(void) const {
	// Return empty object if no query parser
	return std::string("To do");
}

const std::string	HttpRequest::getRoute(void) const {
	// Return route that contains information on current route (path, method, stack)
	return std::string("To do");
}

bool				HttpRequest::isSecure(void) const {
	return this->getProtocol() == "https";
}

const std::string	HttpRequest::getSignedCookies(void) const {
	// Return undefined by defaut, need cookie parser middleware
	return std::string("To do");
}

bool				HttpRequest::isStale(void) const {
	// Indicates whether the request is “stale,” and is the opposite of req.fresh.
	return false && !this->isFresh();
}

const std::string	HttpRequest::getSubdomains(void) const {
	// Return An array of subdomains in the domain name of the request.
	return std::string("To do");
}

bool				HttpRequest::isXhr(void) const {
	// Return A Boolean property that is true if the request’s X-Requested-With header field is “XMLHttpRequest”, indicating that the request was issued by a client library such as jQuery.
	return this->headers.get("X-Requested-With") == "“XMLHttpRequest”";
}

// Request Methods


bool				HttpRequest::accepts(const std::string) {
	return false;
}

bool				HttpRequest::acceptsCharsets(const std::string) {
	return false;
}

bool				HttpRequest::acceptsEncodings(const std::string) {
	return false;
}

bool				HttpRequest::acceptsLanguages(const std::string) {
	return false;
}


const std::string	HttpRequest::get(const std::string &key) const {
	return this->headers.get(key);
}

bool				HttpRequest::is(const std::string) {
	return false;
}

const std::string	HttpRequest::param(const std::string &key, const std::string &defaultValue) const {
	(void)key;
	return defaultValue;
}

// Utils Methods
void	HttpRequest::fromString(const std::string &request) {

	std::string			str(request);
	std::string			find = "";

	// Parse request method
	find = " ";
	this->method = str.substr(0, str.find(find));
	str = str.erase(0, str.find(find) + find.length());

	// Parse request path
	find = " ";
	this->fullPath = str.substr(0, str.find(find));
	str = str.erase(0, str.find(find) + find.length());

	// Parse request protocol
	find = CRLF;
	this->protocol = str.substr(0, str.find(find));
	str = str.erase(0, str.find(find) + find.length());

	// Parse request headers
	find = CRLF + CRLF;
	this->headers.fromString(str.substr(0, str.find(find)));
	str = str.erase(0, str.find(find) + find.length());

	this->body = str;
}

std::string	HttpRequest::toString(void) const {
	std::string	formatedRequest = "";

	formatedRequest += this->method + " " + this->fullPath + " " + this->protocol + CRLF;
	formatedRequest += this->headers.toString() + CRLF;
	formatedRequest += this->body;
	return formatedRequest;
}
