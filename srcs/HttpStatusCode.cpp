/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpStatusCode.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 01:17:46 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/02 20:05:07 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/HttpStatusCode.hpp"

HttpStatusCode::StatusCode	HttpStatusCode::getStatusCode(const int &statusCode) {
	const HttpStatusCode::StatusCode newStatusCode = static_cast<HttpStatusCode::StatusCode>(statusCode);
	return newStatusCode;
}

HttpStatusCode::StatusCode	HttpStatusCode::getStatusCode(const std::string &statusCodeString) {
	HttpStatusCode::StatusCodeStringMap::const_iterator it = HttpStatusCodeStrings.begin();

	while (it != HttpStatusCodeStrings.end()) {
		if (it->second == statusCodeString) {
			return it->first;
		}
		it++;
	}
	return HttpStatusCode::unknown;
}

std::string					HttpStatusCode::getStatusCodeString(const HttpStatusCode::StatusCode &statusCode) {
	if (HttpStatusCodeStrings.count(statusCode) <= 0) {
		return HttpStatusCodeStrings.at(HttpStatusCode::unknown);
	}
	return HttpStatusCodeStrings.at(statusCode);
}

bool						HttpStatusCode::isInformation(const HttpStatusCode::StatusCode &statusCode) {
	int newStatusCode = static_cast<int>(statusCode);
	return (HttpStatusCode::information_offset <= newStatusCode && newStatusCode < HttpStatusCode::success_offset);
}

bool						HttpStatusCode::isSuccess(const HttpStatusCode::StatusCode &statusCode) {
	int newStatusCode = static_cast<int>(statusCode);
	return (HttpStatusCode::success_offset <= newStatusCode && newStatusCode < HttpStatusCode::redirection_offset);
}

bool						HttpStatusCode::isRedirect(const HttpStatusCode::StatusCode &statusCode) {
	int newStatusCode = static_cast<int>(statusCode);
	return (HttpStatusCode::redirection_offset <= newStatusCode && newStatusCode < HttpStatusCode::client_error_offset);
}

bool						HttpStatusCode::isClientError(const HttpStatusCode::StatusCode &statusCode) {
	int newStatusCode = static_cast<int>(statusCode);
	return (HttpStatusCode::client_error_offset <= newStatusCode && newStatusCode < HttpStatusCode::server_error_offset);
}

bool						HttpStatusCode::isServerError(const HttpStatusCode::StatusCode &statusCode) {
	int newStatusCode = static_cast<int>(statusCode);
	return (HttpStatusCode::server_error_offset <= newStatusCode && newStatusCode < HttpStatusCode::misc_error_offset);
}

