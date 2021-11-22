/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Block.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 17:32:01 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/22 18:35:30 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BLOCK_HPP
# define BLOCK_HPP

# include <vector>

# include "Token.hpp"

namespace Webserv {

	namespace Utils {

		class Block {
			public:
				enum block_type {
					T_SIMPLE,
					T_COMPLEX,
					T_COMMENT,
				};
				typedef Webserv::Utils::Token	token_type;

				typedef std::vector<token_type>	values_type;
				typedef std::vector<Block>	childs_type;

			protected:
				block_type		type;
				values_type		values;
				childs_type		childs;

			public:
				Block(const block_type &typestatic bool				isText(const token_type &type);
				static bool				isSimpleEnd(const token_type &type);
				static bool				isComplexStart(const token_type &type);
				static bool				isComplexEnd(const token_type &type);
				static bool				isComment(const token_type &type);
				static bool				isNewLine(const token_type &type);
				static bool				isEnd(const token_type &type);
				static bool				isComplex(const token_type &type);= Block::T_SIMPLE);

				block_type			getType(void) const;
				const values_type	getValues(void) const;
				const childs_type	getChilds(void) const;

				void				setType(const block_type &type = Block::T_SIMPLE);

				void				addValue(const token_type &token);

				void				addChild(const Block &child);

		};

	}

}

#endif