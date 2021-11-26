/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directive.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 16:25:51 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/26 10:37:24 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTIVE_HPP
# define DIRECTIVE_HPP

# include <string>
# include <vector>

namespace Webserv {

	namespace Utils {

		class IDirective {
			public:
				typedef std::string					name_type;
				typedef	std::vector<std::string>	context_vector;
				typedef std::pair<int, int>			argc_type;
				typedef	std::string					value_type;
				typedef	std::string					src_value_type;

			protected:
				const name_type			name;
				const context_vector	contexts;
				const argc_type			argc;
				const value_type		defaultValue;
				value_type				value;

			public:
				virtual const context_vector	fillContexts(void) const {
					context_vector newContext;
					return newContext;
				};

				IDirective(void) : contexts(this->fillContexts()) {}

				virtual ~IDirective() {};

				virtual const name_type			getName(void) const {
					return this->name;
				}
				virtual const context_vector	getContexts(void) const {
					return this->contexts;
				}
				virtual const argc_type			getArgc(void) const {
					return this->argc;
				}
				virtual const value_type		getdefaultValue(void) const {
					return this->defaultValue;
				}

				virtual const value_type		parseValue(const src_value_type &src) const = 0;
		};

	} // namespace Utils

} // namespace Webserv

#endif