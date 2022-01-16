/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Core.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 14:06:38 by ppaglier          #+#    #+#             */
/*   Updated: 2022/01/10 15:46:59 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CORE_HPP
# define CORE_HPP

# include <sstream>					// For ostringstream

# include "../utils/Env.hpp"		// For Env
# include "../utils/Args.hpp"		// For Args
# include "../Config.hpp"			// For Config
# include "../Logger.hpp"			// For Logger
# include "../events/ClientEvent.hpp"	// For All Events class
# include "../events/ServerEvent.hpp"	//
# include "../events/Poll.hpp"			//
# include "../events/EventsManager.hpp"
# include "../utils/Singleton.hpp"	// For Singleton

# define DEFAULT_CONFIG_LOCATION "./conf/webserv.conf"
# define DEFAULT_MIME_TYPES_LOCATION "./conf/mime.types"

namespace Webserv {

	class Core : public Webserv::Utils::Singleton<Core> {

		private:
			Poll			poll_events;
			EventsManager	events_manager;

			void			setup_events(void);

		public:
			typedef Webserv::Config		config_type;
			typedef Webserv::Logger		logger_type;
			typedef Webserv::Utils::Env	env_type;
			typedef Webserv::Utils::Args	args_type;

		protected:
			std::string	customConfigFile;
			std::string	customMimeTypesFile;
			config_type config;
			bool		isInit;
			env_type	env;
			args_type	args;
			logger_type	logger;

		public:
			Core(void);
			~Core();

			const logger_type&	getLogger(void) const;

			void		setCustomConfigFile(const std::string& configFile = DEFAULT_CONFIG_LOCATION);
			void		setCustomMimeTypesFile(const std::string& mimeTypesFile = DEFAULT_MIME_TYPES_LOCATION);

			bool		preInit(int argc = 0, char *argv[] = NULL, char *envp[] = NULL);
			bool		init(void);
			void		exec(void);

			void		add_server_event(Socket &sock);
			void		add_client_event(int fd, Webserv::ClientEvent &client_e);
//			void		add_cgi_event(int fd, CGIEvent &cgi_e);

			void		remove_event(int fd);

			const bool&	isReady(void) const;

			std::string	getHelp(void) const;

			struct coreInitFailed : public std::exception
			{
				virtual const char* what() const throw()
				{
					return ("Core: initialisation failed");
				}
			};

			struct coreExecFailed : public std::exception
			{
				virtual const char* what() const throw()
				{
					return ("Core: initialisation failed");
				}
			};
	};

} // namespace Webserv

#endif
