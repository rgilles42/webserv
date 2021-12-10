/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpVersion.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 00:38:38 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/10 13:44:05 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPVERSION_HPP
# define HTTPVERSION_HPP

# include <cctype>	// For isdigit
# include <cstdlib>	// For atoi
# include <string>	// For string

namespace Webserv {

	namespace Http {

		struct HttpVersion {

			enum Version {
				unknown = 0,

				http_0_9 = 9,
				http_1_0 = 10,
				http_1_1 = 11,
				http_2_0 = 20,
				http_3_0 = 30,
			};

			static HttpVersion::Version	getVersion(const std::string& version);

			static std::string			getVersionString(const HttpVersion::Version& version);

		};

	} // namespace Utils

} // namespace Webserv

#endif