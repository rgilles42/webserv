/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 16:45:04 by ppaglier          #+#    #+#             */
/*   Updated: 2021/10/27 18:57:45 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

#include <string>
#include <stdint.h>
#include <map>

#include <iostream>
#include "utils.hpp"

# define HTTP_PROTOCOL			std::string("HTTP/1.1")
# define DEFAULT_STATUS_CODE	std::string("200 OK")
# define DEFAULT_CONTENT_TYPE	std::string("text/plain")

# define DEFAULT_SERVER			std::string("Webserv/1.0.0")

# define LINE_BREAK				std::string("\r\n")
# define AUTHORIZED_CHAR		std::string("!#$%&'*+-.^_`|~")

# define IS_AUTHORIZED_CHAR( c ) (std::isalpha(c) || std::isdigit(c) || AUTHORIZED_CHAR.find(c))

class HttpRequest {

	protected:
		std::string							method;
		std::string							path;
		std::string							protocol;
		std::string							statusCode;
		std::map<std::string, std::string>	headers;
		std::string							content;

	public:
		// Request Methods
		void		setMethod(const std::string &method);
		void		setPath(const std::string &path);
		void		setProtocol(const std::string &protocol = HTTP_PROTOCOL);
		void		setStatusCode(const std::string &statusCode = DEFAULT_STATUS_CODE);
		const std::string	getMethod(void) const;
		const std::string	getPath(void) const;
		const std::string	getProtocol(void) const;
		const std::string	getStatusCode(void) const;

		// Headers Methods
		bool		isHeaderKeyValid(const std::string &key);
		void		setHeaderKeyValue(const std::string &key, const std::string &value);
		const std::string	getHeaderKeyValue(const std::string &key) const;
		void		setContentType(const std::string &contentType = DEFAULT_CONTENT_TYPE);
		void		setContentHash(const uint32_t &contentHash = 0);
		void		setContentLength(const size_t &contentLength = 0);

		// Utils Methods
		void		prepareRequest();
		std::string	formatToString(void) const;
		void		formatFromString(const std::string &request);

};

#endif