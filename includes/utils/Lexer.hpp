/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 14:56:33 by ppaglier          #+#    #+#             */
/*   Updated: 2022/01/28 21:14:05 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_HPP
# define LEXER_HPP

# include <string>			// For string
# include <vector>			// For vector
# include <exception>		// For exception
# include <sstream>			// For stringstream
# include <sys/types.h>		// For ssize_t

// # include <iostream>		// For cout, endl TODO: Remove

# include "Token.hpp"		// For Token

namespace Webserv {

	namespace Utils {

		class Lexer {
			public:
				typedef Webserv::Utils::Token	token_type;
				typedef std::vector<token_type>		token_vector;

				class LexerException : public std::exception {
					protected:
						std::string	msg;
						token_type	token;

					public:
						LexerException(const token_type& token = token_type(), const std::string& msg = "") : std::exception() {
							this->msg = msg;
							this->token = token;
						}
						virtual ~LexerException() throw() {}
						const token_type& getToken() const {
							return this->token;
						}
						virtual const char	*what() const throw() {
							return this->msg.c_str();
						}
				};

				class missingEndOfDirectiveException : public LexerException {
					public:
						missingEndOfDirectiveException(const token_type& token = token_type()) : LexerException(token) {
							std::ostringstream ss;

							ss << "directive \"" << token.getValue() << "\" is not terminated by \";\" at " << token.getLine();

							this->msg = ss.str();
						}
				};

				class unexpectedEndOfStrException : public LexerException {
					public:
						unexpectedEndOfStrException(const token_type& token = token_type()) : LexerException(token) {
							std::ostringstream ss;

							ss << "Unexpected end of string, expecting \"}\"";

							this->msg = ss.str();
						}
				};

				class UnexpectedTokenException : public LexerException {
					public:
						UnexpectedTokenException(const token_type& token = token_type()) : LexerException(token) {
							std::ostringstream ss;

							ss << "Unexpected \"" << token.getValue() << "\" at " << token.getLine();

							this->msg = ss.str();
						}
				};

				class UnknownTokenException : public LexerException {
					public:
						UnknownTokenException(const token_type& token = token_type()) : LexerException(token) {
							std::ostringstream ss;

							ss << "Unknown token \"" << token.getValue() << "\" at " << token.getLine();

							this->msg = ss.str();
						}
				};


			protected:
				token_vector	tokens;

				bool	isblank(int c) {
					return ::isblank(c);
				}

				size_t				checkTokenText(size_t pos) const;
				size_t				checkTokenSimpleEnd(size_t pos) const;
				size_t				checkTokenComplexStart(size_t pos) const;
				size_t				checkTokenComplexEnd(size_t pos) const;
				size_t				checkTokenComment(size_t pos) const;
				size_t				checkTokenNewLine(size_t pos) const;

			public:
				Lexer(void);
				Lexer(const std::string& str);

				const token_vector	&tokenize(const std::string& str);

				void				resetLexer(void);

				const token_vector	&getTokens(void) const;


				bool				checkTokens(void) const;

				// TODO: Remove
				// void				drawTokens(void) const;
				// static void			drawTokens(const token_vector& tokens);

		};

	} // namespace Utils

} // namespace Webserv

#endif // endif LEXER_HPP