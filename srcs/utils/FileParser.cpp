/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 16:04:46 by ppaglier          #+#    #+#             */
/*   Updated: 2022/02/10 17:21:30 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../includes/utils/FileParser.hpp"

namespace Webserv {

	namespace Utils {

		FileParser::FileParser(void) {
			this->init();
		}

		FileParser::FileParser(const FileParser& other) {
			*this = other;
		}

		FileParser::~FileParser(void) {}

		FileParser&	FileParser::operator=(const FileParser& other) {
			if (this != &other) {
				this->headers = other.headers;
				this->body = other.body;
			}
			return *this;
		}

		const FileParser::headers_type&	FileParser::getHeaders(void) const {
			return this->headers;
		}

		const FileParser::body_type&	FileParser::getBody(void) const {
			return this->body;
		}

		int				FileParser::isKey(int c) {
			if (c == ':' || std::isspace(c)) {
				return 1;
			}
			return 0;
		}

		void		FileParser::init(void) {
			this->headers.clear();
			this->body.clear();
		}

		size_t		FileParser::parseHeaders(const std::string& str, headers_type& headers) {
			std::string tmpBuff = str;
			size_t		pos = 0;
			std::string::iterator it_find;
			size_t		find = 0;
			size_t		endOfHeaders = tmpBuff.find(CRLF + CRLF);

			if (endOfHeaders == tmpBuff.npos) {
				return tmpBuff.npos;
			}

			while (pos <= tmpBuff.length() && pos < endOfHeaders) {
				headers_type::key_type key;
				headers_type::mapped_type value;

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
				headers[key] = value;
			}
			return endOfHeaders + (CRLF + CRLF).length();
		}


		size_t		FileParser::parseFile(const std::string& str) {
			size_t		lastPos = 0;
			std::string tmpBuff = str;
			size_t		pos = 0;
			std::string::iterator it_find;
			size_t		find = 0;

			while (pos <= tmpBuff.length()) {
				headers_type headers;
				// skip whitespace (i'm not sure)
				it_find = find_if(tmpBuff.begin() + pos, tmpBuff.end(), std::not1(std::ptr_fun<int, int>(std::isspace)));
				pos = it_find - tmpBuff.begin();
				if (tmpBuff.length() <= pos) {
					return lastPos;
				}

				// Headers
				find = this->parseHeaders(tmpBuff.substr(pos), headers);
				if (find == tmpBuff.npos) {
					return lastPos;
				}
				this->headers = headers;
				pos += find;
				if (tmpBuff.length() <= pos) {
					return lastPos;
				}

				// Message-Body
				this->body = tmpBuff.substr(pos);
				pos += this->body.length();
				lastPos = pos;
			}
			return lastPos;
		}


	} // namespace Utils

} // namespace Webserv