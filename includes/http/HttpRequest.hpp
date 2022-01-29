/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 16:45:04 by ppaglier          #+#    #+#             */
/*   Updated: 2022/01/28 21:06:31 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include <string>				// For string
# include <map>					// For map
# include <vector>				// For vector

# include "HttpHeaders.hpp"		// For HttpHeaders
# include "../utils/common.hpp"	// For trim

# define HTTP_PROTOCOL			std::string("HTTP/1.1")

# define CR						std::string("\r")
# define LF						std::string("\n")
# define CRLF					(CR + LF)

# define TRUST_PROXY			false

namespace Webserv {

	namespace Http {

		class HttpRequest {
			public:
				typedef std::string method_type;
				typedef std::string path_type;
				typedef std::string protocol_type;
				typedef Webserv::Http::HttpHeaders	headers_type;
				typedef std::string body_type;

				typedef	std::vector<std::string>			ListedValues;
				typedef std::map<std::string, std::string>	MappedValues;
				typedef std::pair<bool, MappedValues >		MappedValuesValid;

			protected:
				method_type		method;
				path_type		fullPath;
				protocol_type	protocol;
				headers_type	headers;
				body_type		body;

			public:
				HttpRequest(void);
				HttpRequest(const HttpRequest& x);
				HttpRequest(const std::string& request);

				void				setMethod(const method_type& method);
				void				setPath(const path_type& path);
				void				setProtocol(const protocol_type& protocol);
				void				setHeaders(const headers_type& headers);
				void				setBody(const body_type& body);

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
				const std::string	get(const std::string& key) const;
				bool				is(const std::string);
				const std::string	param(const std::string& key, const std::string& defaultValue = "") const;



				// Utils Methods
				void				fromString(const std::string& request);
				const std::string	toString(void) const;

		};


		class HttpRequestBuilder {
			public:
				typedef HttpRequest					request_type;
				typedef request_type::headers_type	headers_type;
				typedef Webserv::Http::HttpHeadersBuilder	headers_builder_type;
				typedef std::vector<request_type>	request_list;
				typedef std::string					buffer_type;
				typedef std::string					message_type;

			protected:
				buffer_type		buffer;
				request_list	requests;

			public:
				HttpRequestBuilder(void);
				HttpRequestBuilder(const HttpRequestBuilder& x);

				buffer_type&	getBuffer(void);
				request_list&	getAllRequests(void);

				void				addMessage(const message_type& message);

				size_t				checkBuffer(void) const;
				size_t				checkBuffer(request_list& requests) const;

				bool				parseRequests(void);

		};

	} // namespace Utils

} // namespace Webserv

#endif