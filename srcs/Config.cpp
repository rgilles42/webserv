/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 11:48:02 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/01 16:50:35 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Config.hpp"

namespace Webserv {

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
		file_vector::const_iterator	filesIt;

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

		// Get all files contents
		filesIt = this->files.begin();
		while (filesIt != this->files.end()) {

			try {
				this->filesMap[(*filesIt)] = getFileContents((*filesIt));
			}
			catch (const std::exception& e) {
				std::cerr << e.what() << " in " << (*filesIt) << std::endl;
				return false;
			}

			filesIt++;
		}

		// Lexing all files
		filesIt = this->files.begin();
		while (filesIt != this->files.end()) {

			this->lexerMap[(*filesIt)].tokenize(this->filesMap[(*filesIt)]);

			try {
				if (!this->lexerMap[(*filesIt)].checkTokens()) {
					return false;
				}
			}
			catch (const lexer_type::LexerException& e) {
				std::cerr << e.what() << " in " << (*filesIt);
				if (e.getToken().getLine() > 0) {
					std::cerr << ":" << e.getToken().getLine();
				}
				std::cerr << std::endl;
				return false;
			}

			filesIt++;
		}

		// Parsing all files
		filesIt = this->files.begin();
		while (filesIt != this->files.end()) {

			this->parserMap[(*filesIt)].blockenize(this->lexerMap[(*filesIt)].getTokens());

			try {
				if (!this->parserMap[(*filesIt)].checkBlocks(directives)) {
					return false;
				}
			}
			catch (const parser_type::ParserException& e) {
				std::cerr << e.what() << " in " << (*filesIt);
				if (e.getToken().getLine() > 0) {
					std::cerr << ":" << e.getToken().getLine();
				}
				std::cerr << std::endl;
				return false;
			}

			block_vector currentBlocks = this->parserMap[(*filesIt)].getBlocks();

			this->blocks.insert(this->blocks.begin(), currentBlocks.begin(), currentBlocks.end());
			filesIt++;
		}

		this->sortBlocks(this->blocks);

		parser_type::drawBlocks(this->blocks);

		// block_vector::const_iterator it = this->blocks.begin();
		// while (it != this->blocks.end()) {
		// 	block_type::values_type values = it->getValues();
		// 	if (values.size() > 0) {
		// 		block_type::values_type::value_type::token_value value(values.at(0).getValue());
		// 		std::transform(value.begin(), value.end(), value.begin(), ::tolower);
		// 		if (value == "server") {
		// 			server_type newServer;
		// 			newServer.fromBlocks(it->getChilds());
		// 			this->servers.push_back(newServer);
		// 		} else if (value == "types") {
		// 			this->globalMimesTypes.clear();
		// 			this->globalMimesTypes.fromBlocks(it->getChilds());
		// 		} else {
		// 			std::cerr << "Unknown context: \"" << value << "\"" << std::endl;
		// 			return false;
		// 		}
		// 	}
		// 	it++;
		// }

		return true;
	}

	void	Config::sortBlocks(const block_vector &blocks) {
		// std::sort(blocks.begin(), blocks.end(), this->compBlockVector);
		block_vector::const_iterator it = blocks.begin();
		while (it != blocks.end()) {
			block_vector child = it->getChilds();
			this->sortBlocks(child);
			it++;
		}
	}


} // namespace Webserv
