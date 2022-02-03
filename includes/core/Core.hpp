/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Core.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 14:06:38 by ppaglier          #+#    #+#             */
/*   Updated: 2022/01/30 03:04:01 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CORE_HPP
# define CORE_HPP

# include <sstream>						// For ostringstream
# include <vector>						// For vector

# include "Config.hpp"					// For Config
# include "Logger.hpp"					// For Logger
# include "../utils/Singleton.hpp"		// For Singleton
# include "../utils/Env.hpp"			// For Env
# include "../utils/Args.hpp"			// For Args
# include "../events/ClientEvent.hpp"	// For All Events class
# include "../events/ServerEvent.hpp"	// For ??
# include "../events/Poll.hpp"			// For ??
# include "../events/EventsManager.hpp"	// For ??

# define DEFAULT_CONFIG_LOCATION "./conf/webserv.conf"
# define DEFAULT_MIME_TYPES_LOCATION "./conf/mime.types"

namespace Webserv {

	class Core : public Webserv::Utils::Singleton<Core> {

		private:
			Poll			poll_events;
			EventsManager	events_manager;

			void			setup_events(void);

		public:
			typedef Webserv::Config				config_type;
			typedef Webserv::Logger				logger_type;
			typedef Webserv::Utils::Env			env_type;
			typedef Webserv::Utils::Args		args_type;
			typedef Webserv::Utils::Socket		socket_type;

			typedef config_type::server_type	server_type;
			typedef std::vector<server_type>	server_vector;
			typedef std::vector<socket_type>	socket_vector;

			class CoreException : public std::exception {
				protected:
					std::string	msg;

				public:
					CoreException(const std::string& msg = "") : std::exception() {
						this->msg = msg;
					}
					virtual ~CoreException() throw() {}
					virtual const char	*what() const throw() {
						return this->msg.c_str();
					}
			};

			class InitException : public CoreException {
				public:
					InitException() : CoreException() {
						std::ostringstream ss;

						ss << "Init core failed";

						this->msg = ss.str();
					}
			};

			class ExecException : public CoreException {
				public:
					ExecException(const std::string& reason = std::string()) : CoreException() {
						std::ostringstream ss;

						ss << "Exec core failed";

						if (!reason.empty()) {
							ss << ": " << reason;
						}

						this->msg = ss.str();
					}
			};

			class UnknownException : public CoreException {
				public:
					UnknownException() : CoreException() {
						std::ostringstream ss;

						ss << "Unknown core error";

						this->msg = ss.str();
					}
			};

		protected:
			std::string	customConfigFile;
			std::string	customMimeTypesFile;
			config_type config;
			bool		isInit;
			env_type	env;
			args_type	args;
			logger_type	logger;
			server_vector	servers;
			socket_vector	serversSockets;

		public:
			Core(void);
			~Core();

			const logger_type&	getLogger(void) const;

			void		setCustomConfigFile(const std::string& configFile = DEFAULT_CONFIG_LOCATION);
			void		setCustomMimeTypesFile(const std::string& mimeTypesFile = DEFAULT_MIME_TYPES_LOCATION);

			bool		preInit(int argc = 0, char *argv[] = NULL, char *envp[] = NULL);
			bool		init(void);
			void		exec(void);

			void		add_server_event(const socket_type &sock);
			void		add_client_event(int fd, Webserv::ClientEvent &client_e);

			env_type	getEnv(void);

			void		remove_event(int fd);

			const bool&	isReady(void) const;

			std::string	getHelp(void) const;
	};

} // namespace Webserv

#endif
