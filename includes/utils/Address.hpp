/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Address.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 22:18:20 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/01 23:39:53 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ADDRESS_HPP
# define ADDRESS_HPP

# include <string>
# include <sstream>
# include <cstdlib>
# include <cmath>
# include <iostream>

# include "../utils.hpp"

namespace Webserv {

	namespace Utils {

		// IPV6 not supported for the moment
		class Address {
			public:
				typedef unsigned long	value_type;

				enum address_type {
					ipv4,
					ipv6,
				};

			protected:
				value_type		address;
				address_type	type;

			public:
				Address(void);
				Address(const std::string &address);

				~Address();

				void				fromString(const std::string &address);
				const std::string	toString(void) const;

				const value_type	&getAddress(void) const;
				const address_type	&getType(void) const;

				bool				is(const address_type &type) const;
				bool				isIpv4(void) const;
				bool				isIpv6(void) const;
		};

	} // namespace Utils

} // namespace Webserv

#endif