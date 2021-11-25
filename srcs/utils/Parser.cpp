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

		Parser::Parser(const token_vector &tokens) {
			this->blockenize(tokens);
		}

		const Parser::block_vector	Parser::getBlocks(void) const {
			return this->blocks;
		}

		const Parser::block_vector Parser::blockenize(const token_vector &tokens) {
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

		Parser::parse_type	Parser::parseBlock(const token_vector &tokens, size_t &pos) {
			bool		valid = false;
			block_type	newBlock;
			if (tokens[pos].isNewLine()) {
				while (tokens[pos].isNewLine() && pos < tokens.size()) {
					pos++;
				}
			}
			if (tokens[pos].isComplexEnd()) {
				pos--;
				return std::make_pair(newBlock, valid);
			}
			if (tokens[pos].isComment()) {
				newBlock.setType(block_type::T_COMMENT);
				pos++;
				while (!tokens[pos].isNewLine() && pos < tokens.size()) {
					newBlock.addValue(tokens[pos]);
					pos++;
				}
				valid = true;
				return std::make_pair(newBlock, valid);
			}
			while (!tokens[pos].isSimpleEnd() && !tokens[pos].isComplexStart() && pos < tokens.size()) {
				valid = true;
				if (!tokens[pos].isNewLine()) {
					newBlock.addValue(tokens[pos]);
				}
				pos++;
			}
			bool isComplex = tokens[pos].isComplexStart();
			pos++;
			if (isComplex) {
				newBlock.setType(block_type::T_COMPLEX);
				while (!tokens[pos].isComplexEnd() && pos < tokens.size()) {
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

		void			Parser::drawBlocks(void) const {
			Parser::drawBlocks(this->blocks);
		}

		bool			Parser::checkBlocks(const directive_map &directives) const {
			block_vector::const_iterator it = this->blocks.begin();
			while (it != this->blocks.end()) {
				if (!this->checkBlock(directives, (*it), "main")) {
					return false;
				}
				it++;
			}
			return true;
		}

		bool			Parser::checkBlock(const directive_map &directives, const block_type &block, const std::string &context) const {
			if (block.isComment()) {
				return true;
			}
			block_type::values_type values = block.getValues();
			if (values.size() <= 0) {
				throw UnknownException(block);
			}
			token_type::token_value currentDirective = values[0].getValue();

			if (directives.count(currentDirective) <= 0) {
				throw UnknownDirectiveException(block, values[0]);
			}

			directive_map::mapped_type allowedContext = directives.at(currentDirective);
			directive_map::mapped_type::const_iterator find = std::find(allowedContext.begin(), allowedContext.end(), context);
			if (find == allowedContext.end()) {
				throw DirectiveNotAllowedHereException(block, values[0]);
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

		// Static methods

		void			Parser::drawBlocks(const block_vector &blocks) {
			block_vector::const_iterator it = blocks.begin();
			while (it != blocks.end()) {
				block_type block = (*it);
				block_type::values_type values = block.getValues();
				block_type::values_type::const_iterator it2 = values.begin();
				while (it2 != values.end()) {
					block_type::token_type token = (*it2);
					std::cout << "|" << token.getValue() << "|" << std::endl;
					it2++;
				}
				if (block.isSimple()) {
					std::cout << ";" << std::endl;
				} else if (!block.isComment()) {
					std::cout << "{" << std::endl;
					Parser::drawBlocks(block.getChilds());
					std::cout << "}" << std::endl;
				}
				it++;
			}
		}

	} // namespace Utils

} // namespace Webserv
