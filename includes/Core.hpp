/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Core.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 14:06:38 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/10 14:51:41 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CORE_HPP
# define CORE_HPP

# include <sstream>				// For ostringstream

# include "./utils/Env.hpp"		// For Env
# include "./utils/Args.hpp"	// For Args
# include "Config.hpp"			// For Config

# define DEFAULT_CONFIG_LOCATION "./conf/webserv.conf"
# define DEFAULT_MIME_TYPES_LOCATION "./conf/mime.types"

namespace Webserv {

	class Core {
		public:
			typedef Webserv::Config		config_type;
			typedef Webserv::Utils::Env	env_type;
			typedef Webserv::Utils::Args	args_type;

		protected:
			std::string	customConfigFile;
			std::string	customMimeTypesFile;
			config_type config;
			bool		isInit;
			env_type	env;
			args_type	args;

		public:
			Core(void);
			~Core();

			void		setCustomConfigFile(const std::string& configFile = DEFAULT_CONFIG_LOCATION);
			void		setCustomMimeTypesFile(const std::string& mimeTypesFile = DEFAULT_MIME_TYPES_LOCATION);

			bool		preInit(int argc = 0, char *argv[] = NULL, char *envp[] = NULL);
			bool		init(void);

			const bool&	isReady(void) const;

			std::string	getHelp(void) const;
	};

} // namespace Webserv

#endif