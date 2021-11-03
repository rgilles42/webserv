/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ressource.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 15:15:41 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/03 19:00:56 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESSOURCE_HPP
# define RESSOURCE_HPP

#include <string>
#include <stdint.h>

class Ressource {

	protected:
		std::string	uri;
		std::string	content;
		std::string	contentType;
		std::string	statusCode;
		uint32_t	hash;

	public:
		Ressource(const std::string &uri, const std::string &statusCode = "200 OK");

		void		setUri(const std::string &uri = "");
		void		setContent(const std::string &content = "");
		void		setContentType(const std::string &contentType = "text/plain");
		void		setStatusCode(const std::string &statusCode = "200 OK");

		uint32_t	createHash(void);

		std::string		getUri(void) const;
		std::string		getContent(void) const;
		std::string		getContentType(void) const;
		std::string		getStatusCode(void) const;
		uint32_t		getHash(void) const;

};

#endif