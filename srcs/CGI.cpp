#include "../includes/CGI.hpp"

namespace webserv
{
    CGI::CGI()
    {
        args[0] = NULL;
        args[1] = NULL;
        args[2] = NULL;

        this->fd_save = dup(0);
        if (!fd_save)
            throw dupCGIFailed();
    }

    CGI::CGI(std::string location, std::string file)
    {
        location_cgi = location;
        location_file = file;
        args[0] = (char*)location_cgi.c_str();
        args[1] = (char*)location_file.c_str();
        args[2] = NULL;

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

    void   CGI::set_args(std::string location, std::string file)
    {
        location_cgi = location;
        location_file = file;
        args[0] = (char*)location_cgi.c_str();
        args[1] = (char*)location_file.c_str();
        args[2] = NULL;
    }

    char    **CGI::env()
    {
        char    **ret;
        size_t i = 0;
        std::map<std::string, std::string>::iterator ite = this->m_env.end();

        ret = (char**)malloc(this->m_env.size() * sizeof(ret));
        for(std::map<std::string, std::string>::iterator it = this->m_env.begin(); it != ite; it++)
        {
            ret[i] = (char *)std::string(it->first + "=" + it->second).c_str();
            i++;
        }
        ret[i] = NULL;
        return ret;
    }

    int CGI::exec()
    {
        pid_t   pid;
        int     status;
        int     fd_in[2];
	int	fd_out[2];
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
            close(fd_in[1]);
	    dup2(fd_in[0], 0);
	    close(fd_in[0]);
            close(fd_out[0]);
	    dup2(fd_out[1], 1);
            close(fd_out[1]);
            ret = execve(this->args[0], this->args, this->env());
            if(ret < 0)
                exit(500);
            exit(0);
        }
        else
        {
//	    this->fd = fd_out[0];
//	    fnctl();
//          select()
//          read
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
