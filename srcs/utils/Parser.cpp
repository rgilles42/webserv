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

		std::pair<const Block, bool>	Parser::parseBlock(const token_vector &tokens, size_t &pos) {
			Block newBlock;
			if (tokens[pos].isNewLine()) {
				while (tokens[pos].isNewLine() && pos < tokens.size()) {
					pos++;
				}
				return std::make_pair(newBlock, false);
			}
			if (tokens[pos].isComment()) {
				newBlock.setType(Block::T_COMMENT);
				pos++;
				while (!tokens[pos].isNewLine() && pos < tokens.size()) {
					newBlock.addValue(tokens[pos]);
					pos++;
				}
				return std::make_pair(newBlock, true);
			}
			while (!tokens[pos].isSimpleEnd() && !tokens[pos].isComplexStart() && pos < tokens.size()) {
				newBlock.addValue(tokens[pos]);
				pos++;
			}
			bool isComplex = tokens[pos].isComplexStart();
			pos++;
			if (isComplex) {
				newBlock.setType(Block::T_COMPLEX);
				while (!tokens[pos].isComplexEnd()) {
					parse_type child = this->parseBlock(tokens, pos);
					if (child.second) {
						newBlock.addChild(child.first);
					}
				}
			}
			return std::make_pair(newBlock, true);
		}

		void			Parser::drawBlocks(void) const {
			Parser::drawBlocks(this->blocks);
		}

		bool			Parser::checkBlocks(void) const {
			return true;
		}

		// Static methods

		void			Parser::drawBlocks(const block_vector &blocks) {
			block_vector::const_iterator it = blocks.begin();
			while (it != blocks.end()) {
				Block block = (*it);
				Block::values_type values = block.getValues();
				Block::values_type::const_iterator it2 = values.begin();
				while (it2 != values.end()) {
					Block::token_type token = (*it2);
					std::cout << "|" << token.getValue() << "|" << std::endl;
					it2++;
				}
				if (block.isSimple()) {
					std::cout << ";" << std::endl;
				} else {
					std::cout << "{" << std::endl;
					Parser::drawBlocks(block.getChilds());
					std::cout << "}" << std::endl;
				}
				it++;
			}
		}

	} // namespace Utils

} // namespace Webserv
