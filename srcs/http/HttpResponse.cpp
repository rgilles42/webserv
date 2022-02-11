/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 16:45:31 by ppaglier          #+#    #+#             */
/*   Updated: 2022/02/11 22:54:42 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/http/HttpResponse.hpp"

namespace Webserv {

	namespace Http {

		HttpResponse::HttpResponse(void) {
			this->initDefaultHeaders();
		}

		HttpResponse::HttpResponse(const HttpResponse& other) {
			*this = other;
		}

		HttpResponse::~HttpResponse() {}

		HttpResponse&		HttpResponse::operator=(const HttpResponse& other) {
			if (this != &other) {
				this->protocol = other.protocol;
				this->statusCode = other.statusCode;
				this->headers = other.headers;
				this->body = other.body;
			}
			return *this;
		}

		void		HttpResponse::initDefaultHeaders() {

			this->protocol = protocol_type::http_1_1;
			this->statusCode = status_code_type::success_ok;

			this->headers.clear();

			this->setHeader("Server", DEFAULT_SERVER);
			this->setHeader("Date", Webserv::Utils::getFormatedDate(time(NULL)));

			// this->setHeader("Connection", "keep-alive");

			// this->setHeader("ETag", "5e889dfb-264");

			// this->setHeader("Content-Type", "text/plain");
			this->setHeader("Content-Length", "0");

			// this->setHeader("Content-Encoding", "gzip");


			// this->setHeader("Transfer-Encoding", "chunked"); // Cannot be present while Content-Length is present
		}

		// Headers Methods
		void		HttpResponse::appendHeader(const headers_type::key_type& key, const headers_type::value_type& value) {
			this->headers.append(key, value);
		}

		void		HttpResponse::setHeader(const headers_type::key_type& key, const headers_type::value_type& value) {
			this->headers.set(key, value);
		}

		const HttpResponse::headers_type::value_type	HttpResponse::getHeader(const headers_type::key_type& key) const {
				std::string value = "";
			if (this->headers.has(key)) {
				value = this->headers.get(key);
			}
			return value;
		}

		const HttpResponse::headers_type&			HttpResponse::getHeaders(void) const {
			return this->headers;
		}

		// Response Methods

		void		HttpResponse::setStatusCode(const status_code_type& statusCode) {
			this->statusCode = statusCode;
		}

		void		HttpResponse::setProtocol(const protocol_type& protocol) {
			this->protocol = protocol;
		}

		void		HttpResponse::setBody(const body_type& body) {
			this->body = body;
			this->setHeader("Content-Length", SSTR(body.length()));
		}

		const HttpResponse::status_code_type&		HttpResponse::getStatusCode(void) const {
			return this->statusCode;
		}

		const HttpResponse::protocol_type&			HttpResponse::getProtocol(void) const {
			return this->protocol;
		}

		const HttpResponse::body_type&				HttpResponse::getBody(void) const {
			return this->body;
		}

		void		HttpResponse::setRedirect(const std::string& path, const status_code_type& statusCode) {
			this->initDefaultHeaders();
			this->setHeader("Location", path);
			this->setStatusCode(statusCode);
		}

		void		HttpResponse::setResource(const resource_type& resource, const status_code_type& statusCode) {
			this->initDefaultHeaders();
			this->setBody(resource.getContent());
			this->setHeader("Content-Type", resource.getContentType());
			if (resource.isCGI()) {
				file_parser_type fileParser;
				fileParser.parseFile(resource.getContent());
				file_parser_type::headers_type::const_iterator it = fileParser.getHeaders().begin();
				while (it != fileParser.getHeaders().end()) {
					if (this->headers.isKeyValid(it->first)) {
						this->setHeader(it->first, it->second);
					}
					it++;
				}
				this->setBody(fileParser.getBody());
			}
			this->setStatusCode(statusCode);
		}


		// Utils Methods
		std::string	HttpResponse::toString(void) const {
			std::string	formatedResponse = "";

			formatedResponse += this->protocol.toString() + " " + this->statusCode.toString() + CRLF;
			formatedResponse += this->headers.toString() + CRLF;
			formatedResponse += this->body;
			return formatedResponse;
		}

	} // namespace Http

} // namespace Webserv
