/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Args.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgilles <rgilles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 17:12:59 by ppaglier          #+#    #+#             */
/*   Updated: 2022/02/11 17:46:16 by rgilles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/utils/Args.hpp"

namespace Webserv {

	namespace Utils {

		Args::Args(void) {}
		Args::Args(const Args& other) {
			*this = other;
		}

		Args::~Args() {}

		Args&					Args::operator=(const Args& other) {
			if (this != &other) {
				this->argsArray = other.argsArray;
			}
			return *this;
		}

		bool					Args::set(const key_type& key, const value_type& value) {
			this->argsArray[key] = value;
			return true;
		}

		bool					Args::append(const key_type& key, const value_type& value) {
			if (!this->has(key)) {
				return this->set(key, value);
			}
			this->argsArray[key] += value;
			return true;
		}

		const Args::value_type&	Args::get(const key_type& key) const {
			if (!this->has(key)) {
				throw std::out_of_range("Args::get(" + key + ")");
			}
			map_type::const_iterator it = this->argsArray.lower_bound(key);
			return it->second;
		}

		bool					Args::has(const key_type& key) const {
			return this->argsArray.count(key) > 0;
		}

		const Args::map_type&	Args::getArgs(void) const {
			return this->argsArray;
		}

		void					Args::fromArg(int argc, char **argv) {
			if (argc <= 0 || !argv) {
				return;
			}
			int i = 1;
			std::string tmp;
			key_type key;
			value_type value;
			while (i < argc) {
				if (argv[i]) {
					tmp = argv[i];
					size_t posMinus = tmp.find_first_of("-");
					if (posMinus == tmp.npos) {
						throw BadFormatException(tmp);
					}
					size_t posEqual = tmp.find_first_of("=");
					if (posEqual != tmp.npos) {
						key = tmp.substr(0, posEqual);
						value = tmp.substr(posEqual + 1, tmp.length() - (posEqual + 1));
						this->set(key, value);
					} else {
						if (argc < (i + 1)) {
							throw BadFormatException(tmp);
						}
						key = tmp;
						if (argv[i + 1]) {
							value = argv[i + 1];
						} else {
							value = "";
						}
						this->set(key, value);
						i++;
					}
				}
				i++;
			}
		}

	} // namespace Utils

} // namespace Webserv
