/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ressource.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 15:13:46 by ppaglier          #+#    #+#             */
/*   Updated: 2021/10/26 15:45:33 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Ressource.hpp"

Ressource::Ressource(const std::string &url, const std::string &statusCode) {
	this->url = url;
	this->content = "";
	this->contentType = "text/plain";
	this->statusCode = statusCode;
	this->hash = 1;
}

void		Ressource::setUrl(const std::string &url) {
	this->url = url;
}

void		Ressource::setContent(const std::string &content) {
	this->content = content;
	this->hash = this->createHash();
}

void		Ressource::setContentType(const std::string &contentType) {
	this->contentType = contentType;
}

void		Ressource::setStatusCode(const std::string &statusCode) {
	this->statusCode = statusCode;
}

uint32_t	Ressource::createHash(void) {
	return 1;
}

std::string	Ressource::getUrl(void) const {
	return this->url;
}

std::string	Ressource::getContent(void) const {
	return this->content;
}

std::string	Ressource::getContentType(void) const {
	return this->contentType;
}

std::string	Ressource::getStatusCode(void) const {
	return this->statusCode;
}

uint32_t	Ressource::getHash(void) const {
	return this->hash;
}

