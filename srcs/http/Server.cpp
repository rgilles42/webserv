/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 17:34:54 by ppaglier          #+#    #+#             */
/*   Updated: 2022/02/10 22:26:15 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http/Server.hpp"

namespace Webserv {

	namespace Http {

		Server::Server(void) {
			this->init();
		}

		Server::Server(const Server& other) {
			*this = other;
		}

		Server::~Server() {}

		Server&	Server::operator=(const Server &other)
		{
			if (this != &other) {
				this->serverName = other.serverName;
				this->listen = other.listen;
				this->mimeTypes = other.mimeTypes;
				this->error_pages = other.error_pages;
				this->client_max_body_size = other.client_max_body_size;
				this->upload_store = other.upload_store;
				this->_return = other._return;
				this->autoindex = other.autoindex;
				this->root = other.root;
				this->index = other.index;
				this->cgi_pass = other.cgi_pass;
				this->cgi_ext = other.cgi_ext;
				this->defaultRoute = other.defaultRoute;
				this->routes = other.routes;
			}
			return *this;
		}

		void	Server::init(void) {
			this->mimeTypes.clear();

			this->serverName = DEFAULT_SERVER_NAME;
			this->listen = DEFAULT_LISTEN;

			this->error_pages = directive_type::getDefaultErrorPages();
			this->client_max_body_size = DEFAULT_CLIENT_MAX_BODY_SIZE;
			this->upload_store = DEFAULT_UPLOAD_STORE;

			this->_return = DEFAULT_RETURN;
			this->autoindex = DEFAULT_AUTOINDEX;
			this->root = DEFAULT_ROOT;
			this->index.clear();
			this->cgi_pass.clear();
			this->cgi_ext.clear();

			this->defaultRoute.init();
			this->routes.clear();
		}

		bool	Server::fromBlocks(const block_vector& blocks) {
			block_vector::const_iterator	blockIt = blocks.begin();
			while (blockIt != blocks.end()) {
				if (!blockIt->isComment()) {
					block_vector::value_type::values_type values = blockIt->getValues();
					if (values.size() > 0) {
						block_type::values_type::value_type::token_value directive = values.at(0).getValue();
						if (directive == "location") {
							route_type newRoute;
							newRoute.setParent(&this->defaultRoute);
							if (!newRoute.fromBlocks(blockIt->getChilds())) {
								return false;
							}
							routes_map::key_type key = "/";
							if (values.size() >= 2) {
								key = values[1].getValue();
							}
							newRoute.setCurrentPath(key);
							this->routes[key] = newRoute;
						} else if (directive == "types") {
							this->mimeTypes.clear();
							if (!this->mimeTypes.fromBlocks(blockIt->getChilds())) {
								return false;
							}
							this->defaultRoute.setMimeTypes(this->mimeTypes);
						} else if (directive == "server_name") {
							if (!directive_type::parseServerName(values, this->serverName, DEFAULT_SERVER_NAME)) {
								throw directive_type::InvalidValueDirectiveException(directive);
								return false;
							}
						} else if (directive == "listen") {
							if (!directive_type::parseListen(values, this->listen, DEFAULT_LISTEN)) {
								throw directive_type::InvalidValueDirectiveException(directive);
								return false;
							}
						} else if (directive == "error_page") {
							error_pages_pair errorPage;
							if (!directive_type::parseErrorPage(values, errorPage)) {
								throw directive_type::InvalidValueDirectiveException(directive);
								return false;
							}
							this->error_pages[errorPage.first] = errorPage.second;
							this->defaultRoute.setErrorPages(this->error_pages);
						} else if (directive == "client_max_body_size") {
							if (!directive_type::parseClientMaxBodySize(values, this->client_max_body_size, DEFAULT_CLIENT_MAX_BODY_SIZE)) {
								throw directive_type::InvalidValueDirectiveException(directive);
								return false;
							}
							this->defaultRoute.setClientMaxBodySize(this->client_max_body_size);
						} else if (directive == "return") {
							if (!directive_type::parseReturn(values, this->_return, DEFAULT_RETURN)) {
								throw directive_type::InvalidValueDirectiveException(directive);
								return false;
							}
							this->defaultRoute.setReturn(this->_return);
						} else if (directive == "autoindex") {
							if (!directive_type::parseAutoIndex(values, this->autoindex, DEFAULT_AUTOINDEX)) {
								throw directive_type::InvalidValueDirectiveException(directive);
								return false;
							}
							this->defaultRoute.setAutoindex(this->autoindex);
						} else if (directive == "root") {
							if (!directive_type::parseRoot(values, this->root, DEFAULT_ROOT)) {
								throw directive_type::InvalidValueDirectiveException(directive);
								return false;
							}
							this->defaultRoute.setRoot(this->root);
						} else if (directive == "index") {
							if (!directive_type::parseIndex(values, this->index)) {
								throw directive_type::InvalidValueDirectiveException(directive);
								return false;
							}
							this->defaultRoute.setIndex(this->index);
						} else if (directive == "upload_store") {
							if (!directive_type::parseUploadStore(values, this->upload_store, DEFAULT_UPLOAD_STORE)) {
								throw directive_type::InvalidValueDirectiveException(directive);
								return false;
							}
							this->defaultRoute.setUploadStore(this->upload_store);
						} else if (directive == "cgi_pass") {
							if (!directive_type::parseCgiPass(values, this->cgi_pass)) {
								throw directive_type::InvalidValueDirectiveException(directive);
								return false;
							}
							this->defaultRoute.setCgiPass(this->cgi_pass);
						} else if (directive == "cgi_ext") {
							if (!directive_type::parseCgiExt(values, this->cgi_ext)) {
								throw directive_type::InvalidValueDirectiveException(directive);
								return false;
							}
							this->defaultRoute.setCgiExt(this->cgi_ext);
						} else {
							throw directive_type::UnknownDirectiveException(directive);
							return false;
						}
					}
				}
				blockIt++;
			}
			return true;
		}


		void	Server::setMimeTypes(const mime_types_type& mimeTypes) {
			this->mimeTypes = mimeTypes;
			this->defaultRoute.setMimeTypes(this->mimeTypes);
		}

		void 	Server::addRoute(const routes_map::key_type& path, const routes_map::mapped_type& route) {
			this->routes[path] = route;
		}

		const Server::name_type	&Server::getServerName(void) const {
			return this->serverName;
		}

		const Server::listen_type	&Server::getListen(void) const {
			return this->listen;
		}

		const Server::route_type	&Server::getDefaultRoute(void) const {
			return this->defaultRoute;
		}

		const Server::routes_map	&Server::getRoutes(void) const {
			return this->routes;
		}
		const Server::upload_store_type	&Server::getUploadStore(void) const {
			return this->upload_store;
		}

	} // namespace Http

} // namespace Webserv