/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UploadEvent.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yun <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 15:10:39 by yun               #+#    #+#             */
/*   Updated: 2022/01/21 15:10:40 by yun              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../includes/events/UploadEvent.hpp"

namespace Webserv {

    UploadEvent::UploadEvent(std::string content, int fd): m_content(content), m_fd(fd)
    {
        this->eventFlag = POLLOUT;
        this->write_size = 0;
    }

    UploadEvent::~UploadEvent(void)
    {
        if (this->m_fd)
        {
            close(m_fd);
        }
    }

    void    UploadEvent::write_event()
    {
        ssize_t ret;

        if (write_size >= this->m_content.size())
            return;
        ret = write(this->m_fd, &m_content[], this->m_content.size() - write_size);
        if (ret < 0)
        {
            std::cout<<"Error UploadEvent write"<<std::endl;
            exit(1);
        }
        write_size += static_cast<size_t>(ret);
        if (this->write_size == this->m_content.size())
        {
            std::cout<<"upload end"<<std::endl;
            exit(0);
        }
    }
}   // namespace Webserv