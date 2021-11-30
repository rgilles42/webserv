#ifndef FD_TYPES_HPP
# define FD_TYPES_HPP

#include "Poll.hpp"
#include "Socket.hpp"
#include "CGI.hpp"

namespace webserv
{
    class FD_TYPES
    {
        public:
            enum fd_types
            {
                CGI,
                Socket
            };
        private:
            int m_fd;
            FD_TYPES::fd_types m_type;
        public:
        FD_TYPES(int fd, enum FD_TYPES::fd_type) type;
        ~FD_TYPES();

        bool is_socket(int fd);
        bool is_cgi(int fd);
    };
}

#endif