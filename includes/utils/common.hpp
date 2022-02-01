/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 13:13:22 by ppaglier          #+#    #+#             */
/*   Updated: 2022/01/30 03:06:39 by ppaglier         ###   ########.fr       */
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

# define SSTR( x ) static_cast< std::ostringstream&>( ( std::ostringstream() << std::dec << x ) ).str()

namespace Webserv {

	namespace Utils {

		std::string&	ltrim(std::string& s);
		std::string&	rtrim(std::string& s);
		std::string&	trim(std::string& s);

		std::string		getFormatedDate(const time_t& rawtime);

		std::string		getFileContents(const std::string& filename);
		std::string		getFileExtension(const std::string& filename);

	} // namespace Utils

} // namespace Webserv


#endif