/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectiveServerName.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 22:46:58 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/06 23:56:21 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTIVE_SERVER_NAME
# define DIRECTIVE_SERVER_NAME

# include "../IDirective.hpp"

namespace Webserv {

	namespace Utils {

		class DirectiveServerName : public Webserv::Utils::IDirective {
			public:
				typedef	std::string					value_type;

			protected:
				value_type	defaultValue;
				value_type	value;

				const context_vector	fillContexts(void) const {
					context_vector newContext;
					newContext.push_back("server");
					std::cout << "origin" << std::endl;
					return newContext;
				};

			public:
				DirectiveServerName() : IDirective("server_name", std::make_pair(0, 1), "") {}
				virtual ~DirectiveServerName() {}

				const value_type		&getValue(void) const {
					return this->value;
				}

				void	parseValue(const src_value_type &src) {
					this->value = this->defaultValue;
					if (src.size() > 1) {
						this->value = src[1];
					}
				};
		};

	} // namespace Utils

} // namespace Webserv

#endif