/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHeaders.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 16:45:31 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/01 18:25:44 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/HttpHeaders.hpp"

// can have multiple identique key but not same value

// Headers Methods
bool		HttpHeaders::isKeyValid(const std::string &key) {
	for (size_t i = 0; i < key.length(); i++) {
		if (!IS_AUTHORIZED_KEY_CHAR(key[i])) {
			return false;
		}
	}
	return true;
}

void		HttpHeaders::set(const std::string &key, const std::string &value) {
	if (!this->isKeyValid(key)) {
		throw std::runtime_error("isKeyValid: " + key);
	}
	this->headers[key] = value;
}

const std::string	HttpHeaders::get(const std::string &key) const {
	return this->headers.at(key);
}

std::string	HttpHeaders::toString(void) const {
	std::map<std::string, std::string>::const_iterator it = this->headers.begin();
	std::map<std::string, std::string>::const_iterator end = this->headers.end();
	std::string	formatedHeaders = "";

	while (it != end) {
		formatedHeaders += it->first + ": " + it->second + LINE_BREAK;
		it++;
	}
	return formatedHeaders;
}

void	HttpHeaders::fromString(const std::string &stringHeaders) {
	std::string			line;
	std::stringstream	ss(stringHeaders);

	while (std::getline(ss, line)) {
		line = trim(line);
		std::string key = line.substr(0, line.find(':'));
		line = line.erase(0, line.find(':') + 1);
		std::string value = line;
		this->set(trim(key), trim(value));
	}
}
