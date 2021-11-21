/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 14:56:33 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/21 15:09:56 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_HPP
# define LEXER_HPP

# include <string>
# include <vector>
# include <exception>

# include "Token.hpp"

namespace Webserv {

	namespace Utils {

		class Lexer {
			public:
				typedef Webserv::Utils::Token	token_type;
				typedef std::vector<token_type>		token_vector;

				// class block {
				// 	public:
				// 		enum block_type {
				// 			T_SIMPLE,
				// 			T_COMPLEX,
				// 			T_COMMENT,
				// 		};
				// 	protected:
				// 		token_vector	tokens;
				// 		block_type		type;

				// 	public:

				// };


			protected:
				token_vector	tokens;

				class LexerException : public std::exception {
					protected:
						std::string	msg;
						token_type	token;

					public:
						LexerException(const std::string &msg = "", const token_type &token = token_type()) : std::exception() {
							this->msg = msg;
							this->token = token;
						}
						const token_type getToken() const {
							return this->token;
						}
						virtual const char	*what() const throw() {
							return this->msg.c_str();
						}
				};

				bool	isblank(int c) {
					return ::isblank(c);
				}

			public:
				Lexer(void);
				Lexer(const std::string &str);

				const token_vector	tokenize(const std::string &str);

				const token_vector	getTokens(void) const;

				void				drawVector(void) const;

				bool				checkTokens(void) const;

				static void			drawVector(const token_vector &tokens);

		};

	} // namespace Utils

} // namespace Webserv

#endif // endif LEXER_HPP