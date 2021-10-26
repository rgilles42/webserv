/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 15:15:41 by ppaglier          #+#    #+#             */
/*   Updated: 2021/10/26 15:45:33 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_HPP
# define HEADER_HPP

#include <string>
#include <stdint.h>
#include <map>

#include "utils.hpp"

# define HTTP_VERSION "HTTP/1.1"

class Header {

	protected:
		std::string							statusCode;
		std::map<std::string, std::string>	headers;

		void	setKeyValue(const std::string &key, const std::string &value);

	public:
		void	initForSend();

		const std::string	getKeyValue(const std::string &key);

		void		setStatusCode(const std::string &statusCode = "200 OK");
		void		setContentType(const std::string &contentType = "text/plain");
		void		setContentHash(const uint32_t &contentHash = 0);
		void		setContentLength(const size_t &contentLength = 0);

		std::string		getFormatedHeader(void) const;

};

#endif