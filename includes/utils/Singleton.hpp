/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Singleton.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 15:25:38 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/14 15:34:59 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SINGLETON_HPP
# define SINGLETON_HPP

namespace Webserv {

	namespace Utils {

		template<typename T>
		class Singleton {
			public:
				static T&	getInstance(void) {
					static T instance;
					return (instance);
				}
			private:
				Singleton(void) {};
				Singleton(const Singleton &) {};
				Singleton&	operator=(const Singleton &) {};
		};

	} // namespace Utils

} // namespace Webserv


#endif