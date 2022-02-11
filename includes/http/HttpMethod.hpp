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

		class HttpMethod {
			public:
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

			protected:
				Method		method;
				std::string	methodString;

				static const MethodStringMap	HttpMethodStrings;

				static const MethodStringMap	fillMap(void);

			public:
				HttpMethod(void);
				HttpMethod(const int& method);
				HttpMethod(const std::string& methodString);
				HttpMethod(const HttpMethod& other);
				~HttpMethod();

				HttpMethod&		operator=(const HttpMethod& other);
				HttpMethod&		operator=(const int& method);
				HttpMethod&		operator=(const std::string& methodString);

				const Method&		getMethod(void) const;
				const std::string&	toString(void) const;

				void				setMethod(const int& method);
				void				setMethod(const std::string& methodString);

				static HttpMethod::Method		getMethod(const int& method);
				static HttpMethod::Method		getMethod(const std::string& methodString);

				static std::string				getMethodString(const HttpMethod::Method& method);

		};

	} // namespace Utils

} // namespace Webserv

#endif