/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Block.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 17:32:01 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/01 17:07:19 by ppaglier         ###   ########.fr       */
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
				typedef std::vector<Block>		childs_type;

			protected:
				block_type		type;
				values_type		values;
				childs_type		childs;

			public:
				Block(const block_type &type= Block::T_SIMPLE);

				const block_type	&getType(void) const;
				const values_type	&getValues(void) const;
				const childs_type	&getChilds(void) const;

				void				setType(const block_type &type = Block::T_SIMPLE);

				void				addValue(const token_type &token);

				void				addChild(const Block &child);

				bool				is(block_type &type) const;
				bool				isSimple(void) const;
				bool				isComplex(void) const;
				bool				isComment(void) const;

				// Static Methods

				static bool			is(const block_type &type, const block_type &type2);
				static bool			isSimple(const block_type &type);
				static bool			isComplex(const block_type &type);
				static bool			isComment(const block_type &type);

		};

	}

}

#endif