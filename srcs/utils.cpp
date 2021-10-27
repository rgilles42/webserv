/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 15:47:08 by ppaglier          #+#    #+#             */
/*   Updated: 2021/10/27 16:09:07 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/utils.hpp"

std::string	getFormatedDate(const time_t &rawtime) {
	struct tm *timeinfo;
	char buffer[80];

	timeinfo = gmtime(&rawtime);
	strftime(buffer, sizeof(buffer), "%a, %m %b %Y %H:%M:%S %Z", timeinfo);
	return std::string(buffer);
}


std::string	getFileContents(const std::string &filename) {
	std::ifstream in(filename.c_str(), std::ios::in | std::ios::binary);
	if (in) {
		return (std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>()));
	}
	throw (std::runtime_error("getFileContents"));
}

std::string	getFileExtension(const std::string &filename) {
	return filename.substr(filename.find_last_of(".") + 1);
}

std::string getContentTypeByFile(const std::string &filename, const std::string &fallback) {
	std::string fileExtension = getFileExtension(filename);

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