/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 16:06:41 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/25 14:51:08 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../includes/utils/Lexer.hpp"

namespace Webserv {

	namespace Utils {

		Lexer::Lexer(void) {}

		Lexer::Lexer(const std::string &str) {
			this->tokenize(str);
		}

		const Lexer::token_vector	Lexer::getTokens(void) const {
			return this->tokens;
		}

		const Lexer::token_vector Lexer::tokenize(const std::string &str) {
			token_type::token_value value;
			token_type::token_type type;
			size_t i = 0;
			size_t j = 0;
			size_t line = 1;

			while (i < str.length()) {
				// Skip blank
				while (i < str.length()) {
					if (!Lexer::isblank(str[i])) {
						break ;
					}
					i++;
				}
				// Get the current type of token
				type = token_type::getTokenTypeByStr(str, i);
				if (token_type::isNewLine(type)) {
					line++;
				}
				if (type != token_type::T_TEXT) {
					// If the type of the token is a special type (not a text), then add this token and continue
					value = token_type::getTokenValueByType(type);
					this->tokens.push_back(token_type(value, type, line, i));
					i += value.length();
					continue ;
				}
				j = i + 1;
				while (j < str.length()) {
					if (Lexer::isblank(str[j]) || token_type::getTokenTypeByStr(str, j) != token_type::T_TEXT) {
						break ;
					}
					j++;
				}
				if (i != j) {
					value = str.substr(i, j - i);
					this->tokens.push_back(token_type(value, type, line, i));
				}
				i = j;
			}
			return this->tokens;
		}

		void			Lexer::drawTokens(void) const {
			Lexer::drawTokens(this->tokens);
		}

		bool			Lexer::checkTokens(void) const {
			/**
			 * We need to create another system of "token", that will be the real things i think
			 * We're gonna create block:
			 * - T_COMMENT: start with a T_COMMENT and end with a T_NEWLINE, we can delete all of this part
			 * - T_SIMPLE: start with a T_TEXT and end with a T_SIMPLE_END
			 * - T_COMPLEX: start with a T_COMPLEX_START and end with a T_COMPLEX_END (can start with one or multiple T_TEXT)
			 *
			 * The config can have differents warnings/errors:
			 * - missingEndOfSimpleContextError: When a T_SIMPLE don't have T_SIMPLE_END
			 * - missingEndOfComplexContextError: When a T_COMPLEX don't have T_COMPLEX_END
			 * - unexpectedToken: When a T_SIMPLE_END or T_COMPLEX_END is placed in an unexpected position (when 0 directive before for example)
			 *
			 * But how i can use all of it to using the parser..
			 * i really don't know for the moment so i'll continue to write something.. i'll probably find how.. but not now..
			 * If you read this, we're gonna play a game, okay ? Try to don't read the last line.
			 *
			 * You stupid because you loose the game so easily..
			*/

			size_t pos = 0;
			ssize_t	bracket = 0;

			while (pos < this->tokens.size()) {
				// std::cout << "|" << this->tokens[pos].getValue() << ":" << this->tokens[pos].getType() << "|" << std::endl;
				if (this->tokens[pos].isText()) {
					pos = this->checkTokenText(pos);
				} else if (this->tokens[pos].isSimpleEnd()) {
					pos = this->checkTokenSimpleEnd(pos);
				} else if (this->tokens[pos].isComplexStart()) {
					bracket++;
					pos = this->checkTokenComplexStart(pos);
				} else if (this->tokens[pos].isComplexEnd()) {
					pos = this->checkTokenComplexEnd(pos);
					bracket--;
				} else if (this->tokens[pos].isComment()) {
					pos = this->checkTokenComment(pos);
				} else if (this->tokens[pos].isNewLine()) {
					pos = this->checkTokenNewLine(pos);
				} else {
					throw UnknownTokenException(this->tokens[pos]);
				}
				pos++;
			}
			if (bracket > 0) {
				throw unexpectedEndOfStrException();
			}
			return true;
		}

		size_t				Lexer::checkTokenText(size_t pos) const {
			size_t i = pos;
			while (!this->tokens[i].isSimpleEnd() && !this->tokens[i].isComplexStart() && !this->tokens[i].isNewLine() && i < this->tokens.size()) {
				i++;
			}
			if (i >= this->tokens.size() || !(this->tokens[i].isSimpleEnd() || this->tokens[i].isComplexStart())) {
				throw missingEndOfDirectiveException(this->tokens[pos]);
			}
			return pos;
		}

		size_t				Lexer::checkTokenSimpleEnd(size_t pos) const {
			if (pos <= 0 || (pos > 0 && !this->tokens[pos - 1].isText())) {
				throw UnexpectedTokenException(this->tokens[pos]);
			}
			return pos;
		}

		size_t				Lexer::checkTokenComplexStart(size_t pos) const {
			if (pos <= 0 || (pos > 0 && !this->tokens[pos - 1].isText())) {
				throw UnexpectedTokenException(this->tokens[pos]);
			}
			return pos;
		}

		size_t				Lexer::checkTokenComplexEnd(size_t pos) const {
			if (pos <= 0) {
				throw UnexpectedTokenException(this->tokens[pos]);
			}
			return pos;
		}

		size_t				Lexer::checkTokenComment(size_t pos) const {
			while (!this->tokens[pos].isNewLine() && pos < this->tokens.size()) {
				pos++;
			}
			return pos;
		}

		size_t				Lexer::checkTokenNewLine(size_t pos) const {
			while (this->tokens[pos + 1].isNewLine() && pos + 1 < this->tokens.size()) {
				pos++;
			}
			return pos;
		}


		// Static methods

		void			Lexer::drawTokens(const token_vector &tokens) {
			token_vector::const_iterator it = tokens.begin();
			while (it != tokens.end()) {
				std::cout << "|" << (*it).getValue() << ":" << (*it).getType() << "|" << std::endl;
				it++;
			}
		}

	} // namespace Utils

} // namespace Webserv
