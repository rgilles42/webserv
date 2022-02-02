/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpVersion.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 00:38:38 by ppaglier          #+#    #+#             */
/*   Updated: 2022/02/02 16:31:20 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPVERSION_HPP
# define HTTPVERSION_HPP

# include <cctype>	// For isdigit
# include <cstdlib>	// For atoi
# include <string>	// For string

namespace Webserv {

	namespace Http {

		class HttpVersion {
			public:

				enum Version {
					unknown = 0,

					http_0_9 = 9,
					http_1_0 = 10,
					http_1_1 = 11,
					http_2_0 = 20,
					http_3_0 = 30,
				};

			protected:
				Version	version;

			public:
				HttpVersion(void);
				HttpVersion(const int& version);
				HttpVersion(const std::string& versionString);
				HttpVersion(const HttpVersion& other);
				~HttpVersion();

				HttpVersion&		operator=(const HttpVersion& other);
				HttpVersion&		operator=(const int& version);
				HttpVersion&		operator=(const std::string& versionString);

				const HttpVersion::Version&	getVersion(void) const;
				const std::string			getVersionString(void) const;

				void						setVersion(const int& version);
				void						setVersion(const std::string& versionString);

				static HttpVersion::Version	getVersion(const int& version);
				static HttpVersion::Version	getVersion(const std::string& version);

				static std::string			getVersionString(const HttpVersion::Version& version);
		};

	} // namespace Utils

} // namespace Webserv

#endif