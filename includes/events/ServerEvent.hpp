/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerEvent.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgilles <rgilles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/09 15:33:08 by yun               #+#    #+#             */
/*   Updated: 2022/02/09 23:46:38 by rgilles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVEREVENT_HPP
# define SERVEREVENT_HPP

# include <poll.h>
# include <iostream>

# include "IEvents.hpp"
# include "ClientEvent.hpp"
# include "../core/Core.hpp"
# include "../core/Config.hpp"
# include "../utils/Socket.hpp"
# include "../utils/Env.hpp"

namespace Webserv
{
	class ServerEvent : public IEvents {
		public:
			typedef Webserv::Utils::Socket	socket_type;
			typedef Webserv::Config			config_type;
			typedef	Webserv::Utils::Env		env_type;
			typedef Webserv::Logger			logger_type;

		private:
			socket_type			sock;
			config_type&		config;
			env_type			env;
			short				events_flags;
			logger_type			logger;

		public:
			ServerEvent(const socket_type &new_socket, config_type &_config, Utils::Env& environnement, logger_type& logger);
			~ServerEvent();

			void	write_event(void);
			void	read_event(void);

			short	getEventsFlags(void);
			int		getFD(void);
	};
}
#endif