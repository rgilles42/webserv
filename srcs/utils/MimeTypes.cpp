/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeTypes.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 19:40:21 by ppaglier          #+#    #+#             */
/*   Updated: 2022/02/11 22:57:28 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils/MimeTypes.hpp"

namespace Webserv {

	namespace Utils {

		MimeTypes::MimeTypes(void) {}

		MimeTypes::MimeTypes(const MimeTypes& other) {
			*this = other;
		}

		MimeTypes::MimeTypes(const block_vector& blocks) {
			this->fromBlocks(blocks);
		}

		MimeTypes::~MimeTypes(void) {}

		MimeTypes&					MimeTypes::operator=(const MimeTypes& other) {
			if (this != &other) {
				this->mappedTypes = other.mappedTypes;
			}
			return *this;
		}

		void						MimeTypes::merge(const MimeTypes& x) {
			MapType::const_iterator it = x.mappedTypes.begin();

			while  (it != x.mappedTypes.end()) {
				this->set(it->first, it->second);
				it++;
			}
		}

		void						MimeTypes::set(const std::string& key, const std::string& value) {
			this->mappedTypes[key] = value;
		}

		void						MimeTypes::clear(void) {
			this->mappedTypes.clear();
		}

		const std::string&			MimeTypes::getType(const std::string& path, const std::string& fallback) const {
			std::string fileExtension = getFileExtension(path);
			if (path.length() <= 0 || fileExtension.length() <= 0) {
				return fallback;
			}
			if (this->mappedTypes.count(fileExtension) > 0) {
				MapType::const_iterator it = this->mappedTypes.find(fileExtension);
				if (it != this->mappedTypes.end()) {
					return it->second;
				}
			}
			return fallback;
		}

		const MimeTypes::ReturnExtensionsType	MimeTypes::getExtensions(const std::string& type) const {
			ReturnExtensionsType extensions;
			MapType::const_iterator it = this->mappedTypes.begin();

			while (it != this->mappedTypes.end()) {
				if (it->second == type) {
					extensions.push_back(it->first);
				}
				it++;
			}
			return extensions;
		}

		bool									MimeTypes::fromBlocks(const block_vector& blocks) {
			size_t							i;
			std::string						key;
			std::string						value;
			block_vector::const_iterator	blockIt = blocks.begin();
			while (blockIt != blocks.end()) {
				if (!blockIt->isComment()) {
					i = 0;
					block_vector::value_type::values_type values = blockIt->getValues();
					block_vector::value_type::values_type::const_iterator valueIt = values.begin();
					while (valueIt != values.end()) {
						if (i <= 0) {
							value = valueIt->getValue();
						} else {
							key = valueIt->getValue();
							this->set(key, value);
						}
						valueIt++;
						i++;
					}
				}
				blockIt++;
			}
			return true;
		}

	} // namespace Utils

} // namespace Webserv
