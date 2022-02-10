/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgilles <rgilles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 16:45:04 by ppaglier          #+#    #+#             */
/*   Updated: 2022/02/10 12:36:23 by rgilles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include <string>						// For string

# include "HttpVersion.hpp"				// For HttpVersion
# include "HttpStatusCode.hpp"			// For HttpStatusCode
# include "HttpHeaders.hpp"				// For HttpHeaders
# include "../utils/Resource.hpp"		// For Resource
# include "../utils/common.hpp"			// For trim

# define HTTP_PROTOCOL			std::string("HTTP/1.1")

# define DEFAULT_SERVER			std::string("Webserv/1.0.0")

# define CR						std::string("\r")
# define LF						std::string("\n")
# define CRLF					(CR + LF)

namespace Webserv {

	namespace Http {

		class HttpResponse {
			public:
				typedef Webserv::Http::HttpVersion	protocol_type;
				typedef Webserv::Http::HttpStatusCode	status_code_type;
				typedef Webserv::Http::HttpHeaders	headers_type;
				typedef Webserv::Utils::Resource	resource_type;
				typedef std::string					body_type;

			protected:
				protocol_type		protocol;
				status_code_type	statusCode;
				headers_type		headers;
				body_type			body;
				body_type			raw_headers;

				void	initDefaultHeaders(void);

			public:
				HttpResponse(void);
				HttpResponse(const HttpResponse& other);
				~HttpResponse();

				HttpResponse&	operator=(const HttpResponse& other);

				// Headers Methods
				void			appendHeader(const headers_type::key_type& key, const headers_type::value_type& value);
				void			setHeader(const headers_type::key_type& key, const headers_type::value_type& value);
				const headers_type::value_type	getHeader(const headers_type::key_type& key) const;
				const headers_type&	getHeaders(void) const;

				// Response Methods

				void			setStatusCode(const status_code_type& statusCode);
				void			setProtocol(const protocol_type& protocol);
				void			setBody(const body_type& body);

				const status_code_type&	getStatusCode(void) const;
				const protocol_type&	getProtocol(void) const;
				const body_type&		getBody(void) const;

				void			setRedirect(const std::string& path, const status_code_type& statusCode = status_code_type::redirection_found);
				void			setResource(const resource_type& resource, const status_code_type& statusCode = status_code_type::success_ok);

				// Utils Methods
				std::string	toString(void) const;

				// TODO: Remove because of deprecated
				void		fromString(const std::string& response);
		};

	} // namespace Utils

} // namespace Webserv

#endif
