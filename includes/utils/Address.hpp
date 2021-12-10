/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Address.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 22:18:20 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/10 13:27:13 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ADDRESS_HPP
# define ADDRESS_HPP

# include <cstring>
# include <string>
# include <cstdlib>
# include <iostream>

# include <stdint.h>

# include "../utils/common.hpp"

# define ADDRESS_BITS 16

namespace Webserv {

	namespace Utils {

		// IPV6 not supported for the moment
		class Address {
			public:
				typedef int8_t			uchar_t;
				typedef uchar_t			value_type[ADDRESS_BITS];
				typedef uint16_t		port_type;

				enum address_type {
					unknown,
					ipv4,
					ipv6,
				};

			protected:
				value_type		address;
				address_type	type;
				port_type		port;
				bool			addressIsValid;

				uint16_t	my_htons(uint16_t netshort) const;
				uint16_t	my_ntohs(uint16_t netshort) const;

			public:
				Address(void);
				Address(const std::string &address);

				~Address();

				void				reset(void);

				const value_type	&getAddress(void) const;
				const address_type	&getType(void) const;
				const port_type		&getPort(void) const;

				std::string			getStrAddress(void) const;

				int					getIntPort(void) const;

				bool				is(const address_type &type) const;
				bool				isIpv4(void) const;
				bool				isIpv6(void) const;

				const bool			&isAddressValid(void) const;

				bool				setPort(const int &port);
				bool				setAddress(const value_type &address);

				bool				fromString(const std::string &address);
				const std::string	toString(void) const;


				static void	test(const std::string &str) {
					Address addr;
					bool bSuccess = addr.fromString(str);
					std::cout << "Test case '" << str << "'" << std::endl;

					if (!bSuccess) {
						std::cout << "parse failed" << std::endl;
					}
					std::cout << "addr ";
					int count = 0;
					if (addr.isIpv4()) {
						count = 4;
						std::cout << "(ipv4): " << std::endl;
					} else if (addr.isIpv6()) {
						count = 16;
						std::cout << "(ipv6): " << std::endl;
					} else {
						std::cout << "(?): " << std::endl;
					}
					for (int i = 0; i < count; ++i) {
						printf("%02x", addr.address[i]);
					}

					std::cout << std::endl;
					if (0 == addr.getPort()) {
						std::cout << "port absent" << std::endl;
					} else {
						std::cout << "port:  " << addr.getPort() << " " << addr.getIntPort() << std::endl;
					}
					std::cout << std::endl << std::endl;
				}
		};

	} // namespace Utils

} // namespace Webserv

#endif