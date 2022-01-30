/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventsManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/09 15:33:08 by yun               #+#    #+#             */
/*   Updated: 2022/01/30 03:12:12 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTSMANAGER_HPP
# define EVENTSMANAGER_HPP

#include <map>

#include "IEvents.hpp"

namespace Webserv
{
	/* Manager events - create new events object - call functions and more */
	class EventsManager
	{
		private:
			std::map<int, IEvents *> events_map;

		public:
			EventsManager();
			~EventsManager();

			void	add_event(int fd, IEvents &event_object);
			void	remove_event(int fd);

			IEvents	*get_event(int fd);

			std::map<int, IEvents *>::iterator	begin();
			std::map<int, IEvents *>::iterator	end();
	};
}

#endif