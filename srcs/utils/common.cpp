/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgilles <rgilles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 13:12:44 by ppaglier          #+#    #+#             */
/*   Updated: 2022/02/09 21:54:00 by rgilles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils/common.hpp"

namespace Webserv {

	namespace Utils {

		std::string&	ltrim(std::string& s) {
			s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
			return s;
		}

		std::string&	rtrim(std::string& s) {
			s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
			return s;
		}

		std::string&	trim(std::string& s) {
			return ltrim(rtrim(s));
		}

		std::string		getFormatedDate(const time_t& rawtime) {
			struct tm *timeinfo;
			char buffer[BUFFER_SIZE + 1];

			timeinfo = gmtime(&rawtime);
			strftime(buffer, sizeof(buffer), "%a, %m %b %Y %H:%M:%S %Z", timeinfo);
			return std::string(buffer);
		}


		std::string		getFileContents(const std::string& filename) {
			std::ifstream in(filename.c_str(), std::ios::in | std::ios::binary);
			if (in) {
				return (std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>()));
			}
			throw (std::runtime_error("getFileContents"));
		}

		std::string		getFileExtension(const std::string& filename) {
			return filename.substr(filename.find_last_of(".") + 1);
		}

		const std::string	getConcatURL(const std::string& url1, const std::string url2) {
			size_t start;
			size_t end = 0;
			std::string newUrl;

			while ((start = url1.find_first_not_of('/', end)) != std::string::npos) {
				end = url1.find('/', start);
				if (!newUrl.empty()) {
					newUrl += "/";
				}
				newUrl += url1.substr(start, end - start);
			}

			end = 0;
			while ((start = url2.find_first_not_of('/', end)) != std::string::npos) {
				end = url2.find('/', start);
				newUrl += "/" + url2.substr(start, end - start);
			}

			return newUrl;
		}

		const std::string url_decode(const std::string& value)
		{
			std::ostringstream unescaped("");
			for (std::string::const_iterator i = value.begin(), n = value.end(); i != n; ++i)
			{
				std::string::value_type c = (*i);
				if (c == '%')
				{
					++i;
					std::string::value_type a = *i - (*i >= 'A' ? *i >= 'a' ? 'a' : 'A' : '0');
					a *= 16;
					++i;
					a += (*i - (*i >= 'A' ? *i >= 'a' ? 'a' : 'A' : '0'));
					unescaped << a;
				}
				else
					unescaped << c;
			}
			return unescaped.str();
		}

	} // namespace Utils

} // namespace Webserv
