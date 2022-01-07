/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHeaders.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 17:48:01 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/22 08:30:14 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPHEADERS_HPP
# define HTTPHEADERS_HPP

# include <iostream>

# include <cctype>					// For isalpha, isdigit, etc..
# include <string>					// For string
# include <map>						// For map
# include <stdexcept>				// For runtime_error, out_of_range
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

				typedef std::multimap<key_type, value_type, Webserv::Utils::ci_less>	header_type;

			protected:
				header_type	headers;

			public:
				const header_type&	getHeaders(void) const;

				// Headers Methods
				bool				isKeyValid(const key_type& key);
				void				set(const key_type& key, const value_type& value);
				void				append(const key_type& key, const value_type& value);
				const value_type&	get(const key_type& key) const;
				bool				has(const key_type& key) const;

				// Utils Methods
				const std::string	toString(void) const;
				void				fromString(const std::string& request);
		};

		class HttpHeadersBuilder {
			public:
				typedef HttpHeaders					headers_type;
				typedef std::string					buffer_type;
				typedef std::string					message_type;

			protected:
				buffer_type		buffer;
				headers_type	headers;

				static int			isKey(int c);

			public:
				HttpHeadersBuilder(void);
				HttpHeadersBuilder(const HttpHeadersBuilder& x);

				const buffer_type&	getBuffer(void) const;
				const headers_type&	getHeaders(void) const;

				void				addMessage(const message_type& message);

				size_t				checkBuffer(void) const;
				size_t				checkBuffer(headers_type& headers) const;

				bool				parseHeaders(void);

		};

	} // namespace Utils

} // namespace Webserv

#endif