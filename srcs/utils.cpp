/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 15:47:08 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/25 16:41:16 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/utils.hpp"

std::string	&ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

std::string	&rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

std::string	&trim(std::string &s) {
	return ltrim(rtrim(s));
}

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
	std::string test;
	std::string fileContent(getFileContents("./conf/mime.types"));

	test = "types";
	fileContent = fileContent.substr(fileContent.find(test) + test.length());
	test = "{";
	fileContent = fileContent.substr(fileContent.find(test) + test.length());
	test = "\r\n";
	fileContent = fileContent.substr(fileContent.find(test) + test.length());
	test = "}";
	fileContent = fileContent.substr(0, fileContent.find(test));
	test = "\r\n";
	fileContent = fileContent.substr(0, fileContent.find_last_of(test));

	Webserv::Utils::MimeTypes types(fileContent);
	return types.getType(filename, fallback);
}

bool	isIpv4Address(const std::string& str) {
	struct sockaddr_in sa;
	return (inet_pton(AF_INET, str.c_str(), &(sa.sin_addr)) != 0);
}

bool	isIpv6Address(const std::string& str) {
	struct sockaddr_in6 sa;
	return (inet_pton(AF_INET6, str.c_str(), &(sa.sin6_addr)) != 0);
}

bool	isIpAddress(const std::string& str) {
	return (isIpv4Address(str) || isIpv6Address(str));
}

std::vector<std::string>	split(const std::string &str, const std::string &token) {
	std::string tmp(str);
	std::vector<std::string> result;
	while (tmp.size()) {
		size_t index = tmp.find(token);
		if (index != std::string::npos){
			result.push_back(tmp.substr(0,index));
			tmp = tmp.substr(index+token.size());
			if (tmp.size() == 0) {
				result.push_back(tmp);
			}
		} else {
			result.push_back(tmp);
			tmp = "";
		}
	}
	return result;
}
