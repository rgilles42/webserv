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
