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

# include <string>	// For string
# include <map>		// For map

namespace Webserv {

	namespace Http {

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

			static const MethodStringMap	HttpMethodStrings;

			static const MethodStringMap	fillMap(void);

			static HttpMethod::Method		getMethod(const std::string& methodString);

			static std::string				getMethodString(const HttpMethod::Method& method);

		};

	} // namespace Utils

} // namespace Webserv

#endif