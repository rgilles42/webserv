/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ressource.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 15:15:41 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/10 14:55:35 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESSOURCE_HPP
# define RESSOURCE_HPP

#include <string>		// For string
#include <stdint.h>		// For uint32_t

class Ressource {
	public:
		typedef std::string	uri_type;
		typedef std::string	content_type;
		typedef std::string	content_t_type;
		typedef std::string	status_code_type;
		typedef uint32_t	hash_type;

	protected:
		uri_type	uri;
		content_type	content;
		content_t_type	contentType;
		status_code_type	statusCode;
		hash_type	hash;

	public:
		Ressource(const uri_type& uri, const status_code_type& statusCode = "200 OK");

		void				setUri(const uri_type& uri = "");
		void				setContent(const content_type& content = "");
		void				setContentType(const content_t_type& contentType = "text/plain");
		void				setStatusCode(const status_code_type& statusCode = "200 OK");

		hash_type			createHash(void);

		const uri_type&		getUri(void) const;
		const content_type&	getContent(void) const;
		const content_t_type&	getContentType(void) const;
		const status_code_type&	getStatusCode(void) const;
		hash_type			getHash(void) const;

};

#endif