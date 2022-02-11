/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Args.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 16:19:25 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/08 17:33:03 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARGS_HPP
# define ARGS_HPP

# include <string>		// For string
# include <map>			// For map
# include <exception>	// For exception
# include <sstream>		// For stringstream

namespace Webserv {

	namespace Utils {

		class Args {
			public:
				typedef std::string						key_type;
				typedef std::string						value_type;

				typedef std::map<key_type, value_type>	map_type;

				class ArgException : public std::exception {
					protected:
						std::string	msg;
						key_type	arg;

					public:
						ArgException(const key_type& arg = key_type(), const std::string& msg = "") : std::exception() {
							this->msg = msg;
							this->arg = arg;
						}
						virtual ~ArgException() throw() {}
						const key_type	getArg() const {
							return this->arg;

						}
						virtual const char	*what() const throw() {
							return this->msg.c_str();
						}
				};

				class BadFormatException : public ArgException {
					public:
						BadFormatException(const key_type& arg = key_type()) : ArgException(arg) {
							std::ostringstream ss;

							ss << "Bad format for argument \"" << this->arg << "\"";

							this->msg = ss.str();
						}
				};

				class UnknownArgException : public ArgException {
					public:
						UnknownArgException(const key_type& arg = key_type()) : ArgException(arg) {
							std::ostringstream ss;

							ss << "Unknown argument \"" << this->arg << "\"";

							this->msg = ss.str();
						}
				};

			protected:
				map_type	argsArray;

			public:
				Args(void);
				Args(const Args& other);
				virtual ~Args();

				Args&				operator=(const Args& other);

				bool				set(const key_type& key, const value_type& value = "");
				bool				append(const key_type& key, const value_type& value);
				const value_type&	get(const key_type& key) const;
				bool				has(const key_type& key) const;

				const map_type&		getArgs(void) const;

				// Utils Methods
				void				fromArg(int argc = 0, char **argv = NULL);

		};

	} // namespace Utils

} // namespace Webserv


#endif