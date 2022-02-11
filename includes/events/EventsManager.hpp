/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventsManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/09 15:33:08 by yun               #+#    #+#             */
/*   Updated: 2022/02/11 20:02:30 by ppaglier         ###   ########.fr       */
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
		public:
			typedef std::map<int, IEvents*>	event_map_type;

		private:
			event_map_type events_map;

		public:
			EventsManager();
			~EventsManager();

			void	add_event(int fd, IEvents &event_object);
			void	remove_event(int fd);

			IEvents	*get_event(int fd);

			const event_map_type&		getEvents(void) const;

			event_map_type::iterator	begin();
			event_map_type::iterator	end();
	};
}

#endif