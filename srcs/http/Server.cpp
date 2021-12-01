/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 17:34:54 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/01 22:55:09 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http/Server.hpp"

namespace Webserv {

	namespace Http {

		Server::Server(void) {}

		Server::~Server() {}

		void	Server::fromBlocks(const block_vector &blocks) {
			this->setServerName();
			this->setHost();
			this->setPort();
			this->setClientMaxBodySize();
			block_vector::const_iterator	blockIt = blocks.begin();
			while (blockIt != blocks.end()) {
				if (!blockIt->isComment()) {
					block_vector::value_type::values_type values = blockIt->getValues();
					if (values.size() > 0) {
						block_type::values_type::value_type::token_value directive = values.at(0).getValue();
						if (directive == "location") {
							route_type newRoute;
							newRoute.setMimesTypes(this->mimesTypes);
							newRoute.fromBlocks(blockIt->getChilds());
							this->routes["/"] = newRoute;
						} else if (directive == "types") {
							this->mimesTypes.clear();
							this->mimesTypes.fromBlocks(blockIt->getChilds());
						} else if (directive == "server_name" && values.size() > 2) {
							this->setServerName(values.at(1).getValue());
						} else if (directive == "listen" && values.size() > 2) {
							this->setServerName(values.at(1).getValue());
						}
					}
				}
				blockIt++;
			}
		}


		void	Server::setMimesTypes(const mimes_types_type &mimesTypes) {
			this->mimesTypes = mimesTypes;
		}

		void 	Server::setServerName(const name_type &serverName) {
			this->serverName = serverName;
		}

		void 	Server::setHost(const host_type &host) {
			this->host = host;
		}

		void 	Server::setPort(const port_type &port) {
			this->port = port;
		}


		void 	Server::setErrorPage(const error_pages_type::key_type &errorCode, const error_pages_type::mapped_type &page) {
			this->error_pages[errorCode] = page;
		}

		void 	Server::setClientMaxBodySize(const client_max_body_size_type &client_max_body_size) {
			this->client_max_body_size = client_max_body_size;
		}

		void 	Server::setUploadStore(const upload_store_type &upload_store) {
			this->upload_store = upload_store;
		}

		void 	Server::setReturn(const return_type &_return) {
			this->_return = _return;
		}


		void 	Server::setAutoindex(const autoindex_type &autoindex) {
			this->autoindex = autoindex;
		}

		void 	Server::setRoot(const root_type &root) {
			this->root = root;
		}

		void 	Server::setIndex(const index_type &index) {
			this->index = index;
		}


		void 	Server::addRoute(const routes_map::key_type &path, const routes_map::mapped_type &route) {
			this->routes[path] = route;
		}

	} // namespace Http

} // namespace Webserv
