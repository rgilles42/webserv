/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Core.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 14:06:38 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/03 14:37:24 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CORE_HPP
# define CORE_HPP

# include "Config.hpp"

# define DEFAULT_CONFIG_LOCATION "./conf/webserv.conf"
# define DEFAULT_MIME_TYPES_LOCATION "./conf/mime.types"

namespace Webserv {

	class Core {
		public:
			typedef Webserv::Config	config_type;

		protected:
			std::string	customConfigFile;
			std::string	customMimeTypesFile;
			config_type config;
			bool		isInit;

		public:
			Core(void);
			~Core();

			void		setCustomConfigFile(const std::string &configFile = DEFAULT_CONFIG_LOCATION);
			void		setCustomMimeTypesFile(const std::string &mimeTypesFile = DEFAULT_MIME_TYPES_LOCATION);

			bool		init(void);

			const bool	&isReady(void) const;
	};

} // namespace Webserv

#endif