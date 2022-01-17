/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 11:48:02 by ppaglier          #+#    #+#             */
/*   Updated: 2022/01/17 15:47:39 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Config.hpp"

namespace Webserv {

	Config::Config(void) {}

	Config::~Config(void) {}

	void	Config::reset(void) {
		this->files.clear();
		this->filesMap.clear();
		this->lexerMap.clear();
		this->parserMap.clear();
		this->blocks.clear();
		this->servers.clear();
		this->globalMimesTypes.clear();
	}

	bool	Config::addConfigFile(const file_type& file) {
		if (std::find(this->files.begin(), this->files.end(), file) != this->files.end()) {
			return false;
		}
		this->files.push_back(file);
		return true;
	}

	void	Config::formatBlocks(block_vector& blocks) {
		if (blocks.size() <= 0) {
			return ;
		}
		block_vector newVector;
		block_vector simpleVector;
		block_vector typesVector;
		block_vector otherVector;
		block_vector::const_iterator blockIt;

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

	const Config::directive_map	Config::getDirectiveMap(void) const {
		directive_map directives;

		{
			directive_type::context_vector contexts;
			contexts.push_back("main");
			directives["server"] = directive_type("server", std::make_pair(0, 0), contexts);
		}
		{
			directive_type::context_vector contexts;
			contexts.push_back("main");
			contexts.push_back("server");
			contexts.push_back("location");
			directives["types"] = directive_type("types", std::make_pair(0, 0), contexts);
		}
		{
			directive_type::context_vector contexts;
			contexts.push_back("server");
			directives["listen"] = directive_type("listen", std::make_pair(1, 2), contexts);
		}
		{
			directive_type::context_vector contexts;
			contexts.push_back("server");
			directives["server_name"] = directive_type("server_name", std::make_pair(0, 1), contexts);
		}
		{
			directive_type::context_vector contexts;
			contexts.push_back("server");
			contexts.push_back("location");
			directives["error_page"] = directive_type("error_page", std::make_pair(2, 2), contexts);
		}
		{
			directive_type::context_vector contexts;
			contexts.push_back("server");
			contexts.push_back("location");
			directives["client_max_body_size"] = directive_type("client_max_body_size", std::make_pair(1, 1), contexts);
		}
		{
			directive_type::context_vector contexts;
			contexts.push_back("server");
			contexts.push_back("location");
			directives["location"] = directive_type("location", std::make_pair(1, 2), contexts);
		}
		{
			directive_type::context_vector contexts;
			contexts.push_back("location");
			directives["limit_except"] = directive_type("limit_except", std::make_pair(0, -1), contexts);
		}
		{
			directive_type::context_vector contexts;
			contexts.push_back("server");
			contexts.push_back("location");
			directives["return"] = directive_type("return", std::make_pair(1, 2), contexts);
		}
		{
			directive_type::context_vector contexts;
			contexts.push_back("server");
			contexts.push_back("location");
			directives["autoindex"] = directive_type("autoindex", std::make_pair(1, 1), contexts);
		}
		{
			directive_type::context_vector contexts;
			contexts.push_back("server");
			contexts.push_back("location");
			directives["root"] = directive_type("root", std::make_pair(1, 1), contexts);
		}
		{
			directive_type::context_vector contexts;
			contexts.push_back("server");
			contexts.push_back("location");
			directives["index"] = directive_type("index", std::make_pair(1, -1), contexts);
		}
		{
			directive_type::context_vector contexts;
			contexts.push_back("server");
			contexts.push_back("location");
			directives["upload_store"] = directive_type("upload_store", std::make_pair(1, 1), contexts);
		}
		{
			directive_type::context_vector contexts;
			contexts.push_back("location");
			directives["cgi_pass"] = directive_type("cgi_pass", std::make_pair(1, 1), contexts);
		}
		return directives;
	}

	bool	Config::processFiles(void) {
		file_vector::const_iterator	filesIt;

		directive_map	directives = this->getDirectiveMap();

		// Get all files contents
		filesIt = this->files.begin();
		while (filesIt != this->files.end()) {

			try {
				this->filesMap[(*filesIt)] = Webserv::Utils::getFileContents((*filesIt));
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

		block_vector::const_iterator it = this->blocks.begin();
		while (it != this->blocks.end()) {
			block_type::values_type values = it->getValues();
			if (values.size() > 0) {
				block_type::values_type::value_type::token_value directive(values.at(0).getValue());
				if (directive == "server") {
					server_type newServer;
					newServer.setMimesTypes(this->globalMimesTypes);
					if (!newServer.fromBlocks(it->getChilds())) {
						return false;
					}
					this->servers.push_back(newServer);
				} else if (directive == "types") {
					this->globalMimesTypes.clear();
					if (!this->globalMimesTypes.fromBlocks(it->getChilds())) {
						return false;
					}
				} else {
					std::cerr << directive_type::UnknownDirectiveException(directive).what() << std::endl;
					return false;
				}
			}
			it++;
		}

		return true;
	}

	Config::server_vector	&Config::getServers(void) {
		return this->servers;
	}

} // namespace Webserv
