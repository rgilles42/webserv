/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 16:45:04 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/03 18:26:13 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

#include <string>
#include <map>
#include <vector>

#include "HttpHeaders.hpp"
#include "utils.hpp"

# define HTTP_PROTOCOL			std::string("HTTP/1.1")

# define CR						std::string("\r")
# define LF						std::string("\n")
# define CRLF					(CR + LF)

# define TRUST_PROXY			false

class HttpRequest {

	public:
		typedef	std::vector<std::string>			ListedValues;
		typedef std::map<std::string, std::string>	MappedValues;
		typedef std::pair<bool, MappedValues >		MappedValuesValid;

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
		const MappedValuesValid	getBody(void) const;
		const MappedValues	getCookies(void) const;
		bool				isFresh(void) const;
		const std::string	getHostname(void) const;
		const std::string	getIp(void) const;
		const ListedValues	getIps(void) const;
		const std::string	getMethod(void) const;
		const std::string	getOriginalUrl(void) const;
		const MappedValues	getParams(void) const;
		const std::string	getPath(void) const;
		const std::string	getProtocol(void) const;
		const MappedValues	getQuery(void) const;
		const std::string	getRoute(void) const;
		bool				isSecure(void) const;
		const MappedValues	getSignedCookies(void) const;
		bool				isStale(void) const;
		const ListedValues	getSubdomains(void) const;
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