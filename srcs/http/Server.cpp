/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 17:34:54 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/07 18:19:34 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http/Server.hpp"

namespace Webserv {

	namespace Http {

		Server::Server(void) {}

		Server::~Server() {}

		bool	Server::fromBlocks(const block_vector &blocks) {
			this->setServerName();
			this->setListen();
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
							if (!newRoute.fromBlocks(blockIt->getChilds())) {
								return false;
							}
							this->routes["/"] = newRoute;
						} else if (directive == "types") {
							this->mimesTypes.clear();
							if (!this->mimesTypes.fromBlocks(blockIt->getChilds())) {
								return false;
							}
						} else if (directive == "server_name") {
							if (values.size() < 2) {
								std::cerr << "Unknown context: \"" << directive << "\"" << std::endl;
								return false;
							}
							this->setServerName(values.at(1).getValue());
						} else if (directive == "listen" && values.size() > 2) {
							this->setServerName(values.at(1).getValue());
						}
					}
				}
				blockIt++;
			}
			return true;
		}


		void	Server::setMimesTypes(const mimes_types_type &mimesTypes) {
			this->mimesTypes = mimesTypes;
		}

		void 	Server::setServerName(const name_type &serverName) {
			this->serverName = serverName;
		}

		void 	Server::setListen(const listen_type &listen) {
			this->listen = listen;
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
