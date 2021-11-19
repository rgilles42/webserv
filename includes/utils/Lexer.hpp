/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 14:56:33 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/19 16:16:03 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_HPP
# define LEXER_HPP

# include <string>
# include <vector>

# include "Token.hpp"

namespace Webserv {

	namespace Utils {

		class Lexer {
			public:
				typedef Webserv::Utils::Token	token;
				typedef std::vector<token>		token_vector;

			protected:
				token_vector	tokens;

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