/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directive.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 15:13:45 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/07 16:12:44 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTIVE_HPP
# define DIRECTIVE_HPP

# include <string>
# include <vector>
# include <map>
# include <algorithm>

# include <iostream>

# include "Token.hpp"

namespace Webserv {

	namespace Utils {

		class Directive {
			public:
				typedef std::string					name_type;
				typedef std::string					context_type;
				typedef	std::vector<context_type>	context_vector;
				typedef std::pair<size_t, size_t>	argc_type;
				typedef Webserv::Utils::Token		token_type;
				typedef	std::vector<token_type>		src_value_type;

				typedef std::map<name_type, Directive>	directive_map;

			protected:
				name_type			name;
				argc_type			argc;
				context_vector	contexts;

			public:
				Directive(const name_type &name = name_type(), const argc_type &argc = argc_type(-1, -1), const context_vector &contexts = context_vector()) : name(name), argc(argc), contexts(contexts) {};

				virtual ~Directive() {};

				const name_type			&getName(void) const {
					return this->name;
				}
				const context_vector	&getContexts(void) const {
					return this->contexts;
				}
				const argc_type			&getArgc(void) const {
					return this->argc;
				}

				bool					isContextValid(const context_type &context) const {
					context_vector::const_iterator it = std::find(this->contexts.begin(), this->contexts.end(), context);
					return it != this->contexts.end();
				}

				bool					isSrcValueIsValid(const src_value_type &src) const {
					size_t size = src.size();
					if (size > 0) {
						if (src[0].getValue() != this->name) {
							return false;
						}
						// Don't count the directive keyword
						size -= 1;
					}
					if (this->argc.first != (size_t)-1 && size < this->argc.first) {
						return false;
					}
					if (this->argc.second != (size_t)-1 && size > this->argc.second) {
						return false;
					}
					return true;
				}
		};

	} // namespace Utils

} // namespace Webserv

#endif