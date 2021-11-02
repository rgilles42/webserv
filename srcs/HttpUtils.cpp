/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpUtils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 01:17:46 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/02 02:13:16 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/HttpUtils.hpp"

std::string	HttpStatusCode::getStatusCodeString(const HttpStatusCode::StatusCode &statusCode) {
	if (HttpStatusCodeString.count(statusCode) <= 0) {
		return HttpStatusCodeString.at(HttpStatusCode::unknown);
	}
	return HttpStatusCodeString.at(statusCode);
}

HttpStatusCode::StatusCode	HttpStatusCode::getStatusCode(const int &statusCode) {
	const HttpStatusCode::StatusCode newStatusCode = (HttpStatusCode::StatusCode)statusCode;
	return newStatusCode;
}

HttpStatusCode::StatusCode	HttpStatusCode::getStatusCode(const std::string &statusCodeString) {
	HttpStatusCode::StatusCodeStringMap::const_iterator it = HttpStatusCodeString.begin();

	while (it != HttpStatusCodeString.end()) {
		if (it->second == statusCodeString) {
			return it->first;
		}
		it++;
	}
	return HttpStatusCode::unknown;
}
