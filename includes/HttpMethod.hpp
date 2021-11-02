/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpMethod.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 00:38:38 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/02 19:32:58 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPMETHOD_HPP
# define HTTPMETHOD_HPP

# include <string>
# include <map>

struct HttpMethod {

	enum Method {
		unknown = 0,

		GET,
		POST,
		DELETE,

		PATCH,
		PUT,
		HEAD,
		OPTIONS,
		CONNECT,
		TRACE,
	};

	typedef std::map<HttpMethod::Method, std::string> MethodStringMap;

	static HttpMethod::Method	getMethod(const std::string &methodString);

	static std::string			getMethodString(const HttpMethod::Method &method);

};

static const std::pair<HttpMethod::Method, std::string> HttpMethodStringData[] = {
	std::make_pair(HttpMethod::unknown, ""),

	std::make_pair(HttpMethod::GET, "GET"),
	std::make_pair(HttpMethod::POST, "POST"),
	std::make_pair(HttpMethod::DELETE, "DELETE"),

	std::make_pair(HttpMethod::PATCH, "PATCH"),
	std::make_pair(HttpMethod::PUT, "PUT"),
	std::make_pair(HttpMethod::HEAD, "HEAD"),
	std::make_pair(HttpMethod::OPTIONS, "OPTIONS"),
	std::make_pair(HttpMethod::CONNECT, "CONNECT"),
	std::make_pair(HttpMethod::TRACE, "TRACE"),
};

static const HttpMethod::MethodStringMap HttpMethodStrings(HttpMethodStringData, HttpMethodStringData + (sizeof HttpMethodStringData / sizeof HttpMethodStringData[0]));

#endif