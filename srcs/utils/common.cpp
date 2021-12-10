/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 13:12:44 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/10 13:27:32 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils/common.hpp"

namespace Webserv {

	namespace Utils {

		std::string	&ltrim(std::string &s) {
			s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
			return s;
		}

		std::string	&rtrim(std::string &s) {
			s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
			return s;
		}

		std::string	&trim(std::string &s) {
			return ltrim(rtrim(s));
		}

		std::string	getFormatedDate(const time_t &rawtime) {
			struct tm *timeinfo;
			char buffer[80];

			timeinfo = gmtime(&rawtime);
			strftime(buffer, sizeof(buffer), "%a, %m %b %Y %H:%M:%S %Z", timeinfo);
			return std::string(buffer);
		}


		std::string	getFileContents(const std::string &filename) {
			std::ifstream in(filename.c_str(), std::ios::in | std::ios::binary);
			if (in) {
				return (std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>()));
			}
			throw (std::runtime_error("getFileContents"));
		}

		std::string	getFileExtension(const std::string &filename) {
			return filename.substr(filename.find_last_of(".") + 1);
		}

	} // namespace Utils

} // namespace Webserv

