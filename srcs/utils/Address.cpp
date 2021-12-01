/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Address.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 22:53:05 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/01 23:37:02 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils/Address.hpp"

namespace Webserv {

	namespace Utils {

		Address::Address(void) {}

		Address::Address(const std::string &address) {
			this->fromString(address);
		}

		Address::~Address() {}

		void						Address::fromString(const std::string &address) {
			std::istringstream	is(address);
			std::string			line;
			char				delim = '.';
			size_t				i = 0;

			if (address.find(delim)) {
				this->type = Address::ipv4;
			} else {
				this->type = Address::ipv6;
				delim = ':';
			}
			this->address = 0;
			if (this->isIpv4()) {
				while (std::getline(is, line, delim)) {
					if (!line.empty()) {
						int number = std::atoi(line.c_str());
						if (number < 0 || number > 255) {
							this->address = 0;
							return ;
						}
						this->address += (number * std::pow(256, i));
					}
					i++;
				}
			}
		}

		const std::string			Address::toString(void) const {
			std::string	formatedAddress = "";
			if (this->isIpv4()) {
				formatedAddress += SSTR((this->address & 0x000000FF) / std::pow(256, 0)) + ".";
				formatedAddress += SSTR((this->address & 0x0000FF00) / std::pow(256, 1)) + ".";
				formatedAddress += SSTR((this->address & 0x00FF0000) / std::pow(256, 2)) + ".";
				formatedAddress += SSTR((this->address & 0xFF000000) / std::pow(256, 3));
			}
			return formatedAddress;
		}

		const Address::value_type	&Address::getAddress(void) const {
			return this->address;
		}

		const Address::address_type	&Address::getType(void) const {
			return this->type;
		}

		bool						Address::is(const address_type &type) const {
			return this->type == type;
		}

		bool						Address::isIpv4(void) const {
			return this->is(Address::ipv4);
		}

		bool						Address::isIpv6(void) const {
			return this->is(Address::ipv6);
		}

	} // namespace Utils

} // namespace Webserv
