/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpMethod.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 01:17:46 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/02 19:53:35 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/HttpMethod.hpp"

HttpMethod::Method	HttpMethod::getMethod(const std::string &methodString) {
	HttpMethod::MethodStringMap::const_iterator it = HttpMethodStrings.begin();

	while (it != HttpMethodStrings.end()) {
		if (it->second == methodString) {
			return it->first;
		}
		it++;
	}
	return HttpMethod::unknown;
}

std::string			HttpMethod::getMethodString(const HttpMethod::Method &method) {
	if (HttpMethodStrings.count(method) <= 0) {
		return HttpMethodStrings.at(HttpMethod::unknown);
	}
	return HttpMethodStrings.at(method);
}
