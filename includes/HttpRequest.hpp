/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 16:45:04 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/03 01:30:50 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

#include <string>

#include "HttpHeaders.hpp"
#include "utils.hpp"

# define HTTP_PROTOCOL			std::string("HTTP/1.1")

# define CR						std::string("\r")
# define LF						std::string("\n")
# define CRLF					(CR + LF)

# define TRUST_PROXY			false

class HttpRequest {

	protected:
		std::string	method;
		std::string	fullPath;
		std::string	protocol;
		HttpHeaders	headers;
		std::string	body;

	public:
		HttpRequest(void);
		HttpRequest(const HttpRequest &x);
		HttpRequest(const std::string &request);

		// Request Properties
		const std::string	getBaseUrl(void) const;
		const std::string	getBody(void) const;
		const std::string	getCookies(void) const;
		bool				isFresh(void) const;
		const std::string	getHostname(void) const;
		const std::string	getIp(void) const;
		const std::string	getIps(void) const;
		const std::string	getMethod(void) const;
		const std::string	getOriginalUrl(void) const;
		const std::string	getParams(void) const;
		const std::string	getPath(void) const;
		const std::string	getProtocol(void) const;
		const std::string	getQuery(void) const;
		const std::string	getRoute(void) const;
		bool				isSecure(void) const;
		const std::string	getSignedCookies(void) const;
		bool				isStale(void) const;
		const std::string	getSubdomains(void) const;
		bool				isXhr(void) const;


		// Request Methods
		bool				accepts(const std::string);
		bool				acceptsCharsets(const std::string);
		bool				acceptsEncodings(const std::string);
		bool				acceptsLanguages(const std::string);
		const std::string	get(const std::string &key) const;
		bool				is(const std::string);
		const std::string	param(const std::string &key, const std::string &defaultValue = "") const;



		// Utils Methods
		void		fromString(const std::string &request);
		std::string	toString(void) const;

};

#endif