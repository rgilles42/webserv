/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yun <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 16:50:50 by yun               #+#    #+#             */
/*   Updated: 2021/11/24 17:05:17 by yun              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

namespace webserv
{
	class CGI
	{
		private:
			std::string	location_cgi;
			std::string location_file;
			char	*args[3];
			char	**env;

		public:
			CGI();
			CGI(std::string path_cgi, std::string location_file);
			~CGI();


			int	exec();
			int		set_env_var(std::string var);
			char	*message();

		private:
	}
}

#endif
