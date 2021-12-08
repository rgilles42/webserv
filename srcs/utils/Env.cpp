/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Env.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 17:12:59 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/08 17:44:42 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils/Env.hpp"

namespace Webserv {

	namespace Utils {

		Env::Env(void) {}

		Env::~Env() {}

		bool		Env::isKeyValid(const key_type &key) {
			if (key.empty() || std::isdigit(key[0])) {
				return false;
			}
			size_t	i = 0;
			while (i <= key.length()) {
				if (!std::isalnum(key[i]) && !(key[i] != '_')) {
					return false;
				}
				i++;
			}
			return true;
		}

		bool		Env::set(const key_type &key, const value_type &value) {
			if (!this->isKeyValid(key)) {
				return false;
			}
			this->envArray[key] = value;
			return true;
		}

		bool		Env::append(const key_type &key, const value_type &value) {
			if (!this->isKeyValid(key)) {
				return false;
			}
			if (!this->has(key)) {
				return this->set(key, value);
			}
			this->envArray[key] += value;
			return true;
		}

		const Env::value_type	Env::get(const key_type &key) const {
			if (!this->has(key)) {
				return value_type();
			}
			map_type::const_iterator it = this->envArray.lower_bound(key);
			return it->second;
		}

		bool				Env::has(const key_type &key) const {
			return this->envArray.count(key) > 0;
		}

		char**		Env::toEnvp(void) const {
			char** envp = new char*[this->envArray.size() + 1];
			if (!envp) {
				return NULL;
			}
			size_t i = 0;
			map_type::const_iterator it = this->envArray.begin();
			std::string	tmp;
			while (it != this->envArray.end()) {
				tmp = it->first + "=" + it->second;
				envp[i] = new char[tmp.length() + 1];
				if (!envp) {
					Env::freeEnvp(envp);
					return NULL;
				}
				std::strcpy(envp[i], tmp.c_str());
				it++;
				i++;
			}
			envp[i] = NULL;
			return envp;
		}

		void				Env::fromEnvp(char** envp) {
			if (!envp) {
				return;
			}
			size_t i = 0;
			std::string tmp;
			key_type key;
			value_type value;
			while (envp[i] != NULL) {
				if (envp[i]) {
					tmp = envp[i];
					size_t pos = tmp.find_first_of("=");
					if (pos != tmp.npos) {
						key = tmp.substr(0, pos);
						value = tmp.substr(pos + 1, tmp.length() - (pos + 1));
						this->set(key, value);
					}
				}
				i++;
			}
		}

		void			Env::printEnvp(char** envp) {
			if (!envp) {
				std::cout << "envp is empty" << std::endl;
				return;
			}
			size_t i = 0;
			while (envp[i] != NULL) {
				if (envp[i]) {
					std::cout << envp[i] << std::endl;
				}
				i++;
			}
		}

		void			Env::freeEnvp(char** envp) {
			if (!envp) {
				return;
			}
			size_t i = 0;
			while (envp[i] != NULL) {
				delete[] envp[i];
				envp[i] = NULL;
				i++;
			}
			delete[] envp;
			envp = NULL;
		}

	} // namespace Utils

} // namespace Webserv
