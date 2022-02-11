/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Env.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 16:19:25 by ppaglier          #+#    #+#             */
/*   Updated: 2022/01/29 22:21:43 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_HPP
# define ENV_HPP

# include <cctype>		// For isalnum, isdigit
# include <cstring>		// For strcpy
# include <string>		// For string
# include <map>			// For map
# include <stdexcept>	// For runtime_error, out_of_range

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
				Env(const Env& other);
				virtual ~Env();

				Env&				operator=(const Env& other);

				bool				isKeyValid(const key_type& key);
				void				set(const key_type& key, const value_type& value = "");
				void				append(const key_type& key, const value_type& value);
				const value_type&	get(const key_type& key) const;
				bool				has(const key_type& key) const;

				// Utils Methods
				char**				toEnvp(void) const;
				void				fromEnvp(char** envp);

				static void			freeEnvp(char** envp);

		};

	} // namespace Utils

} // namespace Webserv


#endif