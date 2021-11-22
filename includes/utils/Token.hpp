/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 14:56:33 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/22 11:23:31 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_HPP
# define TOKEN_HPP

# include <string>
# include <map>
# include <vector>

# include <iostream>

namespace Webserv {

	namespace Utils {

		class Token {
			public:
				enum token_type {
					T_TEXT,
					T_SIMPLE_END,
					T_COMPLEX_START,
					T_COMPLEX_END,
					T_COMMENT,
					T_NEWLINE_CRLF,
					T_NEWLINE_CR,
					T_NEWLINE_LF,
				};
				typedef	std::string							token_value;
				typedef	std::size_t							token_line;
				typedef	std::size_t							token_pos;

				typedef std::map<token_type, token_value>	token_map;

				const static token_map	tokensList;

			protected:
				token_value	value;
				token_type	type;
				token_line	line;
				token_pos	position;

			public:
				// Non-static Methods
				Token(const token_value &value = token_value(), const token_type &type = Token::T_TEXT, const token_line &line = 1, const token_pos &position = token_pos());

				const token_value		getValue(void) const;

				token_type				getType(void) const;

				token_line				getLine(void) const;

				token_pos				getPosition(void) const;

				bool					is(token_type &type) const;
				bool					isText(void) const;
				bool					isSimpleEnd(void) const;
				bool					isComplexStart(void) const;
				bool					isComplexEnd(void) const;
				bool					isComment(void) const;
				bool					isNewLine(void) const;
				bool					isComplex(void) const;
				bool					isEnd(void) const;

				// Static Methods

				static bool				is(const token_type &type, const token_type &type2);
				static bool				isText(const token_type &type);
				static bool				isSimpleEnd(const token_type &type);
				static bool				isComplexStart(const token_type &type);
				static bool				isComplexEnd(const token_type &type);
				static bool				isComment(const token_type &type);
				static bool				isNewLine(const token_type &type);
				static bool				isEnd(const token_type &type);
				static bool				isComplex(const token_type &type);

				static const token_map	fillMap(void);

				static token_type		getTokenTypeByStr(const std::string &str, size_t start);

				static token_value		getTokenValueByType(token_type type);
		};

	} // namespace Utils

} // namespace Webserv

#endif // endif TOKEN_HPP