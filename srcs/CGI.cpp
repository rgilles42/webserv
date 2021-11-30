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

    void    init_env_var()
    {
        /*--------*/
        /* SERVER */
        /*--------*/

        add_env_var("SERVER_SOFTWARE", "Webserv/HTTP/1.1");
//        add_env_var("SERVER_NAME", this->srv.name());
        add_env_var("GATEWAY_INTERFACE", "CGI/1.1");

        /*---------*/
        /* Request */
        /*---------*/

        add_env_var("SERVER_PROTOCOL", )
//        add_env_var("SERVER_PORT", srv.port());
//        add_env_var("REQUEST_METHODS",this->request.methods());
//        add_env_var("PATH_INFO", this->request.path_info());
        add_env_var("PATH_TRANSLATED", "") //? Need more infos
        add_env_var("SCRIPT_NAME", this->location_cgi.c_str());
//        add_env_var("QUERY_STRING", this->request.query_string());
//        add_env_var("REMOTE_HOTE", this->request.hote_client());
//        add_env_var("REMOTE_ADDR", this->request.client_add());
        add_env_var("AUTH_SCRIPT", "") //? Need more infos
        add_env_var("REMOTE_USER", this->request.client_user()); //if script protect and srv allow identification
//        add_env_var("CONTENT_TYPE", this->request.content_type());
        add_env_var("CONTENT_LENGHT", this->request.content_lenght());

        /*--------*/
        /* Client */
        /*--------*/

//        add_env_var("HTTP_ACCEPT", this->request.http_accept());
//        add_env_var("HTTP_ACCEPT_LANGUAGE", this->request.http_accept_language());
//        add_env_var("HTTP_USER_AGENT", this->request.navigateur());
//        add_env_var("HTTP_COOKIE", this->request.cookie());   //BONUS
        add_env_var("HTTP_COOKIE", "");
        add_env_var("HTTP_REFERER", ""); //? Need more infos
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

    void    CGI::free_dtab(char **tab)
    {
        int i;

        i = 0;
        while (tab[i] != NULL)
        {
            free(tab[i]);
            i++;
        }
        free(tab);
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
         {
            close(fd_in[0]);
            close(fd_in[1]);
            throw pipeCGIFailed();
         }

        this->poll_cgi.add_fd(this->fd, POLLIN);

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
                close(fd_in[0]);
                close(fd_out[0]);
                close(fd_out[0]);
                free_dtab(env);
                throw dupCGIFailed();
            }
            close(fd_in[0]);
            close(fd_out[0]);
            if (dup2(fd_out[1], 1) < 0)
            {
                close(fd_in[0]);
                close(fd_out[1]);
                free_dtab(env);
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
            waitpid(pid, &status, 0);
            if (WIFEXITED(status))
                ret = WEXITSTATUS(status);
            //this->fd = fd_out[0] ?
            close(fd_in[0]);
            close(fd_in[1]);
            close(fd_out[1]);
        }
        free_dtab(env);        
        return ret;
    }
}
