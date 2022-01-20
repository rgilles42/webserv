/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Address.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 22:18:20 by ppaglier          #+#    #+#             */
/*   Updated: 2022/01/17 16:22:47 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils/Address.hpp"

namespace Webserv {

	namespace Utils {

		Address::Address(void) {}

		Address::Address(const std:: string& address) {
			this->fromString(address);
		}

		Address::~Address() {}

		uint16_t					Address::my_htons(uint16_t hostshort) const {
			return ((hostshort&  0xff) << 8) | (hostshort >> 8);
		}
		uint16_t					Address::my_ntohs(uint16_t netshort) const {
			return ((netshort&  0xff) << 8) | (netshort >> 8);
		}

		void						Address::reset(void) {
			this->port = 0;
			this->type = Address::unknown;
			std::memset(this->address, 0, ADDRESS_BITS);
			this->addressIsValid = false;
		}

		const Address::value_type&	Address::getAddress(void) const {
			return this->address;
		}

		const Address::address_type&	Address::getType(void) const {
			return this->type;
		}

		const Address::port_type&	Address::getPort(void) const {
			return this->port;
		}

		std::string					Address::getStrAddress(void) const {
			std::string address;
			if (this->isIpv4()) {
				address += SSTR((int)this->address[0]) + ".";
				address += SSTR((int)this->address[1]) + ".";
				address += SSTR((int)this->address[2]) + ".";
				address += SSTR((int)this->address[3]);
			} else if (this->isIpv6()) {
				address += ":::";
			}
			return address;
		}

		int							Address::getIntPort(void) const {
			return this->my_ntohs(this->port);
		}

		bool						Address::is(const address_type& type) const {
			return this->type == type;
		}

		bool						Address::isIpv4(void) const {
			return this->is(Address::ipv4);
		}

		bool						Address::isIpv6(void) const {
			return this->is(Address::ipv6);
		}

		const bool&					Address::isAddressValid(void) const {
			return this->addressIsValid;
		}

		bool						Address::setPort(const int& number) {
			if (number < 0 || number > 65535) {
				return false;
			}
			this->port = this->my_htons(number);
			return true;
		}

		bool						Address::setAddress(const value_type& address) {
			std::memcpy(this->address, address, ADDRESS_BITS);
			this->addressIsValid = true;
			return true;
		}

		bool						Address::fromString(const  std::string& str) {
			bool success = true;
			std::string line = str;
			size_t pchColon = line.find(':');
			size_t pchDot = line.find('.');
			size_t pchOpenBracket = line.find('[');
			size_t pchCloseBracket = line.npos;

				bool				fromString(const std::string& address);
				const std::string	toString(void) const;

				bool				operator==(const Address &other) const;
		};

			this->reset();

			this->type = Address::unknown;
			if (pchOpenBracket != line.npos || (pchColon != line.npos && (pchDot == line.npos || pchColon < pchDot))) {
				pchCloseBracket = line.find(']');
				if (pchOpenBracket != line.npos && (pchOpenBracket == line.npos || pchCloseBracket < pchOpenBracket)) {
					return false;
				}
				this->type = Address::ipv6;
			} else {
				if (pchColon != line.npos && pchColon < pchDot) {
					return false;
				}
				this->type = Address::ipv4;
			}

			if (this->isIpv4()) {
				if (pchDot == line.npos && pchColon == line.npos) {
					return this->setPort(std::atoi(line.c_str()));
				}
				size_t count = 0;
				size_t j = 0;
				while (i < line.length()) {
					j = i + 1;
					while (j < line.length()) {
						if (line[j] == '.' || line[j] == ':') {
							break ;
						}
						j++;
					}
					if (i != j) {
						std::string value = line.substr(i, j - i);
						if (count < 4) {
							if (count < 3 && line[j] != '.') {
								success = false;
							}
							if (success && value.find_first_not_of("0123456789") != value.npos) {
								success = false;
							}
							if (success) {
								int number = std::atoi(value.c_str());
								if (number < 0 || number > 255) {
									return false;
								}
								this->address[count] = number;
							}
							count++;
							j++;
						} else {
							if (!this->setPort(std::atoi(value.c_str()))) {
								return success;
							}
						}
					}
					i = j;
				}
			} else if (this->isIpv6()) {
				// "je suis une IPV6 non traiter"
			}
			if (success) {
				this->addressIsValid = true;
			}
			return success;
		}
		bool				Address::operator==(const Address &other) const {
			if (this->type != other.type) {
				return false;
			}
			if (this->addressIsValid != other.addressIsValid) {
				return false;
			}
			if (this->addressIsValid && this->address != other.address) {
				return false;
			}
			if (this->port != other.port) {
				return false;
			}
			return true;
		}

	} // namespace Utils

} // namespace Webserv