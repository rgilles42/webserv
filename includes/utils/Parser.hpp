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

# include <iostream>

# include <string>
# include <vector>
# include <map>
# include <exception>
# include <sstream>
# include <sys/types.h>
# include <algorithm>

# include "Token.hpp"
# include "Block.hpp"

namespace Webserv {

	namespace Utils {

		class Parser {
			public:
				typedef Webserv::Utils::Token		token_type;
				typedef Webserv::Utils::Block		block_type;
				typedef std::vector<token_type>		token_vector;
				typedef std::vector<block_type>		block_vector;

				// map[directive] = [context1, context2, ...]
				typedef std::map<std::string, std::vector<std::string> >	directive_map;

				class ParserException : public std::exception {
					protected:
						std::string	msg;
						block_type	block;
						token_type	token;

					public:
						ParserException(const block_type &block = block_type(), const token_type &token = token_type(), const std::string &msg = "") : std::exception() {
							this->msg = msg;
							this->block = block;
							this->token = token;
						}
						virtual ~ParserException() throw() {}
						const block_type	getBlock() const {
							return this->block;

						}
						const token_type	getToken() const {
							return this->token;
						}
						virtual const char	*what() const throw() {
							return this->msg.c_str();
						}
				};

				class UnknownDirectiveException : public ParserException {
					public:
						UnknownDirectiveException(const block_type &block = block_type(), const token_type &token = token_type()) : ParserException(block) {
							std::ostringstream ss;

							ss << "Unknown directive \"" << token.getValue() << "\" at " << token.getLine();

							this->msg = ss.str();
						}
				};

				class DirectiveNotAllowedHereException : public ParserException {
					public:
						DirectiveNotAllowedHereException(const block_type &block = block_type(), const token_type &token = token_type()) : ParserException(block) {
							std::ostringstream ss;

							ss << "Directive \"" << token.getValue() << "\" is not allowed here at " << token.getLine();

							this->msg = ss.str();
						}
				};

				class UnknownException : public ParserException {
					public:
						UnknownException(const block_type &block = block_type()) : ParserException(block) {
							std::ostringstream ss;

							ss << "Unknown error at " << token.getLine();

							this->msg = ss.str();
						}
				};

			protected:
				block_vector	blocks;

				typedef std::pair<const block_type, bool> parse_type;

				parse_type			parseBlock(const token_vector &tokens, size_t &pos);

				bool				checkBlock(const directive_map &directives, const block_type &block, const std::string &context) const;

			public:
				Parser(void);
				Parser(const token_vector &tokens);

				const block_vector	blockenize(const token_vector &tokens);

				const block_vector	getBlocks(void) const;

				void				drawBlocks(void) const;

				bool				checkBlocks(const directive_map &directives = directive_map()) const;

				static void			drawBlocks(const block_vector &blocks);

		};

	} // namespace Utils

} // namespace Webserv

#endif // endif PARSER_HPP