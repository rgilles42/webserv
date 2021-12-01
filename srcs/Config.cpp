/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 11:48:02 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/01 19:15:03 by ppaglier         ###   ########.fr       */
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

	void	Config::formatBlocks(block_vector &blocks) {
		if (blocks.size() <= 0) {
			return ;
		}
		block_vector newVector;
		block_vector simpleVector;
		block_vector typesVector;
		block_vector otherVector;
		block_vector::const_iterator blockIt;
		// directives tolower
		blockIt = blocks.begin();
		while (blockIt != blocks.end()) {
			block_type::values_type values = blockIt->getValues();
			block_type::values_type::value_type::token_value directive;
			if (values.size() > 0) {
				directive = values.at(0).getValue();
			}
			if (blockIt->isSimple()) {
				simpleVector.push_back(*blockIt);
			} else if (blockIt->isComplex()) {
				if (directive == "types") {
					typesVector.push_back(*blockIt);
				} else {
					otherVector.push_back(*blockIt);
				}
			}
			blockIt++;
		}
		newVector.insert(newVector.end(), simpleVector.begin(), simpleVector.end());
		newVector.insert(newVector.end(), typesVector.begin(), typesVector.end());
		newVector.insert(newVector.end(), otherVector.begin(), otherVector.end());
		blocks = newVector;
		block_vector::iterator it = blocks.begin();
		while (it != blocks.end()) {
			block_type::childs_type childs = it->getChilds();
			if (childs.size() > 0) {
				this->formatBlocks(childs);
				it->setChilds(childs);
			}
			it++;
		}
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

		filesIt = this->files.begin();
		while (filesIt != this->files.end()) {

			// Lexing all files
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

			// Parsing all files
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

		this->formatBlocks(this->blocks);

		parser_type::drawBlocks(this->blocks);

		block_vector::const_iterator it = this->blocks.begin();
		while (it != this->blocks.end()) {
			block_type::values_type values = it->getValues();
			if (values.size() > 0) {
				block_type::values_type::value_type::token_value value(values.at(0).getValue());
				if (value == "server") {
					server_type newServer;
					newServer;
					newServer.fromBlocks(it->getChilds());
					this->servers.push_back(newServer);
				} else if (value == "types") {
					this->globalMimesTypes.clear();
					this->globalMimesTypes.fromBlocks(it->getChilds());
				} else {
					std::cerr << "Unknown context: \"" << value << "\"" << std::endl;
					return false;
				}
			}
			it++;
		}

		return true;
	}

} // namespace Webserv
