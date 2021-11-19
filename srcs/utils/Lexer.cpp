/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 16:06:41 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/19 16:16:44 by ppaglier         ###   ########.fr       */
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
