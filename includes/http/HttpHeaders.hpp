/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHeaders.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 17:48:01 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/10 13:26:04 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPHEADERS_HPP
# define HTTPHEADERS_HPP

# include <cctype>					// For isalpha, isdigit, etc..
# include <string>					// For string
# include <map>						// For map
# include <stdexcept>				// For exceptions
# include <sstream>					// For stringstream

# include "../utils/common.hpp"		// For trim
# include "../utils/ci_less.hpp"	// For ci_less

# define CR						std::string("\r")
# define LF						std::string("\n")
# define CRLF					(CR + LF)

# define AUTHORIZED_CHAR		std::string("!#$%&'*+-.^_`|~")

# define IS_AUTHORIZED_KEY_CHAR( c ) (std::isalpha(c) || std::isdigit(c) || AUTHORIZED_CHAR.find(c))

namespace Webserv {

	namespace Http {

		class HttpHeaders {
			public:
				typedef std::string										key_type;
				typedef std::string										value_type;

				typedef std::multimap<key_type, value_type, ci_less>	headerType;

			protected:
				headerType	headers;

			public:
				// Headers Methods
				bool				isKeyValid(const key_type& key);
				void				set(const key_type& key, const value_type& value);
				void				append(const key_type& key, const value_type& value);
				const value_type	&get(const key_type& key) const;
				bool				has(const key_type& key) const;

				// Utils Methods
				const std::string	toString(void) const;
				void				fromString(const std::string& request);
		};

	} // namespace Utils

} // namespace Webserv

#endif