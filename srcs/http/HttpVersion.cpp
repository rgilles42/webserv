/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpVersion.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 01:17:46 by ppaglier          #+#    #+#             */
/*   Updated: 2022/02/02 18:26:15 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http/HttpVersion.hpp"

namespace Webserv {

	namespace Http {

		HttpVersion::HttpVersion(void) {
			this->setVersion(HttpVersion::unknown);
		}

		HttpVersion::HttpVersion(const int& version) {
			this->setVersion(version);
		}

		HttpVersion::HttpVersion(const std::string& versionString) {
			this->setVersion(versionString);
		}

		HttpVersion::HttpVersion(const HttpVersion& other) {
			*this = other;
		}

		HttpVersion::~HttpVersion() {}

		HttpVersion&		HttpVersion::operator=(const HttpVersion& other) {
			if (this != &other) {
				this->version = other.version;
				this->versionString = other.versionString;
			}
			return *this;
		}

		HttpVersion&		HttpVersion::operator=(const int& version) {
			this->setVersion(version);
			return *this;
		}

		HttpVersion&		HttpVersion::operator=(const std::string& versionString) {
			this->setVersion(versionString);
			return *this;
		}

		void									HttpVersion::setVersion(const int& version) {
			this->version = HttpVersion::getVersion(version);
			this->versionString = HttpVersion::getVersionString(this->version);
		}

		void									HttpVersion::setVersion(const std::string& versionString) {
			this->version = HttpVersion::getVersion(versionString);
			this->versionString = versionString;
		}

		const HttpVersion::Version&		HttpVersion::getVersion(void) const {
			return this->version;
		}

		const std::string&						HttpVersion::toString(void) const {
			return this->versionString;
		}


		HttpVersion::Version	HttpVersion::getVersion(const int& version) {
			switch (version) {
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

		HttpVersion::Version	HttpVersion::getVersion(const std::string& version) {
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
			return (HttpVersion::getVersion(std::atoi(number.c_str())));
		}

		std::string			HttpVersion::getVersionString(const HttpVersion::Version& version) {
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
					return "HTTP/3";

				default:
					return "Unknown";
			}
		}

	} // namespace Http

} // namespace Webserv
