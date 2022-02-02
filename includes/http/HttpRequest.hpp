/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 16:45:04 by ppaglier          #+#    #+#             */
/*   Updated: 2022/02/02 18:19:50 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include <string>				// For string
# include <map>					// For map
# include <vector>				// For vector

# include "HttpMethod.hpp"		// For HttpMethod
# include "HttpVersion.hpp"		// For HttpVersion
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
				typedef Webserv::Http::HttpMethod	method_type;
				typedef std::string path_type;
				typedef Webserv::Http::HttpVersion	protocol_type;
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
				HttpRequest(const HttpRequest& other);
				HttpRequest(const std::string& request);
				~HttpRequest();

				HttpRequest&		operator=(const HttpRequest& other);

				void				setMethod(const method_type& method);
				void				setPath(const path_type& path);
				void				setProtocol(const protocol_type& protocol);
				void				setHeaders(const headers_type& headers);
				void				setBody(const body_type& body);

				// Headers Methods
				bool				hasHeader(const std::string& key) const;
				const std::string	getHeader(const std::string& key) const;
				const headers_type&	getHeaders(void) const;

				// Request Properties
				const method_type&	getMethod(void) const;
				const path_type		getBasePath(void) const;
				const path_type&	getFullPath(void) const;
				const protocol_type&	getProtocol(void) const;
				const body_type&	getBody(void) const;

				const MappedValues	getCookies(void) const;
				bool				isFresh(void) const;
				const std::string	getHostname(void) const;
				const std::string	getIp(void) const;
				const ListedValues	getIps(void) const;
				const std::string	getBaseProtocol(void) const;
				const MappedValues	getParams(void) const;
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
				bool				is(const std::string);
				const std::string	param(const std::string& key, const std::string& defaultValue = "") const;

				// Utils Methods
				const std::string	toString(void) const;

				// TODO: Remove because of deprecated
				void				fromString(const std::string& request);

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
				HttpRequestBuilder(const HttpRequestBuilder& other);
				~HttpRequestBuilder();

				HttpRequestBuilder&	operator=(const HttpRequestBuilder& other);

				buffer_type&		getBuffer(void);
				request_list&		getAllRequests(void);

				void				addMessage(const message_type& message);

				size_t				checkBuffer(void) const;
				size_t				checkBuffer(request_list& requests) const;

				bool				parseRequests(void);

		};

	} // namespace Utils

} // namespace Webserv

#endif