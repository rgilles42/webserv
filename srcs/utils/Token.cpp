/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 16:06:41 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/19 16:08:52 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../includes/utils/Token.hpp"

namespace Webserv {

	namespace Utils {

		const Token::token_map Token::tokensList =  Token::fillMap();

		Token::Token(const token_value &value, const token_type &type) {
			this->value = value;
			this->type = type;
		}

		const Token::token_value	Token::getValue(void) const {
			return this->value;
		}

		Token::token_type			Token::getType(void) const {
			return this->type;
		}

		bool						Token::is(token_type &type) const {
			return this->type == type;
		}

		bool						Token::isText(void) const {
			return Token::isText(this->type);
		}

		bool						Token::isComplex(void) const {
			return Token::isComplex(this->type);
		}

		bool						Token::isEnd(void) const {
			return Token::isEnd(this->type);
		}

		bool						Token::isComment(void) const {
			return Token::isComment(this->type);
		}

		bool						Token::isNewLine(void) const {
			return Token::isNewLine(this->type);
		}

		// Static methods

		const Token::token_map		Token::fillMap(void) {
			token_map tokensList;

			tokensList[Token::T_SIMPLE_END] = ";";
			tokensList[Token::T_COMMENT] = "#";
			tokensList[Token::T_COMPLEX_START] = "{";
			tokensList[Token::T_COMPLEX_END] = "}";
			tokensList[Token::T_NEWLINE_CRLF] = "\r\n";
			tokensList[Token::T_NEWLINE_CR] = "\r";
			tokensList[Token::T_NEWLINE_LF] = "\n";

			return tokensList;
		}

		bool						Token::isText(const token_type &type) {
					return type == Token::T_TEXT;
		}

		bool						Token::isComplex(const token_type &type) {
					return type == Token::T_COMPLEX_START || type == Token::T_COMPLEX_END;
		}

		bool						Token::isEnd(const token_type &type) {
					return type == Token::T_SIMPLE_END || type == T_COMPLEX_END;
		}

		bool						Token::isComment(const token_type &type) {
					return type == Token::T_COMMENT;
		}

		bool						Token::isNewLine(const token_type &type) {
			return type == Token::T_NEWLINE_CRLF || type == T_NEWLINE_CR || type == T_NEWLINE_LF;
		}

		Token::token_type			Token::getTokenTypeByStr(const std::string &str, size_t start) {
			token_map::const_iterator it = Token::tokensList.begin();
			while (it != Token::tokensList.end()) {
				if (str.compare(start, (*it).second.length(), (*it).second) == 0) {
					return (*it).first;
				}
				it++;
			}
			return Token::T_TEXT;
		}

		Token::token_value		Token::getTokenValueByType(token_type type) {
			if (Token::tokensList.count(type) <= 0) {
				return token_value();
			}
			return Token::tokensList.at(type);
		}

	} // namespace Utils

} // namespace Webserv
