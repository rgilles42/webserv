/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgilles <rgilles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 13:13:22 by ppaglier          #+#    #+#             */
/*   Updated: 2022/02/10 12:33:45 by rgilles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_HPP
# define COMMON_HPP

# include <string>		// For string
# include <sstream>		// For ostringstream
# include <fstream>		// For ifstream
# include <ctime>		// For gmtime, strftime
# include <algorithm>	// For find_if, not1, ptr_fun
# include <stdexcept>	// For runtime_error

#include <iostream>

# define SSTR( x ) static_cast< std::ostringstream&>( ( std::ostringstream() << std::dec << x ) ).str()

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 4096
# endif

namespace Webserv {

	namespace Utils {

		std::string&	ltrim(std::string& s);
		std::string&	rtrim(std::string& s);
		std::string&	trim(std::string& s);

		std::string		getFormatedDate(const time_t& rawtime);

		std::string		getFileContents(const std::string& filename);
		std::string		getFileExtension(const std::string& filename);

		const std::string	getConcatURL(const std::string& url1, const std::string url2);
		const std::string	url_decode(const std::string& value);
		void	separate_header(std::string& content, std::string& add_headers);

	} // namespace Utils

} // namespace Webserv


#endif