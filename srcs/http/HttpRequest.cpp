/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 16:45:31 by ppaglier          #+#    #+#             */
/*   Updated: 2022/01/18 10:21:35 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http/HttpRequest.hpp"

namespace Webserv {

	namespace Http {

		HttpRequest::HttpRequest(void) {}

		HttpRequest::HttpRequest(const HttpRequest& x) {
			this->method = x.method;
			this->fullPath = x.fullPath;
			this->protocol = x.protocol;
			this->headers = x.headers;
			this->body = x.body;
		}

		HttpRequest::HttpRequest(const std::string& response) {
			this->fromString(response);
		}



		void				HttpRequest::setMethod(const method_type& method) {
			this->method = method;
		}

		void				HttpRequest::setPath(const path_type& path) {
			this->fullPath = path;
		}

		void				HttpRequest::setProtocol(const protocol_type& protocol) {
			this->protocol = protocol;
		}

		void				HttpRequest::setHeaders(const headers_type& headers) {
			this->headers = headers;
		}

		void				HttpRequest::setBody(const body_type& body) {
			this->body = body;
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
			// if (!isIpAddress(this->getHostname())) {
			// 	ListedValues splitHost = split(this->getHostname(), ".");
			// 	if (splitHost.size() > 2) {
			// 		subdomains.insert(subdomains.begin(), splitHost.begin(), splitHost.end() - 2);
			// 	}
			// }
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


		const std::string	HttpRequest::get(const std::string& key) const {
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

		const std::string	HttpRequest::param(const std::string& key, const std::string& defaultValue) const {
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
		void	HttpRequest::fromString(const std::string& request) {

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

		const std::string	HttpRequest::toString(void) const {
			std::string	formatedRequest = "";

			formatedRequest += this->method + " " + this->fullPath + " " + this->protocol + CRLF;
			formatedRequest += this->headers.toString() + CRLF;
			formatedRequest += this->body;
			return formatedRequest;
		}


		HttpRequestBuilder::HttpRequestBuilder(void) {}
		HttpRequestBuilder::HttpRequestBuilder(const HttpRequestBuilder& x) {
			this->buffer = x.buffer;
			this->requests = x.requests;
		}

		HttpRequestBuilder::buffer_type&	HttpRequestBuilder::getBuffer(void) {
			return this->buffer;
		}

		HttpRequestBuilder::request_list&	HttpRequestBuilder::getAllRequests(void) {
			return this->requests;
		}

		void									HttpRequestBuilder::addMessage(const message_type& message) {
			this->buffer.append(message);
		}

		size_t									HttpRequestBuilder::checkBuffer(void) const {
			request_list list;
			return this->checkBuffer(list);
		}

		size_t									HttpRequestBuilder::checkBuffer(request_list& requests) const {
			size_t		lastPos = 0;
			buffer_type tmpBuff = this->buffer;
			size_t		pos = 0;
			buffer_type::iterator it_find;
			size_t		find = 0;

			while (pos <= tmpBuff.length()) {
				request_type request;

				// skip whitespace (i'm not sure)
				it_find = find_if(tmpBuff.begin() + pos, tmpBuff.end(), std::not1(std::ptr_fun<int, int>(std::isspace)));
				pos = it_find - tmpBuff.begin();
				if (tmpBuff.length() <= pos) {
					return lastPos;
				}

				// Start-line

				// Position of method (between pos & find)
				it_find = find_if(tmpBuff.begin() + pos, tmpBuff.end(), std::ptr_fun<int, int>(std::isspace));
				find = it_find - tmpBuff.begin();
				if (tmpBuff.length() <= find || pos == find) {
					return lastPos;
				}
				request.setMethod(tmpBuff.substr(pos, find - pos));
				pos = find + 1;

				// Position of uri (between pos & find)
				it_find = find_if(tmpBuff.begin() + pos, tmpBuff.end(), std::ptr_fun<int, int>(std::isspace));
				find = it_find - tmpBuff.begin();
				if (tmpBuff.length() <= find || pos == find) {
					return lastPos;
				}
				request.setPath(tmpBuff.substr(pos, find - pos));
				pos = find;

				// For simple request, there's not protocol.. think about it TODO: check this out
				if (true) {
					pos++;
					// Position of protocol (between pos & find)
					it_find = find_if(tmpBuff.begin() + pos, tmpBuff.end(), std::ptr_fun<int, int>(std::isspace));
					find = it_find - tmpBuff.begin();
					if (tmpBuff.length() <= find || pos == find) {
						return lastPos;
					}
					request.setProtocol(tmpBuff.substr(pos, find - pos));
					pos = find;
				}

				// Position of CRLF (end of Start-Line)
				find = tmpBuff.find(CRLF, pos);
				if (find == tmpBuff.npos || find != pos) {
					return lastPos;
				}
				pos = find + 2;

				// Headers

				headers_builder_type	headerBuilder;

				headerBuilder.addMessage(tmpBuff.substr(pos));
				find = headerBuilder.checkBuffer();
				if (find == tmpBuff.npos) {
					return lastPos;
				}
				headerBuilder.parseHeaders();
				headers_type	headers = headerBuilder.getHeaders();
				request.setHeaders(headers);
				pos += find;

				// Message-Body
				if (headers.has("Transfer-Encoding")) {
					if (headers.get("Transfer-Encoding") != "chunked") {
						return lastPos;
					}
					std::string parsedBody = "";

					while (pos < tmpBuff.length()) {
						size_t chunkLen = 0;
						std::stringstream ss;
						ss << std::hex << tmpBuff.substr(pos);
						ss >> chunkLen;

						// Find the len of chunk
						// Find the CRLF of len
						// Get the chunk by the content
						// Find the CRLF of chunk
						// If chunkLen <= 0 then chunk is ended

						// Position of CRLF (end current chunk len)
						find = tmpBuff.find(CRLF, pos);
						if (find == tmpBuff.npos) {
							return lastPos;
						}
						pos = find + 2;
						if (tmpBuff.substr(pos).length() < chunkLen) {
							return lastPos;
						}
						parsedBody += tmpBuff.substr(pos, chunkLen);
						pos += chunkLen;

						// Position of CRLF (end current chunk content)
						find = tmpBuff.find(CRLF, pos);
						if (find == tmpBuff.npos) {
							return lastPos;
						}
						pos = find + 2;
						if (chunkLen <= 0) {
							request.setBody(parsedBody);
							requests.push_back(request);
							pos += chunkLen;
							lastPos = pos;
							continue ;
						}
					}
				} else if (headers.has("Content-Lenght")) {
					std::stringstream	ss;
					size_t contentLen = 0;
					ss << headers.get("Content-Lenght");
					ss >> contentLen;
					if (tmpBuff.substr(pos).length() < contentLen) {
						return lastPos;
					}
					request.setBody(tmpBuff.substr(pos, contentLen));
					requests.push_back(request);
					pos += contentLen;
					lastPos = pos;
					continue ;
				} else {
					requests.push_back(request);
					lastPos = pos;
					continue ;
				}
			}
			return lastPos;
		}

		bool									HttpRequestBuilder::parseRequests(void) {
			request_list list;
			size_t lastPos = this->checkBuffer(list);
			if (lastPos <= 0) {
				return false;
			}
			this->requests.insert(this->requests.begin(), list.begin(), list.end());
			this->buffer.erase(0, lastPos);
			return true;
		}

	} // namespace Http

} // namespace Webserv
