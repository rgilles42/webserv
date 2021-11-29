#include "../includes/CGI.hpp"

namespace webserv
{
    CGI::CGI()
    {
        args[0] = NULL;
        args[1] = NULL;
        args[2] = NULL;
    }

    CGI::CGI(std::string location, std::string file)
    {
        location_cgi = location;
        location_file = file;
        args[0] = (char*)location_cgi.c_str();
        args[1] = (char*)location_file.c_str();
        args[2] = NULL;
    }

    CGI::~CGI()
    {
        if (this->fd)
            close(this->fd);
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

    void    CGI::readFD()
    {
        ssize_t         size_read = 0;
        char    buffer[2048 + 1];

        size_read = read(this->fd, buffer, 2048 + 1);
        if (size_read < 0)
        {
            perror("recv:");
        }
        std::cout<<size_read<<std::endl;
        buffer[size_read] = '\0';
        this->cgi_message = buffer;
        std::cout<<"read end"<<std::endl;
        close(fd);
    }

    int CGI::exec()
    {
        pid_t   pid;
        int     status;
        int     fd_in[2];
	    int     fd_out[2];
        char    **env;
        int     ret;

        if(pipe(fd_in) < 0)
            throw pipeCGIFailed();
        if(pipe(fd_out) < 0)
            throw pipeCGIFailed();

        env =this->env();
        pid = fork();
        if (pid < 0)
        {
//            free_dtab(env);
            throw pidCGIFailed();
        }
        else (pid == 0)
        {
            close(fd_in[1]);
            if (dup2(fd_in[0], 0) < 0)
            {
//                free_dtab(env);
                throw dupCGIFailed();
            }
            close(fd_in[0]);
            close(fd_out[0]);
            if (dup2(fd_out[1], 1) < 0)
            {
//                free_dtab(env);
                throw dupCGIFailed();
            }
            close(fd_out[1]);
            ret = execve(this->args[0], this->args, this->env());
            if(ret < 0)
                exit(500);
            exit(0);
        }
        else
        {
            this->fd = fd_out[0];
//          if (fcntl(this->fd, F_SETFL,O_NONBLOCK) < 0)
  //            perror("Fnctl: ");
//          select ?
            readFD();
            waitpid(pid, &status, 0);
            if (WIFEXITED(status))
                ret = WEXITSTATUS(status);
            close(fd_in[0]);
            close(fd_in[1]);
            close(fd_out[0]);
            close(fd_out[1]);
        }
//      free_dtab(env);        
        return ret;
    }
}
