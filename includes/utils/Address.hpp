/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Address.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 22:18:20 by ppaglier          #+#    #+#             */
/*   Updated: 2022/01/30 03:06:27 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ADDRESS_HPP
# define ADDRESS_HPP

# include <string>		// For string
# include <cstring>		// For memcpy
# include <cctype>		// For isblank

# include <stdint.h>	// For int8_t, uint16_t

# include "common.hpp"	// For SSTR

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
				Address(const Address& other);
				Address(const std::string& address);
				~Address();

				Address&			operator=(const Address& other);

				void				reset(void);

				const value_type&	getAddress(void) const;
				const address_type&	getType(void) const;
				const port_type&	getPort(void) const;

				std::string			getStrAddress(void) const;

				int					getIntPort(void) const;

				bool				is(const address_type& type) const;
				bool				isIpv4(void) const;
				bool				isIpv6(void) const;

				const bool&			isAddressValid(void) const;

				bool				setPort(const int& port);
				bool				setAddress(const value_type& address);

				bool				fromString(const std::string& address);
				const std::string	toString(void) const;

				bool				operator==(const Address &other) const;
		};

	} // namespace Utils

} // namespace Webserv

#endif