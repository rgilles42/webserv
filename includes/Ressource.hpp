/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ressource.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 15:15:41 by ppaglier          #+#    #+#             */
/*   Updated: 2021/10/26 16:00:03 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESSOURCE_HPP
# define RESSOURCE_HPP

#include <string>
#include <stdint.h>

class Ressource {

	protected:
		std::string	url;
		std::string	content;
		std::string	contentType;
		std::string	statusCode;
		uint32_t	hash;

	public:
		Ressource(const std::string &url, const std::string &statusCode = "200 OK");

		void		setUrl(const std::string &url = "");
		void		setContent(const std::string &content = "");
		void		setContentType(const std::string &contentType = "text/plain");
		void		setStatusCode(const std::string &statusCode = "200 OK");

		uint32_t	createHash(void);

		std::string		getUrl(void) const;
		std::string		getContent(void) const;
		std::string		getContentType(void) const;
		std::string		getStatusCode(void) const;
		uint32_t		getHash(void) const;

};

#endif