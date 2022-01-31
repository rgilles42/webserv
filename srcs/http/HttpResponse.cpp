/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 16:45:31 by ppaglier          #+#    #+#             */
/*   Updated: 2021/10/28 18:07:49 by ppaglier         ###   ########.fr       */
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

		HttpResponse::HttpResponse(const resource_type& resource) {
			this->initDefaultHeaders();

			this->headers.set("Content-Type", resource.getContentType());
			this->headers.set("Content-Length", SSTR(resource.getContent().length()));
			this->body = resource.getContent();
		}

		HttpResponse::HttpResponse(const std::string& response) {
			this->fromString(response);
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

			this->protocol = "HTTP/1.1";
			this->statusCode = "200 OK";

			this->headers.set("Server", DEFAULT_SERVER);
			this->headers.set("Connection", "keep-alive");

			// this->headers.set("ETag", "5e889dfb-264");

			this->headers.set("Content-Type", "text/plain");
			this->headers.set("Content-Length", "0");

			// this->headers.set("Content-Encoding", "gzip");


			// this->headers.set("Transfer-Encoding", "chunked"); // Cannot be present while Content-Lenght is present
		}

		// Request Methods
		void		HttpResponse::append(const std::string& key, const std::string& value) {
			this->headers.append(key, value);
		}

		void		HttpResponse::attachment(const std::string& file) {
			if (file.length() <= 0) {
				this->headers.set("Content-Disposition", "attachement");
				return ;
			}
			// std::string filetype = Webserv::Utils::getContentTypeByFile(file, "");
			// std::string filename = file.substr(file.find_last_of("/") + 1);

			// this->headers.set("Content-Disposition", "attachement; filename=\"" + filename + "\"");
			// this->type(filetype);
		}

		void		HttpResponse::cookie(const std::string& name, const std::string& value, const std::string& options) {
			(void)name;
			(void)value;
			(void)options;
		}

		void		HttpResponse::clearCookie(const std::string& name, const std::string& options) {
			(void)name;
			(void)options;
		}

		void		HttpResponse::download(const std::string& path, const std::string& filename, const std::string& options) {
			if (filename.length() <= 0) {
				this->headers.set("Content-Disposition", "attachement; filename=\"" + path + "\"");
			} else {
				this->headers.set("Content-Disposition", "attachement; filename=\"" + filename + "\"");
			}
			this->sendFile(path, options);
		}

		const std::string	HttpResponse::get(const std::string& key) const {
				std::string value = "";
			if (this->headers.has(key)) {
				value = this->headers.get(key);
			}
			return value;
		}

		void		HttpResponse::links(const std::string& next, const std::string& last) {
			if (next.length() > 0) {
				this->headers.append("Link", "<"+ next +">; rel=\"next\"");
			}
			if (last.length() > 0) {
				this->headers.append("Link", "<"+ last +">; rel=\"last\"");
			}
		}

		void		HttpResponse::location(const std::string& path) {
			this->headers.set("Location", path);
		}

		void		HttpResponse::redirect(const std::string& path, const std::string& statusCode) {
			(void)path;
			(void)statusCode;
		}

		void		HttpResponse::send(const std::string& body) {
			(void)body;
		}

		void		HttpResponse::sendFile(const std::string& path, const std::string& options) {
			(void)path;
			// std::string filetype = Webserv::Utils::getContentTypeByFile(path, "");
			// this->type(filetype);
			(void)options;
		}

		void		HttpResponse::sendStatus(const std::string& statusCode) {
			this->status(statusCode);
		}

		void		HttpResponse::set(const std::string& key, const std::string& value) {
			this->headers.set(key, value);
		}

		void		HttpResponse::status(const std::string& statusCode) {
			this->statusCode = statusCode;
		}

		void		HttpResponse::type(const std::string& type) {
			this->headers.set("Content-Type", type);
		}

		// Utils Methods
		void	HttpResponse::fromString(const std::string& response) {

			std::string			str(response);
			std::string			find = "";

			// Parse response protocol
			find = " ";
			this->protocol = str.substr(0, str.find(find));
			str = str.erase(0, str.find(find) + find.length());

			// Parse response status
			find = CRLF;
			this->statusCode = str.substr(0, str.find(find));
			str = str.erase(0, str.find(find) + find.length());

			// Parse response headers
			find = CRLF + CRLF;
			this->headers.fromString(str.substr(0, str.find(find)));
			str = str.erase(0, str.find(find) + find.length());

			// Parse response body
			this->body = str;
		}

		std::string	HttpResponse::toString(void) const {
			std::string	formatedResponse = "";

			formatedResponse += this->protocol + " " + this->statusCode + CRLF;
			formatedResponse += this->headers.toString() + CRLF;
			formatedResponse += this->body;
			return formatedResponse;
		}

	} // namespace Http

} // namespace Webserv
