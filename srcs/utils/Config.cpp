/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 11:48:02 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/26 12:05:11 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils/Config.hpp"

namespace Webserv {

	namespace Utils {

		Config::Config() {}

		Config::~Config() {}

		bool	Config::addConfigFile(const file_type &file) {
			if (std::find(this->files.begin(), this->files.end(), file) != this->files.end()) {
				return false;
			}
			this->files.push_back(file);
			return true;
		}

		bool	Config::processFiles(void) {
			file_vector::const_iterator	it;

			parser_type::directive_map	directives;

			directives["server"].push_back("main");
			directives["listen"].push_back("server");
			directives["server_name"].push_back("server");
			directives["error_page"].push_back("server");
			directives["error_page"].push_back("location");
			directives["client_max_body_size"].push_back("server");
			directives["client_max_body_size"].push_back("location");
			directives["location"].push_back("server");
			directives["location"].push_back("location");
			directives["limit_except"].push_back("location");
			directives["return"].push_back("server");
			directives["return"].push_back("location");
			directives["autoindex"].push_back("server");
			directives["autoindex"].push_back("location");
			directives["root"].push_back("server");
			directives["root"].push_back("location");
			directives["index"].push_back("server");
			directives["index"].push_back("location");
			directives["upload_store"].push_back("server");
			directives["upload_store"].push_back("location");
			directives["cgi_pass"].push_back("location");

			it = this->files.begin();
			while (it != this->files.end()) {

				try {
					this->filesMap[(*it)] = getFileContents((*it));
				}
				catch(const std::exception& e) {
					std::cerr << e.what() << '\n';
					return false;
				}

				it++;
			}

			it = this->files.begin();
			while (it != this->files.end()) {

				this->lexerMap[(*it)].tokenize(this->filesMap[(*it)]);

				try {
					if (!this->lexerMap[(*it)].checkTokens()) {
						return false;
					}
				}
				catch(const std::exception& e) {
					std::cerr << e.what() << '\n';
					return false;
				}

				it++;
			}

			it = this->files.begin();
			while (it != this->files.end()) {

				this->parserMap[(*it)].blockenize(this->lexerMap[(*it)].getTokens());

				try {
					if (!this->parserMap[(*it)].checkBlocks(directives)) {
						return false;
					}
				}
				catch(const std::exception& e) {
					std::cerr << e.what() << '\n';
					return false;
				}

				it++;
			}

			/**
			 * Missing parser to servers, etc..
			*/

			return true;
		}

	} // namespace Utils


} // namespace Webserv
