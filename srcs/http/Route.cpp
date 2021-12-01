/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 17:34:54 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/01 22:55:17 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http/Route.hpp"

namespace Webserv {

	namespace Http {

		Route::Route(void) {}

		Route::~Route() {}

		void	Route::fromBlocks(const block_vector &blocks) {
			block_vector::const_iterator	blockIt = blocks.begin();
			while (blockIt != blocks.end()) {
				if (!blockIt->isComment()) {
					block_vector::value_type::values_type values = blockIt->getValues();
					if (values.size() > 0) {
						block_type::values_type::value_type::token_value directive = values.at(0).getValue();
						if (directive == "location") {
							route_type newRoute;
							newRoute.fromBlocks(blockIt->getChilds());
							this->routes["/"] = newRoute;
						} else if (directive == "types") {
							this->mimesTypes.clear();
							this->mimesTypes.fromBlocks(blockIt->getChilds());
						}
					}
				}
				blockIt++;
			}
		}

		void	Route::setMimesTypes(const mimes_types_type &mimesTypes) {
			this->mimesTypes = mimesTypes;
		}

		void 	Route::setErrorPage(const error_pages_type::key_type &errorCode, const error_pages_type::mapped_type &page) {
			this->error_pages[errorCode] = page;
		}

		void 	Route::setClientMaxBodySize(const client_max_body_size_type &client_max_body_size) {
			this->client_max_body_size = client_max_body_size;
		}

		void 	Route::setUploadStore(const upload_store_type &upload_store) {
			this->upload_store = upload_store;
		}

		void 	Route::setReturn(const return_type &_return) {
			this->_return = _return;
		}


		void 	Route::setAutoindex(const autoindex_type &autoindex) {
			this->autoindex = autoindex;
		}

		void 	Route::setRoot(const root_type &root) {
			this->root = root;
		}

		void 	Route::setIndex(const index_type &index) {
			this->index = index;
		}

		void 	Route::setLimitExcept(const limit_except_type &limit_except) {
			this->limit_except = limit_except;
		}

		void 	Route::addRoute(const routes_map::key_type &path, const routes_map::mapped_type &route) {
			this->routes[path] = route;
		}

	} // namespace Http

} // namespace Webserv
