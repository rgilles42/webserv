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
				typedef Webserv::Utils::Token		token_type;
				typedef Webserv::Utils::Block		block_type;
				typedef std::vector<token_type>		token_vector;
				typedef std::vector<block_type>		block_vector;

				class ParserException : public std::exception {
					protected:
						std::string	msg;
						block_type	block;

					public:
						ParserException(const block_type &block = block_type(), const std::string &msg = "") : std::exception() {
							this->msg = msg;
							this->block = block;
						}
						virtual ~ParserException() throw() {}
						const block_type getblock() const {
							return this->block;
						}
						virtual const char	*what() const throw() {
							return this->msg.c_str();
						}
				};

			protected:
				block_vector	blocks;

				bool	isblank(int c) {
					return ::isblank(c);
				}

				typedef std::pair<const Block, bool> parse_type;

				parse_type	parseBlock(const token_vector &tokens, size_t &pos);

			public:
				Parser(void);
				Parser(const token_vector &tokens);

				const block_vector	blockenize(const token_vector &tokens);

				const block_vector	getBlocks(void) const;

				void				drawBlocks(void) const;

				bool				checkBlocks(void) const;

				static void			drawBlocks(const block_vector &blocks);

		};

	} // namespace Utils

} // namespace Webserv

#endif // endif PARSER_HPP