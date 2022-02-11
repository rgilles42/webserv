/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IEvents.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/09 15:33:08 by yun               #+#    #+#             */
/*   Updated: 2022/01/30 03:11:27 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IEVENTS_HPP
# define IEVENTS_HPP

namespace Webserv
{
	/* Interface use for all events class */
	class IEvents
	{
		public:
			virtual			~IEvents() {}
			virtual void	read_event() = 0;
			virtual void	write_event() = 0;
			virtual short	getEventsFlags() = 0;
			virtual int		getFD() = 0;
	};
}

#endif
