/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UploadEvent.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 15:06:39 by yun               #+#    #+#             */
/*   Updated: 2022/01/30 03:14:04 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <iostream>
# include <unistd.h>
# include <string>

# include "IEvents.hpp"

namespace Webserv {

	class UploadEvent : public IEvents
	{
		public:
			UploadEvent(std::string content, int fd);
			~UploadEvent();

			void    write_event();
			short getEventsFlags();

		private:
			int m_fd;
			std::string m_content;
			short eventFlag;
			size_t write_size;

	};

}   // namespace Webserv
