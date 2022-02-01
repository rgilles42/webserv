/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 11:48:02 by ppaglier          #+#    #+#             */
/*   Updated: 2022/01/31 14:34:21 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/core/Config.hpp"

namespace Webserv {

	Config::Config(void) {}

	Config::Config(const Config& other) {
		*this = other;
	}

	Config::~Config(void) {}

	Config&	Config::operator=(const Config& other) {
		if (this != &other) {
			this->files = other.files;
			this->filesMap = other.filesMap;
			this->lexerMap = other.lexerMap;
			this->parserMap = other.parserMap;
			this->blocks = other.blocks;
			this->servers = other.servers;
			this->globalMimesTypes = other.globalMimesTypes;
		}
		return *this;
	}

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
				throw ConfigException((*filesIt), e.what());
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
				throw LexerException(e, (*filesIt));
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
				throw ParserException(e, (*filesIt));
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
					try {
						if (!newServer.fromBlocks(it->getChilds())) {
							return false;
						}
					}
					catch (const std::exception& e) {
						throw e;
						return false;
					}
					this->servers.push_back(newServer);
				} else if (directive == "types") {
					this->globalMimesTypes.clear();
					if (!this->globalMimesTypes.fromBlocks(it->getChilds())) {
						return false;
					}
				} else {
					throw ConfigException("", directive_type::UnknownDirectiveException(directive).what());
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

	const Config::server_type	Config::getServer(const std::string& address, const int& port, const std::string& host) const {
		server_vector	serversTmp = this->servers;
		server_vector::iterator it = serversTmp.begin();

		while (it != serversTmp.end()) {
			if (port != it->getListen().getIntPort()) {
				it = serversTmp.erase(it);
				continue ;
			}
			// TODO: care of 0.0.0.0 with this (for now it's safe)
			if (address != it->getListen().getStrAddress()) {
				it = serversTmp.erase(it);
				continue ;
			}
			if (host == it->getServerName()) {
				return *it;
			}
			// std::cout << it->getListen().getStrAddress() << ":" << it->getListen().getIntPort() << std::endl;
			it++;
		}
		if (!serversTmp.empty()) {
			return serversTmp.front();
		}
		return this->servers.front();
	}

} // namespace Webserv
