/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Block.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 17:51:10 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/10 14:03:04 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils/Block.hpp"

namespace Webserv {

	namespace Utils {

		Block::Block(const block_type& type) {
			this->type = type;
		}

		const Block::block_type&	Block::getType(void) const {
			return this->type;
		}

		const Block::values_type&	Block::getValues(void) const {
			return this->values;
		}

		const Block::childs_type&	Block::getChilds(void) const {
			return this->childs;
		}

		void						Block::setType(const block_type& type) {
			this->type = type;
		}

		void						Block::addValue(const token_type& token) {
			this->values.push_back(token);
		}

		void						Block::setChilds(const childs_type& childs) {
			this->childs = childs;
		}

		void						Block::addChild(const Block& child) {
			this->childs.push_back(child);
		}


		bool						Block::is(block_type& type) const {
			return Block::is(this->type, type);
		}

		bool						Block::isSimple(void) const {
			return Block::isSimple(this->type);
		}

		bool						Block::isComplex(void) const {
			return Block::isComplex(this->type);
		}

		bool						Block::isComment(void) const {
			return Block::isComment(this->type);
		}

		// Static methods

		bool						Block::is(const block_type& type, const block_type& type2) {
			return type == type2;
		}

		bool						Block::isSimple(const block_type& type) {
			return Block::is(type, T_SIMPLE);
		}

		bool						Block::isComplex(const block_type& type) {
			return Block::is(type, T_COMPLEX);
		}

		bool						Block::isComment(const block_type& type) {
			return Block::is(type, T_COMMENT);
		}

	}
}

