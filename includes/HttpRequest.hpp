/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 16:45:04 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/02 02:18:21 by ppaglier         ###   ########.fr       */
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

class HttpRequest {

	protected:
		std::string	method;
		std::string	path;
		std::string	protocol;
		HttpHeaders	headers;
		std::string	body;

	public:
		HttpRequest(void);
		HttpRequest(const HttpRequest &x);
		HttpRequest(const std::string &request);

		// Request Methods
		void		setMethod(const std::string &method);
		void		setPath(const std::string &path);
		void		setProtocol(const std::string &protocol = HTTP_PROTOCOL);
		void		setBody(const std::string &body);
		const std::string	getMethod(void) const;
		const std::string	getPath(void) const;
		const std::string	getProtocol(void) const;
		const std::string	getBody(void) const;
		const HttpHeaders	getHeaders(void) const;

		// Utils Methods
		void		fromString(const std::string &request);
		std::string	toString(void) const;

};

#endif