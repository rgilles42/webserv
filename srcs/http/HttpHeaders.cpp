/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHeaders.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 16:45:31 by ppaglier          #+#    #+#             */
/*   Updated: 2022/01/18 10:20:20 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http/HttpHeaders.hpp"

namespace Webserv {

	namespace Http {
		// can have multiple identique key but not same value

		const HttpHeaders::header_type&	HttpHeaders::getHeaders(void) const {
			return this->headers;
		}

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
			header_type::const_iterator it = this->headers.lower_bound(key);
			return it->second;
		}

		bool	HttpHeaders::has(const key_type& key) const {
			return this->headers.count(key) > 0;
		}

		const std::string				HttpHeaders::toString(void) const {
			header_type::const_iterator it = this->headers.begin();
			header_type::const_iterator end = this->headers.end();
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


		HttpHeadersBuilder::HttpHeadersBuilder(void) {}
		HttpHeadersBuilder::HttpHeadersBuilder(const HttpHeadersBuilder& x) {
			this->buffer = x.buffer;
			this->headers = x.headers;
		}

		const HttpHeadersBuilder::buffer_type&	HttpHeadersBuilder::getBuffer(void) const {
			return this->buffer;
		}

		const HttpHeadersBuilder::headers_type&	HttpHeadersBuilder::getHeaders(void) const {
			return this->headers;
		}

		void									HttpHeadersBuilder::addMessage(const message_type& message) {
			this->buffer.append(message);
		}

		size_t									HttpHeadersBuilder::checkBuffer(void) const {
			headers_type list;
			return this->checkBuffer(list);
		}

		int										HttpHeadersBuilder::isKey(int c) {
			if (c == ':' || std::isspace(c)) {
				return 1;
			}
			return 0;
		}

		size_t									HttpHeadersBuilder::checkBuffer(headers_type& headers) const {
			buffer_type tmpBuff = this->buffer;
			size_t		pos = 0;
			buffer_type::iterator it_find;
			size_t		find = 0;
			size_t		endOfHeaders = tmpBuff.find(CRLF + CRLF);

			if (endOfHeaders == tmpBuff.npos) {
				return tmpBuff.npos;
			}

			while (pos <= tmpBuff.length() && pos < endOfHeaders) {
				headers_type::key_type key;
				headers_type::value_type value;

				// skip whitespace (i'm not sure)
				it_find = find_if(tmpBuff.begin() + pos, tmpBuff.end(), std::not1(std::ptr_fun<int, int>(std::isspace)));
				pos = it_find - tmpBuff.begin();
				if (tmpBuff.length() <= pos) {
					return tmpBuff.npos;
				}

				// Position of key (between pos & find)
				it_find = find_if(tmpBuff.begin() + pos, tmpBuff.end(), std::ptr_fun<int, int>(this->isKey));
				find = it_find - tmpBuff.begin();
				if (tmpBuff.length() <= find || pos == find) {
					return tmpBuff.npos;
				}
				key = tmpBuff.substr(pos, find - pos);
				pos = find + 1;

				// skip whitespace (i'm not sure)
				it_find = find_if(tmpBuff.begin() + pos, tmpBuff.end(), std::not1(std::ptr_fun<int, int>(std::isspace)));
				pos = it_find - tmpBuff.begin();
				if (tmpBuff.length() <= pos) {
					return tmpBuff.npos;
				}

				find = tmpBuff.find(CRLF, pos);
				if (find == tmpBuff.npos) {
					return tmpBuff.npos;
				}
				value = tmpBuff.substr(pos, find - pos);
				pos = find + CRLF.length();
				headers.set(key, value);
			}
			return endOfHeaders + (CRLF + CRLF).length();
		}

		bool									HttpHeadersBuilder::parseHeaders(void) {
			headers_type list;
			size_t lastPos = this->checkBuffer(list);
			if (lastPos <= 0 && lastPos != this->buffer.npos) {
				return false;
			}
			headers_type::header_type::const_iterator it = list.getHeaders().begin();
			while (it != list.getHeaders().end()) {
				this->headers.set(it->first, it->second);
				it++;
			}
			this->buffer.erase(0, lastPos);
			return true;
		}

	} // namespace Http

} // namespace Webserv
