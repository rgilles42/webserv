/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHeaders.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 17:48:01 by ppaglier          #+#    #+#             */
/*   Updated: 2021/10/28 19:50:35 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPHEADERS_HPP
# define HTTPHEADERS_HPP

#include <string>
#include <map>

#include "utils.hpp"

# define LINE_BREAK				std::string("\r\n")
# define AUTHORIZED_CHAR		std::string("!#$%&'*+-.^_`|~")

# define IS_AUTHORIZED_KEY_CHAR( c ) (std::isalpha(c) || std::isdigit(c) || AUTHORIZED_CHAR.find(c))

class HttpHeaders {

	protected:
		std::map<std::string, std::string>	headers;

	public:
		// Headers Methods
		bool		isKeyValid(const std::string &key);
		void		set(const std::string &key, const std::string &value);
		const std::string	get(const std::string &key) const;

		// Utils Methods
		std::string	toString(void) const;
		void		fromString(const std::string &request);

};

#endif