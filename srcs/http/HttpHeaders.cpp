/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHeaders.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 16:45:31 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/10 13:32:17 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http/HttpHeaders.hpp"

namespace Webserv {

	namespace Http {
		// can have multiple identique key but not same value

		// Headers Methods
		bool	HttpHeaders::isKeyValid(const key_type& key) {
			for (size_t i = 0; i < key.length(); i++) {
				if (!IS_AUTHORIZED_KEY_CHAR(key[i])) {
					return false;
				}
			}
			return true;
		}

		void	HttpHeaders::set(const key_type& key, const value_type& value) {
			if (!this->isKeyValid(key)) {
				throw std::runtime_error("HttpHeaders::set(" + key + ", " + value + ")");
			}
			this->headers.erase(key);
			this->headers.insert(std::make_pair(key, value));
			// this->headers[key] = value;
		}

		void	HttpHeaders::append(const key_type& key, const value_type& value) {
			if (!this->isKeyValid(key)) {
				throw std::runtime_error("HttpHeaders::append(" + key + ", " + value + ")");
			}
			if (!this->has(key)) {
				return this->set(key, value);
			}
			this->headers.insert(std::make_pair(key, value));
			// this->headers[key] += ", " + value;
		}

		const HttpHeaders::value_type&	HttpHeaders::get(const key_type& key) const {
			if (!this->has(key)) {
				throw std::out_of_range("HttpHeaders::get(" + key + ")");
			}
			headerType::const_iterator it = this->headers.lower_bound(key);
			return it->second;
		}

		bool	HttpHeaders::has(const key_type& key) const {
			return this->headers.count(key) > 0;
		}

		const std::string				HttpHeaders::toString(void) const {
			headerType::const_iterator it = this->headers.begin();
			headerType::const_iterator end = this->headers.end();
			std::string	formatedHeaders = "";

			while (it != end) {
				formatedHeaders += it->first + ": " + it->second + CRLF;
				it++;
			}
			return formatedHeaders;
		}

		void	HttpHeaders::fromString(const std::string& stringHeaders) {
			std::string			line;
			std::stringstream	ss(stringHeaders);

			while (std::getline(ss, line)) {
				line = Webserv::Utils::trim(line);
				std::string key = line.substr(0, line.find(':'));
				line = line.erase(0, line.find(':') + 1);
				std::string value = line;
				this->set(Webserv::Utils::trim(key), Webserv::Utils::trim(value));
			}
		}

	} // namespace Http

} // namespace Webserv
