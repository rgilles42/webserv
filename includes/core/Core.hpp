/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Core.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 14:06:38 by ppaglier          #+#    #+#             */
/*   Updated: 2022/01/09 15:36:21 by yun              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CORE_HPP
# define CORE_HPP

# include <sstream>					// For ostringstream

# include "../utils/Env.hpp"		// For Env
# include "../utils/Args.hpp"		// For Args
# include "../Config.hpp"			// For Config
# include "../Logger.hpp"			// For Logger
# include "./Singleton.hpp"			// For Singleton
# include "../events/events.hpp"		// For All Events class


# define DEFAULT_CONFIG_LOCATION "./conf/webserv.conf"
# define DEFAULT_MIME_TYPES_LOCATION "./conf/mime.types"

namespace Webserv {
	/* Inherit Singletons to instanciate 1 time and use in other class */
	class Core : public Singleton<Core> {

		private:
			Poll			*poll_events;
			EventsManager	*events_manager;

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
			void		add_client_event(int fd, ClientEvent &client_e);
			void		add_cgi_event(CGIEvent const &new_cgi);

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
