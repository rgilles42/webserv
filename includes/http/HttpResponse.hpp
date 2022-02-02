/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 16:45:04 by ppaglier          #+#    #+#             */
/*   Updated: 2022/02/02 16:38:05 by ppaglier         ###   ########.fr       */
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

				void	initDefaultHeaders(void);

			public:
				HttpResponse(void);
				HttpResponse(const HttpResponse& other);
				HttpResponse(const resource_type &resource);
				HttpResponse(const std::string &response);
				~HttpResponse();

				HttpResponse&		operator=(const HttpResponse& other);

				// response Methods
				void				append(const std::string& key, const std::string& value);
				void				attachment(const std::string& filename = "");
				void				cookie(const std::string& name, const std::string& value, const std::string& options = "");
				void				clearCookie(const std::string& name, const std::string& options = "");
				void				download(const std::string& path, const std::string& filename = "", const std::string& options = "");
				const std::string	get(const std::string& key) const;
				void				links(const std::string& next = "", const std::string& last = "");
				void				location(const std::string& path);
				void				redirect(const std::string& path, const status_code_type& statusCode = status_code_type::redirection_found);
				void				send(const std::string& body = "");
				void				sendFile(const std::string& path, const std::string& options = "");
				void				sendStatus(const status_code_type& statusCode);
				void				set(const std::string& key, const std::string& value);
				void				status(const status_code_type& statusCode);
				void				type(const std::string& contentType);


				// Utils Methods
				void		fromString(const std::string& response);
				std::string	toString(void) const;

		};

	} // namespace Utils

} // namespace Webserv

#endif
