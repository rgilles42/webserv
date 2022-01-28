/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 17:34:54 by ppaglier          #+#    #+#             */
/*   Updated: 2022/01/28 17:34:46 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http/Server.hpp"

namespace Webserv {

	namespace Http {

		Server::Server(void) {
			this->init();
		}

		Server	&Server::operator = (const Server &old)
		{
			this->serverName = old.serverName;
			this->listen = old.listen;
			this->mimesTypes = old.mimesTypes;
			this->error_pages = old.error_pages;
			this->client_max_body_size = old.client_max_body_size;
			this->upload_store = old.upload_store;
			this->_return = old._return;
			this->autoindex = old.autoindex;
			this->root = old.root;
			this->index = old.index;
			this->defaultRoute = old.defaultRoute;
			this->routes = old.routes;
			return *this;
		}

		Server::~Server() {}

		void	Server::init(void) {
			this->mimesTypes.clear();

			this->serverName = DEFAULT_SERVER_NAME;
			this->listen = DEFAULT_LISTEN;

			this->error_pages.clear();
			this->client_max_body_size = DEFAULT_CLIENT_MAX_BODY_SIZE;
			this->upload_store = DEFAULT_UPLOAD_STORE;

			this->_return = DEFAULT_RETURN;
			this->autoindex = DEFAULT_AUTOINDEX;
			this->root = DEFAULT_ROOT;
			this->index.clear();

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
							newRoute.fromParent(this->defaultRoute);
							if (!newRoute.fromBlocks(blockIt->getChilds())) {
								return false;
							}
							routes_map::key_type key = "/";
							if (values.size() >= 2) {
								key = values[1].getValue();
							}
							this->routes[key] = newRoute;
						} else if (directive == "types") {
							this->mimesTypes.clear();
							if (!this->mimesTypes.fromBlocks(blockIt->getChilds())) {
								return false;
							}
						} else if (directive == "server_name") {
							if (!directive_type::parseServerName(values, this->serverName, DEFAULT_SERVER_NAME)) {
								std::cerr << directive_type::InvalidValueDirectiveException(directive).what() << std::endl;
								return false;
							}
						} else if (directive == "listen") {
							if (!directive_type::parseListen(values, this->listen, DEFAULT_LISTEN)) {
								std::cerr << directive_type::InvalidValueDirectiveException(directive).what() << std::endl;
								return false;
							}
						} else if (directive == "error_page") {
							error_pages_pair errorPage;
							if (!directive_type::parseErrorPage(values, errorPage)) {
								std::cerr << directive_type::InvalidValueDirectiveException(directive).what() << std::endl;
								return false;
							}
							this->error_pages.insert(errorPage);
							this->defaultRoute.setErrorPages(this->error_pages);
						} else if (directive == "client_max_body_size") {
							if (!directive_type::parseClientMaxBodySize(values, this->client_max_body_size, DEFAULT_CLIENT_MAX_BODY_SIZE)) {
								std::cerr << directive_type::InvalidValueDirectiveException(directive).what() << std::endl;
								return false;
							}
							this->defaultRoute.setClientMaxBodySize(this->client_max_body_size);
						} else if (directive == "return") {
							if (!directive_type::parseReturn(values, this->_return, DEFAULT_RETURN)) {
								std::cerr << directive_type::InvalidValueDirectiveException(directive).what() << std::endl;
								return false;
							}
							this->defaultRoute.setReturn(this->_return);
						} else if (directive == "autoindex") {
							if (!directive_type::parseAutoIndex(values, this->autoindex, DEFAULT_AUTOINDEX)) {
								std::cerr << directive_type::InvalidValueDirectiveException(directive).what() << std::endl;
								return false;
							}
							this->defaultRoute.setAutoindex(this->autoindex);
						} else if (directive == "root") {
							if (!directive_type::parseRoot(values, this->root, DEFAULT_ROOT)) {
								std::cerr << directive_type::InvalidValueDirectiveException(directive).what() << std::endl;
								return false;
							}
							this->defaultRoute.setRoot(this->root);
						} else if (directive == "index") {
							if (!directive_type::parseIndex(values, this->index)) {
								std::cerr << directive_type::InvalidValueDirectiveException(directive).what() << std::endl;
								return false;
							}
							this->defaultRoute.setIndex(this->index);
						} else if (directive == "upload_store") {
							if (!directive_type::parseUploadStore(values, this->upload_store, DEFAULT_UPLOAD_STORE)) {
								std::cerr << directive_type::InvalidValueDirectiveException(directive).what() << std::endl;
								return false;
							}
							this->defaultRoute.setUploadStore(this->upload_store);
						} else {
							std::cerr << directive_type::UnknownDirectiveException(directive).what() << std::endl;
							return false;
						}
					}
				}
				blockIt++;
			}
			return true;
		}


		void	Server::setMimesTypes(const mimes_types_type& mimesTypes) {
			this->mimesTypes = mimesTypes;
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

	} // namespace Http

} // namespace Webserv