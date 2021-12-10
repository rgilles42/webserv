/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 16:06:41 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/10 14:46:12 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../includes/utils/Token.hpp"

namespace Webserv {

	namespace Utils {

		const Token::token_map Token::tokensList =  Token::fillMap();

		Token::Token(const token_value& value, const token_type& type, const token_line& line, const token_pos& position) {
			this->value = value;
			this->type = type;
			this->line = line;
			this->position = position;
		}

		const Token::token_value&	Token::getValue(void) const {
			return this->value;
		}

		const Token::token_type&	Token::getType(void) const {
			return this->type;
		}

		const Token::token_line&	Token::getLine(void) const {
			return this->line;
		}

		const Token::token_pos&		Token::getPosition(void) const {
			return this->position;
		}

		bool						Token::is(token_type& type) const {
			return Token::is(this->type, type);
		}

		bool						Token::isText(void) const {
			return Token::isText(this->type);
		}

		bool						Token::isSimpleEnd(void) const {
			return Token::isSimpleEnd(this->type);
		}

		bool						Token::isComplexStart(void) const {
			return Token::isComplexStart(this->type);
		}

		bool						Token::isComplexEnd(void) const {
			return Token::isComplexEnd(this->type);
		}

		bool						Token::isComment(void) const {
			return Token::isComment(this->type);
		}

		bool						Token::isNewLine(void) const {
			return Token::isNewLine(this->type);
		}

		bool						Token::isEnd(void) const {
			return Token::isEnd(this->type);
		}

		bool						Token::isComplex(void) const {
			return Token::isComplex(this->type);
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

		bool						Token::is(const token_type& type, const token_type& type2) {
			return type == type2;
		}

		bool						Token::isText(const token_type& type) {
			return Token::is(type, T_TEXT);
		}

		bool						Token::isSimpleEnd(const token_type& type) {
			return Token::is(type, T_SIMPLE_END);
		}

		bool						Token::isComplexStart(const token_type& type) {
			return Token::is(type, T_COMPLEX_START);
		}

		bool						Token::isComplexEnd(const token_type& type) {
			return Token::is(type, T_COMPLEX_END);
		}

		bool						Token::isComment(const token_type& type) {
			return Token::is(type, T_COMMENT);
		}

		bool						Token::isNewLine(const token_type& type) {
			return Token::is(type, T_NEWLINE_CRLF) || Token::is(type, T_NEWLINE_CR) || Token::is(type, T_NEWLINE_LF);
		}

		bool						Token::isEnd(const token_type& type) {
			return Token::isSimpleEnd(type) || Token::isComplexEnd(type);
		}

		bool						Token::isComplex(const token_type& type) {
			return Token::isComplexStart(type) || Token::isComplexEnd(type);
		}

		Token::token_type			Token::getTokenTypeByStr(const std::string& str, size_t start) {
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
