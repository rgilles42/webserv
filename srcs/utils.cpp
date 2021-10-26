/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 15:47:08 by ppaglier          #+#    #+#             */
/*   Updated: 2021/10/26 17:27:33 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include <ctime>

#include "../includes/utils.hpp"

std::string	getFormatedDate(const time_t &rawtime) {
	struct tm *timeinfo;
	char buffer[80];

	timeinfo = gmtime(&rawtime);

	strftime(buffer, sizeof(buffer), "%a, %m %b %Y %H:%M:%S %Z", timeinfo);

	return std::string(buffer);
}

std::string getContentTypeByFileExtension(const std::string &url, const std::string &fallback) {
	std::string fileExtension = url.substr(url.find_last_of(".") + 1);

	if (fileExtension == "txt") {
		return "text/plain";
	}
	if ((fileExtension == "htm") || (fileExtension == "html")) {
		return "text/html";
	}
	if (fileExtension == "css") {
		return "text/css";
	}
	if (fileExtension == "js") {
		return "text/javascript";
	}

	if (fileExtension == "gif") {
		return "image/gif";
	}
	if (fileExtension == "png") {
		return "image/png";
	}
	if ((fileExtension == "jpg") || (fileExtension == "jpeg")) {
		return "image/jpeg";
	}
	if (fileExtension == "bmp") {
		return "image/bmp";
	}
	if (fileExtension == "ico") {
		return "image/x-icon";
	}

	return fallback;
}