/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 16:06:41 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/19 16:28:10 by ppaglier         ###   ########.fr       */
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
			 * T_COMMENT: start with a T_COMMENT and end with a T_NEWLINE, we can delete all of this part
			 * T_SIMPLE: start with a T_TEXT and end with a T_SIMPLE_END
			 * T_COMPLEX: start with a T_COMPLEX_START and end with a T_COMPLEX_END (can start with one or multiple T_TEXT)
			 *
			 * But how i can use all of it to using the parser..
			 * i really don't know for the moment so i'll continue to write something.. i'll probably find how.. but not now..
			 * If you read this, we're gonna play a game, okay ? Try to don't read the last line.
			 *
			 * You stupid because you loose the game so easily..
			*/

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
