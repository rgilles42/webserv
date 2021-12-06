/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IDirective.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 16:25:51 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/06 23:56:02 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTIVE_HPP
# define DIRECTIVE_HPP

# include <string>
# include <vector>
# include <iostream>

namespace Webserv {

	namespace Utils {

		class IDirective {
			public:
				typedef std::string					name_type;
				typedef	std::vector<std::string>	context_vector;
				typedef std::pair<int, int>			argc_type;
				typedef	std::string					value_type;
				typedef	std::vector<std::string>	src_value_type;

			protected:
				const name_type			name;
				const context_vector	contexts;
				const argc_type			argc;
				const value_type		defaultValue;
				value_type				value;

				virtual const context_vector	fillContexts(void) const {
					context_vector newContext;
					std::cout << "sub" << std::endl;
					return newContext;
				};

			public:

				IDirective(const name_type &name = name_type(), const argc_type &argc = argc_type(-1, -1), const value_type &defaultValue = value_type()) : name(name), argc(argc), defaultValue(defaultValue) {};

				virtual ~IDirective() {};

				virtual const name_type			&getName(void) const {
					return this->name;
				}
				virtual const context_vector	&getContexts(void) const {
					return this->contexts;
				}
				virtual const argc_type			&getArgc(void) const {
					return this->argc;
				}
				virtual const value_type		&getDefaultValue(void) const {
					return this->defaultValue;
				}
				virtual const value_type		&getValue(void) const {
					return this->value;
				}

				virtual void					parseValue(const src_value_type &src) = 0;
		};

	} // namespace Utils

} // namespace Webserv

#endif