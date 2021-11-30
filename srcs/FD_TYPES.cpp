#include "../includes/FD_TYPES.hpp"

namespace webserv
{
    FD_TYPES::FD_TYPES(int fd, enum FD_TYPES::fd_type type): m_fd(fd), m_type(type) {}

    FD_TYPES::~FD_TYPES();

    bool    FD_TYPES::is_socket()
    {
        if (this->type == FD_TYPES::Socket)
                return true;
        return false;
    }
}