/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 16:06:41 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/20 18:38:46 by ppaglier         ###   ########.fr       */
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
			token::token_value value;
			token::token_type type;
			size_t i = 0;
			size_t j = 0;

			while (i < str.length()) {
				// Skip blank
				while (i < str.length()) {
					if (!Lexer::isblank(str[i])) {
						break ;
					}
					i++;
				}
				// Get the current type of token
				type = token::getTokenTypeByStr(str, i);
				if (type != token::T_TEXT) {
					// If the type of the token is a special type (not a text), then add this token and continue
					value = token::getTokenValueByType(type);
					this->tokens.push_back(token(value, type));
					i += value.length();
					continue ;
				}
				j = i + 1;
				while (j < str.length()) {
					if (Lexer::isblank(str[j]) || token::getTokenTypeByStr(str, j) != token::T_TEXT) {
						break ;
					}
					j++;
				}
				if (i != j) {
					value = str.substr(i, j - i);
					this->tokens.push_back(token(value, type));
				}
				i = j;
			}
			return this->tokens;
		}

		void			Lexer::drawVector(void) const {
			Lexer::drawVector(this->tokens);
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
			 * - unexpectedThing: When a T_SIMPLE_END or T_COMPLEX_END is placed in an unexpected position (when 0 directive before for example)
			 *
			 * Need to speparate thoses detection for detect the perfect error (like doing a {{, it's an unexpected "{" in the second position)
			 *
			 * But how i can use all of it to using the parser..
			 * i really don't know for the moment so i'll continue to write something.. i'll probably find how.. but not now..
			 * If you read this, we're gonna play a game, okay ? Try to don't read the last line.
			 *
			 * You stupid because you loose the game so easily..
			*/

			size_t pos = 0;
			size_t i = 0;

			while (pos < this->tokens.size()) {
				i = pos;
				// std::cout << "|" << this->tokens[pos].getValue() << ":" << this->tokens[pos].getType() << "|" << std::endl;
				if (this->tokens[pos].isComment()) {
					while (!this->tokens[pos].isNewLine() && pos < this->tokens.size()) {
						pos++;
					}
				} else if (this->tokens[pos].isText()) {
					while (!this->tokens[i].isSimpleEnd() && !this->tokens[i].isComplexStart() && i < this->tokens.size()) {
						i++;
					}
					if (i >= this->tokens.size() && !this->tokens[i].isSimpleEnd() && !this->tokens[i].isComplexStart()) {
						throw std::runtime_error("Missing \";\"");
					}
				} else if (this->tokens[pos].isSimpleEnd()) {
					if (pos <= 0 || (pos > 0 && !this->tokens[pos - 1].isText())) {
						throw std::runtime_error("Unexpected \";\"");
					}
				} else if (this->tokens[pos].isComplexStart()) {
					if (pos <= 0 || (pos > 0 && !this->tokens[pos - 1].isText())) {
						throw std::runtime_error("Unexpected \"{\""); // This need to pe trigered if {{ is doing for exemple
					}
					i++;
					bool isBracketEnded = false;
					size_t bracketLevel = 0;
					while (i < this->tokens.size()) {
						if (this->tokens[i].isComplexStart()) {
							bracketLevel++;
						}
						if (this->tokens[i].isComplexEnd()) {
							if (bracketLevel == 0) {
								isBracketEnded = true;
								break ;
							}
							bracketLevel--;
						}
						i++;
					}
					if (!isBracketEnded) {
						throw std::runtime_error("Missing \"}\"");
					}
				} else if (this->tokens[pos].isComplexEnd()) {
					if (pos <= 0) {
						throw std::runtime_error("Unexpected \"}\"");
					}
					// find the start
				}
				pos++;
			}

			return true;
		}

		// Static methods

		void			Lexer::drawVector(const token_vector &tokens) {
			token_vector::const_iterator it = tokens.begin();
			while (it != tokens.end()) {
				std::cout << "|" << (*it).getValue() << ":" << (*it).getType() << "|" << std::endl;
				it++;
			}
		}

	} // namespace Utils

} // namespace Webserv
