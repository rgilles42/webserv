/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Block.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 17:51:10 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/22 18:02:21 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils/Block.hpp"

namespace Webserv {

	namespace Utils {

		Block::Block(const block_type &type) {
			this->type = type;
		}

		Block::block_type			Block::getType(void) const {
			return this->type;
		}

		const Block::values_type	Block::getValues(void) const {
			return this->values;
		}

		const Block::childs_type	Block::getChilds(void) const {
			return this->childs;
		}

		void						Block::setType(const block_type &type) {
			this->type = type;
		}

		void						Block::addValue(const token_type &token) {
			this->values.push_back(token);
		}

		void						Block::addChild(const Block &child) {
			this->childs.push_back(child);
		}

	}
}

