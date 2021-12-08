/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Env.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 16:19:25 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/08 17:33:03 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_HPP
# define ENV_HPP

# include <cstring>
# include <string>
# include <map>

# include <iostream>

namespace Webserv {

	namespace Utils {

		class Env {
			public:
				typedef std::string						key_type;
				typedef std::string						value_type;

				typedef std::map<key_type, value_type>	map_type;

			protected:
				map_type	envArray;

			public:
				Env(void);
				virtual ~Env();


				bool				isKeyValid(const key_type &key);
				bool				set(const key_type &key, const value_type &value = "");
				bool				append(const key_type &key, const value_type &value);
				const value_type	get(const key_type &key) const;
				bool				has(const key_type &key) const;

				// Utils Methods
				char**				toEnvp(void) const;
				void				fromEnvp(char** envp);

				static void			printEnvp(char** envp);

				static void			freeEnvp(char** envp);

		};

	} // namespace Utils

} // namespace Webserv


#endif