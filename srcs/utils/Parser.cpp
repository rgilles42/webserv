/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 16:06:41 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/23 13:54:20 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../includes/utils/Parser.hpp"

namespace Webserv {

	namespace Utils {

		Parser::Parser(void) {}

		Parser::Parser(const Parser& other) {
			*this = other;
		}

		Parser::Parser(const token_vector& tokens) {
			this->blockenize(tokens);
		}

		Parser::~Parser(void) {}

		Parser&						Parser::operator=(const Parser& other) {
			if (this != &other) {
				this->blocks = other.blocks;
			}
			return *this;
		}

		const Parser::block_vector&	Parser::getBlocks(void) const {
			return this->blocks;
		}

		void						Parser::resetParser(void) {
			this->blocks.clear();
		}

		const Parser::block_vector&	Parser::blockenize(const token_vector& tokens) {
			size_t pos = 0;

			while (pos < tokens.size()) {
				parse_type parsedBlock = this->parseBlock(tokens, pos);
				if (parsedBlock.second) {
					this->blocks.push_back(parsedBlock.first);
				}
				pos++;
			}
			return this->blocks;
		}

		Parser::parse_type	Parser::parseBlock(const token_vector& tokens, size_t& pos) {
			bool		valid = false;
			block_type	newBlock;
			if (pos < tokens.size() && tokens[pos].isNewLine()) {
				while (pos < tokens.size() && tokens[pos].isNewLine()) {
					pos++;
				}
			}
			if (pos < tokens.size() && tokens[pos].isComplexEnd()) {
				pos--;
				return std::make_pair(newBlock, valid);
			}
			if (pos < tokens.size() && tokens[pos].isComment()) {
				newBlock.setType(block_type::T_COMMENT);
				pos++;
				while (pos < tokens.size() && !tokens[pos].isNewLine()) {
					newBlock.addValue(tokens[pos]);
					pos++;
				}
				valid = true;
				return std::make_pair(newBlock, valid);
			}
			while (pos < tokens.size() && !tokens[pos].isSimpleEnd() && !tokens[pos].isComplexStart()) {
				valid = true;
				if (pos < tokens.size() && !tokens[pos].isNewLine()) {
					newBlock.addValue(tokens[pos]);
				}
				pos++;
			}
			bool isComplex = pos < tokens.size() && tokens[pos].isComplexStart();
			pos++;
			if (isComplex) {
				newBlock.setType(block_type::T_COMPLEX);
				while (pos < tokens.size() && !tokens[pos].isComplexEnd()) {
					parse_type child = this->parseBlock(tokens, pos);
					if (child.second) {
						valid = true;
						newBlock.addChild(child.first);
					}
					pos++;
				}
			}
			return std::make_pair(newBlock, valid);
		}

		bool			Parser::checkBlocks(const directive_map& directives) const {
			block_vector::const_iterator it = this->blocks.begin();
			while (it != this->blocks.end()) {
				if (!this->checkBlock(directives, (*it), "main")) {
					return false;
				}
				it++;
			}
			return true;
		}

		bool			Parser::checkBlock(const directive_map& directives, const block_type& block, const std::string& context) const {
			if (block.isComment()) {
				return true;
			}
			block_type::values_type values = block.getValues();
			if (values.size() <= 0) {
				throw UnknownException(block);
			}
			token_type::token_value currentDirective = values[0].getValue();

			if (context != "types") {
				if (directives.count(currentDirective) <= 0) {
					throw UnknownDirectiveException(block, values[0]);
				}
				directive_map::mapped_type directive = directives.at(currentDirective);
				if (!directive.isContextValid(context)) {
					throw DirectiveNotAllowedHereException(block, values[0]);
				}
				if (!directive.isSrcValueIsValid(values)) {
					throw InvalidArgumentsDirectiveException(block, values[0]);
				}
			}

			block_type::childs_type childs = block.getChilds();
			if (childs.size() > 0) {
				if (block.isSimple()) {
					throw UnknownException(block);
				}
				block_type::childs_type::const_iterator it = childs.begin();
				while (it != childs.end()) {
					if (!this->checkBlock(directives, (*it), currentDirective)) {
						return false;
					}
					it++;
				}
			}
			return true;
		}

	} // namespace Utils

} // namespace Webserv
