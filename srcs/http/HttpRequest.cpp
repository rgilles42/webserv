/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 16:45:31 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/08 11:47:05 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http/HttpRequest.hpp"

namespace Webserv {

	namespace Http {
		HttpRequest::HttpRequest(void) {}

		HttpRequest::HttpRequest(const HttpRequest &x) {
			this->method = x.method;
			this->fullPath = x.fullPath;
			this->protocol = x.protocol;
			this->headers = x.headers;
			this->body = x.body;
		}

		HttpRequest::HttpRequest(const std::string &response) {
			this->fromString(response);
		}

		// Request Properties
		const std::string	HttpRequest::getBaseUrl(void) const {
			// Return the origin from with router is provide the req
			size_t find = this->fullPath.find('?');
			if (find != this->fullPath.npos) {
				return fullPath.substr(0, find);
			}
			return this->fullPath;
		}

		const HttpRequest::MappedValuesValid	HttpRequest::getBody(void) const {
			// Return undefined by defaut, need middleware to parse body and get values
			MappedValues values;
			return std::make_pair(!values.empty(), values);
		}

		const HttpRequest::MappedValues	HttpRequest::getCookies(void) const {
			// Return undefined by defaut, need middleware to parse body and get values
			MappedValues cookies;
			return cookies;
		}

		bool				HttpRequest::isFresh(void) const {
			// Return true if the req is cached in the client false otherwise
			return false;
		}

		const std::string	HttpRequest::getHostname(void) const {
			// Return the Host or if trust proxy is enable return X-Forwarded-Host
			std::string hostname = "";
			if (TRUST_PROXY) {
				if (this->headers.has("X-Forwarded-Host")) {
					hostname = this->headers.get("X-Forwarded-Host");
				}
			} else {
				hostname = this->headers.get("Host");
			}
			return hostname;
		}

		const std::string	HttpRequest::getIp(void) const {
			// Return the ip or if trust proxy is enable return left-most entry of X-Forwarded-For
			std::string ip = "";
			if (TRUST_PROXY) {
				if (this->headers.has("X-Forwarded-For")) {
					ip = this->headers.get("X-Forwarded-For");
				}
			} else {
				ip = "TODO";
			}
			return ip;
		}

		const HttpRequest::ListedValues	HttpRequest::getIps(void) const {
			// Return if trust proxy is enable return every entry of X-Forwarded-For
			ListedValues ips;
			if (TRUST_PROXY) {
				if (this->headers.has("X-Forwarded-For")) {
					ips.push_back(this->headers.get("X-Forwarded-For"));
				}
			} else {
				ips.push_back("TODO");
				ips.push_back("TODO");
			}
			return ips;
		}

		const std::string	HttpRequest::getMethod(void) const {
			return this->method;
		}

		const std::string	HttpRequest::getOriginalUrl(void) const {
			// Return the full url ('http://www.example.com/admin/new?sort=desc' => '/admin/new?sort=desc')
			return this->fullPath;
		}

		const HttpRequest::MappedValues	HttpRequest::getParams(void) const {
			// Return all parameters in mapped value (std::map)
			MappedValues params;
			return params;
		}

		const std::string	HttpRequest::getPath(void) const {
			size_t find = this->fullPath.find('?');
			if (find != this->fullPath.npos) {
				return fullPath.substr(0, find);
			}
			return this->fullPath;
		}

		const std::string	HttpRequest::getProtocol(void) const {
			// Return Http or Https (for TLS requests) if trust proxy is enable will return the value of X-Forwarded-Proto
			std::string protocol = "";
			if (TRUST_PROXY) {
				if (this->headers.has("X-Forwarded-Proto")) {
					protocol = this->headers.get("X-Forwarded-Proto");
				}
			} else {
				protocol = "http";
			}
			return protocol;
		}

		const HttpRequest::MappedValues	HttpRequest::getQuery(void) const {
			// Return empty object if no query parser
			MappedValues values;
			values.insert(std::make_pair("TODO", "TODO"));
			return values;
		}

		const std::string	HttpRequest::getRoute(void) const {
			// Return route that contains information on current route (path, method, stack)
			return std::string("TODO");
		}

		bool				HttpRequest::isSecure(void) const {
			return this->getProtocol() == "https";
		}

		const HttpRequest::MappedValues	HttpRequest::getSignedCookies(void) const {
			// Return undefined by defaut, need cookie parser middleware
			MappedValues cookies;
			return cookies;
		}

		bool				HttpRequest::isStale(void) const {
			// Indicates whether the request is “stale,” and is the opposite of req.fresh.
			// TODO
			return false && !this->isFresh();
		}

		const HttpRequest::ListedValues	HttpRequest::getSubdomains(void) const {
			// Return An array of subdomains in the domain name of the request.
			ListedValues subdomains;
			if (!isIpAddress(this->getHostname())) {
				ListedValues splitHost = split(this->getHostname(), ".");
				if (splitHost.size() > 2) {
					subdomains.insert(subdomains.begin(), splitHost.begin(), splitHost.end() - 2);
				}
			}
			return subdomains;
		}

		bool				HttpRequest::isXhr(void) const {
			// Return A Boolean property that is true if the request’s X-Requested-With header field is “XMLHttpRequest”, indicating that the request was issued by a client library such as jQuery.
			if (!this->headers.has("X-Requested-With")) {
				return false;
			}
			return this->headers.get("X-Requested-With") == "“XMLHttpRequest”";
		}

		// Request Methods
		bool				HttpRequest::accepts(const std::string) {
			std::string accepted = "";
			if (this->headers.has("Accept")) {
				accepted = this->headers.get("Accept");
			}
			return true;
		}

		bool				HttpRequest::acceptsCharsets(const std::string) {
			std::string accepted = "";
			if (this->headers.has("Accept-Charset")) {
				accepted = this->headers.get("Accept-Charset");
			}
			return true;
		}

		bool				HttpRequest::acceptsEncodings(const std::string) {
			std::string accepted = "";
			if (this->headers.has("Accept-Encoding")) {
				accepted = this->headers.get("Accept-Encoding");
			}
			return true;
		}

		bool				HttpRequest::acceptsLanguages(const std::string) {
			std::string accepted = "";
			if (this->headers.has("Accept-Language")) {
				accepted = this->headers.get("Accept-Language");
			}
			return true;
		}


		const std::string	HttpRequest::get(const std::string &key) const {
				std::string value = "";
			if (this->headers.has(key)) {
				value = this->headers.get(key);
			}
			return value;
		}

		bool				HttpRequest::is(const std::string) {
			std::string contentType = "";
			if (this->headers.has("Content-Type")) {
				contentType = this->headers.get("Content-Type");
			}
			return false;
		}

		const std::string	HttpRequest::param(const std::string &key, const std::string &defaultValue) const {
			MappedValues params = this->getParams();
			if (params.count(key) > 0) {
				MappedValues::const_iterator it = params.find(key);
				if (it != params.end()) {
					return it->second;
				}
			}
			return defaultValue;
		}

		// Utils Methods
		void	HttpRequest::fromString(const std::string &request) {

			std::string			str(request);
			std::string			find = "";

			// Parse request method
			find = " ";
			this->method = str.substr(0, str.find(find));
			str = str.erase(0, str.find(find) + find.length());

			// Parse request path
			find = " ";
			this->fullPath = str.substr(0, str.find(find));
			str = str.erase(0, str.find(find) + find.length());

			// Parse request protocol
			find = CRLF;
			this->protocol = str.substr(0, str.find(find));
			str = str.erase(0, str.find(find) + find.length());

			// Parse request headers
			find = CRLF + CRLF;
			this->headers.fromString(str.substr(0, str.find(find)));
			str = str.erase(0, str.find(find) + find.length());

			this->body = str;
		}

		std::string	HttpRequest::toString(void) const {
			std::string	formatedRequest = "";

			formatedRequest += this->method + " " + this->fullPath + " " + this->protocol + CRLF;
			formatedRequest += this->headers.toString() + CRLF;
			formatedRequest += this->body;
			return formatedRequest;
		}

	} // namespace Http

} // namespace Webserv
