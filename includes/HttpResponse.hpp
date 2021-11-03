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

#include "Ressource.hpp"
#include "HttpHeaders.hpp"
#include "utils.hpp"

# define HTTP_PROTOCOL			std::string("HTTP/1.1")

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

		void	initDefaultHeaders(void);

	public:
		HttpResponse(void);
		HttpResponse(const Ressource &ressource);
		HttpResponse(const HttpResponse &x);
		HttpResponse(const std::string &response);

		// response Methods
		void				append(const std::string &key, const std::string &value);
		void				attachment(void);
		void				cookie(const std::string &name, const std::string &value, const std::string &options = "");
		void				clearCookie(const std::string &name, const std::string &options = "");
		void				download(const std::string &path, const std::string &filename = "", const std::string &options = "", const std::string &fn = "");
		void				end(const std::string &data = "", const std::string &encoding = "");
		void				format(const std::string &object);
		const std::string	get(const std::string &key) const;
		void				json(const std::string &body = "");
		void				links(const std::string &next = "", const std::string &last = "");
		void				location(const std::string &path);
		void				redirect(const std::string &path, const std::string &statusCode = "320 Found");
		void				send(const std::string &body = "");
		void				sendFile(const std::string &path, const std::string &options = "", const std::string &fn = "");
		void				sendStatus(const std::string &statusCode);
		void				set(const std::string &key, const std::string &value);
		void				status(const std::string &statusCode);
		void				type(const std::string &contentType);


		// Utils Methods
		void		fromString(const std::string &response);
		std::string	toString(void) const;

};

#endif