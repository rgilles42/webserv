/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHeaders.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 17:48:01 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/03 19:17:18 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPHEADERS_HPP
# define HTTPHEADERS_HPP

# include <string>
# include <map>
# include <stdexcept>

# include "utils.hpp"

# define CR						std::string("\r")
# define LF						std::string("\n")
# define CRLF					(CR + LF)

# define AUTHORIZED_CHAR		std::string("!#$%&'*+-.^_`|~")

# define IS_AUTHORIZED_KEY_CHAR( c ) (std::isalpha(c) || std::isdigit(c) || AUTHORIZED_CHAR.find(c))

class HttpHeaders {

	protected:
		typedef std::map<std::string, std::string>	headerType;
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