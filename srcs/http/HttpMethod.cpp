/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpMethod.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 01:17:46 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/02 19:53:35 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http/HttpMethod.hpp"

namespace Webserv {

	namespace Http {

		const HttpMethod::MethodStringMap HttpMethod::HttpMethodStrings =  HttpMethod::fillMap();

		HttpMethod::HttpMethod(void) {
			this->setMethod(HttpMethod::unknown);
		}

		HttpMethod::HttpMethod(const int& method) {
			this->setMethod(method);
		}

		HttpMethod::HttpMethod(const std::string& methodString) {
			this->setMethod(methodString);
		}

		HttpMethod::HttpMethod(const HttpMethod& other) {
			*this = other;
		}

		HttpMethod::~HttpMethod() {}

		HttpMethod&		HttpMethod::operator=(const HttpMethod& other) {
			if (this != &other) {
				this->method = other.method;
				this->methodString = other.methodString;
			}
			return *this;
		}

		HttpMethod&		HttpMethod::operator=(const int& method) {
			this->setMethod(method);
			return *this;
		}

		HttpMethod&		HttpMethod::operator=(const std::string& methodString) {
			this->setMethod(methodString);
			return *this;
		}

		void									HttpMethod::setMethod(const int& method) {
			this->method = HttpMethod::getMethod(method);
			this->methodString = HttpMethod::getMethodString(this->method);
		}

		void									HttpMethod::setMethod(const std::string& methodString) {
			this->method = HttpMethod::getMethod(methodString);
			this->methodString = methodString;
		}

		const HttpMethod::Method&		HttpMethod::getMethod(void) const {
			return this->method;
		}

		const std::string&						HttpMethod::toString(void) const {
			return this->methodString;
		}


		const HttpMethod::MethodStringMap HttpMethod::fillMap(void) {
			MethodStringMap map;

			map[HttpMethod::unknown] = "";

			map[HttpMethod::GET] = "GET";
			map[HttpMethod::POST] = "POST";
			map[HttpMethod::DELETE] = "DELETE";

			map[HttpMethod::PATCH] = "PATCH";
			map[HttpMethod::PUT] = "PUT";
			map[HttpMethod::HEAD] = "HEAD";
			map[HttpMethod::OPTIONS] = "OPTIONS";
			map[HttpMethod::CONNECT] = "CONNECT";
			map[HttpMethod::TRACE] = "TRACE";

			return map;
		}

		HttpMethod::Method	HttpMethod::getMethod(const int& method) {
			const HttpMethod::Method newMethod = static_cast<HttpMethod::Method>(method);
			if (HttpMethod::HttpMethodStrings.count(newMethod) <= 0) {
				return HttpMethod::unknown;
			}
			return newMethod;
		}

		HttpMethod::Method	HttpMethod::getMethod(const std::string& methodString) {
			HttpMethod::MethodStringMap::const_iterator it = HttpMethod::HttpMethodStrings.begin();

			while (it != HttpMethod::HttpMethodStrings.end()) {
				if (it->second == methodString) {
					return it->first;
				}
				it++;
			}
			return HttpMethod::unknown;
		}

		std::string			HttpMethod::getMethodString(const HttpMethod::Method& method) {
			if (HttpMethod::HttpMethodStrings.count(method) <= 0) {
				return HttpMethod::HttpMethodStrings.at(HttpMethod::unknown);
			}
			return HttpMethod::HttpMethodStrings.at(method);
		}

	} // namespace Http

} // namespace Webserv
