/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 16:05:37 by ppaglier          #+#    #+#             */
/*   Updated: 2022/02/10 17:21:40 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_PARSER_HPP
# define FILE_PARSER_HPP

# include <map>			// For map
# include <string>		// For string
# include <algorithm>	// For find_if

# include <iostream>

# define CR						std::string("\r")
# define LF						std::string("\n")
# define CRLF					(CR + LF)

namespace Webserv {

	namespace Utils {

		class FileParser {
			public:
				typedef std::map<std::string, std::string>	headers_type;
				typedef std::string							body_type;

			protected:
				headers_type	headers;
				body_type		body;

				static int			isKey(int c);

			public:
				FileParser(void);
				FileParser(const FileParser& other);
				~FileParser();

				FileParser&			operator=(const FileParser& other);

				const headers_type&	getHeaders(void) const;
				const body_type&	getBody(void) const;

				void				init(void);

				size_t				parseHeaders(const std::string& str, headers_type& headers);
				size_t				parseFile(const std::string& str);

		};

	} // namespace Utils

} // namespace Webserv

#endif