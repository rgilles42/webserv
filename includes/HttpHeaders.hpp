/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHeaders.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 17:48:01 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/05 13:32:29 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPHEADERS_HPP
# define HTTPHEADERS_HPP

# include <cctype>
# include <string>
# include <map>
# include <functional>
# include <stdexcept>

# include "utils.hpp"

# define CR						std::string("\r")
# define LF						std::string("\n")
# define CRLF					(CR + LF)

# define AUTHORIZED_CHAR		std::string("!#$%&'*+-.^_`|~")

# define IS_AUTHORIZED_KEY_CHAR( c ) (std::isalpha(c) || std::isdigit(c) || AUTHORIZED_CHAR.find(c))

struct ci_less : std::binary_function<std::string, std::string, bool> {
	// case-independent (ci) compare_less binary function
	struct nocase_compare : public std::binary_function<unsigned char,unsigned char,bool>
	{
		bool operator() (const unsigned char& c1, const unsigned char& c2) const {
			return std::tolower (c1) < std::tolower (c2);
		}
	};
	bool operator() (const std::string & s1, const std::string & s2) const {
		return std::lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end(), nocase_compare ());
	}
};

class HttpHeaders {

	protected:
		typedef std::multimap<std::string, std::string, ci_less>	headerType;
		headerType	headers;

	public:
		// Headers Methods
		bool		isKeyValid(const std::string &key);
		void		set(const std::string &key, const std::string &value);
		void		append(const std::string &key, const std::string &value);
		const std::string	get(const std::string &key) const;
		bool		has(const std::string &key) const;

		// Utils Methods
		std::string	toString(void) const;
		void		fromString(const std::string &request);

};

#endif