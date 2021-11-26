#include "../includes/CGI.hpp"

namespace webserv
{
    CGI::CGI()
    {
        args[0] = NULL;
        args[1] = NULL;
        args[2] = NULL;

        this->location_cgi = NULL;
        this->location_file = NULL;

        this->m_env = NULL;
        this->fd_save = dup(0);
        if (!fd_save)
            throw dupCGIFailed();
    }

    CGI::~CGI()
    {
        if(fd_save)
            close(fd_save[1]);
        if (fd_return)
            close (fd_return);
    }

    void    CGI::add_env_var(std::string name, std::string value)
    {
        this->m_env.insert(std::make_pair(name, value));
    }

    char    **CGI::env()
    {
        char    **ret;
        std::string str;
        int i = 0;
        std::allocator<char*> alloc;
        std::map<std::string, std::string>::iterator ite;

        ret = this->alloc.allocate(this->m_env.size());
        ite =this->m_env.end();
        for (std::map<std::string, std::string>::iterator it = this->m_env.begin(); it != ite; it++)
        {
            str =*it.first;
            str+= " = ";
            str+= *it.second;
            this->alloc.contruct(&ret[i], str.c_str());
//            ret[i] = str.c_str();
            i++;
            str.clear();
        }
        ret[i] = NULL;
        return ret;
    }

    int CGI::exec()
    {
        pid_t   pid;
        int     status;
        int     fd[2];
        int     ret;

        if(pipe(fd) < 0)
            throw pipeCGIFailed();

        pid = fork();
        if (pid < 0)
        {
            throw pidCGIFailed();
        }
        else (pid == 0)
        {
            close(1);
            close(fd[0]);
            if (dup2(fd[1], 1))
                exit(500);
            ret = execve(this->args[0], this->args, this->env);
            if(ret < 0)
                exit(500);
            exit(0);
        }
        else
        {
            waitpid(pid, &status, 0);
            if (WEXITED(status))
                ret = WEXITSTATUS(status);
            dup2(fd_save, 1)
            close(fd[0]);
        }
        this->fd_return = fd[1];
        return ret;
    }
}
