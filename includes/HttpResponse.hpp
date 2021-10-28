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

# define LINE_BREAK				std::string("\r\n")

class HttpResponse {

	protected:
		std::string	protocol;
		std::string	statusCode;
		HttpHeaders	headers;
		std::string	content;

	public:
		// Request Methods
		void		setProtocol(const std::string &protocol = HTTP_PROTOCOL);
		void		setStatusCode(const std::string &statusCode = DEFAULT_STATUS_CODE);
		const std::string	getProtocol(void) const;
		const std::string	getStatusCode(void) const;

		// Headers Methods
		void		setContent(const std::string &content);
		void		setContentType(const std::string &contentType = DEFAULT_CONTENT_TYPE);
		void		setContentHash(const uint32_t &contentHash = 0);
		void		setContentLength(const size_t &contentLength = 0);

		// Utils Methods
		void		prepareResponse();
		void		fromString(const std::string &request);
		std::string	toString(void) const;

};

#endif