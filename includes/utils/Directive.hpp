/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directive.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 16:25:51 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/23 16:50:33 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTIVE_HPP
# define DIRECTIVE_HPP

# include <string>
# include <vector>

namespace Webserv {

	namespace Utils {

		class Directive {
			public:
				typedef std::string					name_type;
				typedef	std::vector<std::string>	context_vector;
				typedef	std::size_t					argc_type;
				typedef	std::string					argv_type;

			protected:
				name_type		name;
				context_vector	contexts;
				argc_type		minArgs;
				argv_type		defaultArgs;

			public:
				Directive(void);
				~Directive();
		};

		Directive::Directive(void)
		{
		}

		Directive::~Directive()
		{}


	} // namespace Utils

} // namespace Webserv

#endif