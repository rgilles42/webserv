/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 16:45:04 by ppaglier          #+#    #+#             */
/*   Updated: 2021/10/27 18:57:45 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

#include <string>
#include <stdint.h>

#include "HttpHeaders.hpp"
#include "utils.hpp"

# define HTTP_PROTOCOL			std::string("HTTP/1.1")
# define DEFAULT_STATUS_CODE	std::string("200 OK")
# define DEFAULT_CONTENT_TYPE	std::string("text/plain")

# define DEFAULT_SERVER			std::string("Webserv/1.0.0")

# define CR						std::string("\r")
# define LF						std::string("\n")
# define CRLF					(CR + LF)

class HttpResponse {

	protected:
		std::string	protocol;
		std::string	statusCode;
		HttpHeaders	headers;
		std::string	body;

	public:
		HttpResponse(void);
		HttpResponse(const HttpResponse &x);
		HttpResponse(const std::string &response);

		// response Methods
		void		setProtocol(const std::string &protocol = HTTP_PROTOCOL);
		void		setStatusCode(const std::string &statusCode = DEFAULT_STATUS_CODE);
		void		setBody(const std::string &body);
		const std::string	getProtocol(void) const;
		const std::string	getStatusCode(void) const;
		const std::string	getBody(void) const;
		const HttpHeaders	getHeaders(void) const;

		// Headers Methods
		void		setContent(const std::string &content);
		void		setContentType(const std::string &contentType = DEFAULT_CONTENT_TYPE);
		void		setContentHash(const uint32_t &contentHash = 0);
		void		setContentLength(const size_t &contentLength = 0);

		// Utils Methods
		void		prepareResponse();
		void		fromString(const std::string &response);
		std::string	toString(void) const;

};

#endif