/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Byte.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 16:13:44 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/26 17:12:14 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BYTE_HPP
# define BYTE_HPP

# include <string>		// For string
# include <cmath>		// For pow
# include <map>			// For map

# include "ci_less.hpp"	// For ci_less

namespace Webserv {

	namespace Utils {

		class Byte {
			public:
				enum unit_type {
					U_B,
					U_KB,
					U_MB,
					U_GB,
					U_TB,
				};
				typedef long double	value_type;

				typedef std::map<std::string, unit_type, Webserv::Utils::ci_less>	unit_map;

				const static unit_map	unitsMap;

			protected:
				value_type	value;
				unit_type	unit;

			public:
				Byte(const value_type& value = value_type(), const unit_type& unit = Byte::U_B);

				void				setValue(const value_type& value);
				void				setUnit(const unit_type& unit = Byte::U_B);

				const value_type&	getValue(void) const;
				const unit_type&	getUnit(void) const;

				value_type			toUnit(const unit_type& unit) const;

				static const unit_map	fillMap(void);

				static bool					isUnitValid(const std::string& unit);

				static const std::string	getStrByUnit(const unit_type& unit);
				static unit_type			getUnitByStr(const std::string& unit);

		};

	} // namespace Utils

} // namespace Webserv

#endif