/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Byte.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 16:14:01 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/10 14:07:26 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils/Byte.hpp"

namespace Webserv {

	namespace Utils {

		const Byte::unit_map Byte::unitsMap =  Byte::fillMap();

		Byte::Byte(const value_type& value, const unit_type& unit) {
			this->value = value;
			this->unit = unit;
		}

		void					Byte::setValue(const value_type& value) {
			this->value = value;
		}

		void					Byte::setUnit(const unit_type& unit) {
			this->unit = unit;
		}

		const Byte::value_type&	Byte::getValue(void) const {
			return this->value;
		}

		const Byte::unit_type&	Byte::getUnit(void) const {
			return this->unit;
		}

		Byte::value_type		Byte::toUnit(const unit_type& unit) const {
			return this->value * std::pow(1024, this->unit - unit);
		}

		const Byte::unit_map	Byte::fillMap(void) {
			unit_map unitsMap;

			unitsMap["b"] = Byte::U_B;
			unitsMap["k"] = Byte::U_KB;
			unitsMap["m"] = Byte::U_MB;
			unitsMap["g"] = Byte::U_GB;
			unitsMap["t"] = Byte::U_TB;

			return unitsMap;
		}

		bool					Byte::isUnitValid(const std::string& unit) {
			return Byte::unitsMap.count(unit) > 0;
		}

		const std::string		Byte::getStrByUnit(const unit_type& unit) {
			Byte::unit_map::const_iterator it = Byte::unitsMap.begin();
			while (it != Byte::unitsMap.end()) {
				if (it->second == unit) {
					return it->first;
				}
				it++;
			}
			return "";
		}

		Byte::unit_type			Byte::getUnitByStr(const std::string& unit) {
			if (!Byte::isUnitValid(unit)) {
				return Byte::U_B;
			}
			return Byte::unitsMap.at(unit);
		}


	} // namespace Utils

} // namespace Webserv
