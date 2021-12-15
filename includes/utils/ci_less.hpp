/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ci_less.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 16:31:22 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/10 14:11:11 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CI_LESS_HPP
# define CI_LESS_HPP

# include <cctype>		// For tolower
# include <string>		// For string
# include <algorithm>	// For lexicographical_compare

namespace Webserv {

	namespace Utils {

		struct ci_less : std::binary_function<std::string, std::string, bool> {
			// case-independent (ci) compare_less binary function
			struct nocase_compare : public std::binary_function<unsigned char,unsigned char,bool> {
				bool operator() (const unsigned char& c1, const unsigned char& c2) const {
					return std::tolower (c1) < std::tolower (c2);
				}
			};
			bool operator() (const std::string & s1, const std::string & s2) const {
				return std::lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end(), nocase_compare ());
			}
		};

	} // namespace Utils

} // namespace Webserv


#endif