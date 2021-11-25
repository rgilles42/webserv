/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 16:30:57 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/25 16:48:51 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <string>
# include <vector>

# include "./MimeTypes.hpp"

namespace Webserv {

	namespace Utils {

		class Config {
			public:
				typedef	std::string					file_type;
				typedef std::vector<file_type>		file_vector;

				typedef std::string					server_type;
				typedef std::vector<server_type>	server_vector;

				typedef Webserv::Utils::MimeTypes	mimes_types_type;

			protected:
				file_vector			files;
				server_vector		servers;
				mimes_types_type	globalMimesTypes;

			public:
				Config(void);
				~Config();
		};



	} // namespace Utils

} // namespace Webserv

#endif