/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ressource.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 15:13:46 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/10 14:56:16 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Ressource.hpp"

Ressource::Ressource(const std::string& uri, const std::string& statusCode) {
	this->uri = uri;
	this->content = "";
	this->contentType = "text/plain";
	this->statusCode = statusCode;
	this->hash = 1;
}

void		Ressource::setUri(const std::string& uri) {
	this->uri = uri;
}

void		Ressource::setContent(const std::string& content) {
	this->content = content;
	this->hash = this->createHash();
}

void		Ressource::setContentType(const std::string& contentType) {
	this->contentType = contentType;
}

void		Ressource::setStatusCode(const std::string& statusCode) {
	this->statusCode = statusCode;
}

Ressource::hash_type	Ressource::createHash(void) {
	return 1;
}

const Ressource::uri_type&			Ressource::getUri(void) const {
	return this->uri;
}

const Ressource::content_type&		Ressource::getContent(void) const {
	return this->content;
}

const Ressource::content_t_type&	Ressource::getContentType(void) const {
	return this->contentType;
}

const Ressource::status_code_type&	Ressource::getStatusCode(void) const {
	return this->statusCode;
}

Ressource::hash_type				Ressource::getHash(void) const {
	return this->hash;
}

