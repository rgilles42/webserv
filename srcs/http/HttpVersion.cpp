/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpVersion.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 01:17:46 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/03 12:50:32 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http/HttpVersion.hpp"

namespace Webserv {

	namespace Http {

		HttpVersion::Version	HttpVersion::getVersion(const std::string &version) {
			if (version.length() <= 0) {
				return HttpVersion::http_0_9;
			}
			std::string number = "";
			size_t i = 0;
			while (i < version.length()) {
				if (std::isdigit(version[i])) {
					number += version[i];
				}
				i++;
			}
			switch (std::atoi(number.c_str())) {
				case HttpVersion::http_0_9:
					return HttpVersion::http_0_9;

				case HttpVersion::http_1_0:
					return HttpVersion::http_1_0;

				case HttpVersion::http_1_1:
					return HttpVersion::http_1_1;

				case 2:
				case HttpVersion::http_2_0:
					return HttpVersion::http_2_0;

				case 3:
				case HttpVersion::http_3_0:
					return HttpVersion::http_3_0;

				default:
					return HttpVersion::unknown;
			}
		}

		std::string			HttpVersion::getVersionString(const HttpVersion::Version &version) {
			switch (version) {
				case HttpVersion::http_0_9:
					return "";

				case HttpVersion::http_1_0:
					return "HTTP/1.0";

				case HttpVersion::http_1_1:
					return "HTTP/1.1";

				case HttpVersion::http_2_0:
					return "HTTP/2.0";

				case HttpVersion::http_3_0:
					return "HTTP/3.0";

				default:
					return "Unknown";
			}
		}

	} // namespace Http

} // namespace Webserv
