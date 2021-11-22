/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 14:56:33 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/22 17:31:17 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

# include <string>
# include <vector>
# include <exception>
# include <sstream>
# include <sys/types.h>

# include "Token.hpp"
# include "Block.hpp"

namespace Webserv {

	namespace Utils {

		class Parser {
			public:
				typedef Webserv::Utils::Token	token_type;
				typedef Webserv::Utils::Token	block_type;
				typedef std::vector<token_type>		token_vector;

				class Parser : public std::exception {
					protected:
						std::string	msg;
						block_type	block;

					public:
						Parser(const block_type &block = block_type(), const std::string &msg = "") : std::exception() {
							this->msg = msg;
							this->block = block;
						}
						virtual ~Parser() throw() {}
						const block_type getblock() const {
							return this->block;
						}
						virtual const char	*what() const throw() {
							return this->msg.c_str();
						}
				};

			protected:
				token_vector	block;

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
				Parser(void);
				Parser(const std::string &str);

				const token_vector	tokenize(const std::string &str);

				const token_vector	getTokens(void) const;

				void				drawVector(void) const;

				bool				checkTokens(void) const;

				static void			drawVector(const token_vector &tokens);



		};

	} // namespace Utils

} // namespace Webserv

#endif // endif PARSER_HPP