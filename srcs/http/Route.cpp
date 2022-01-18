/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 17:34:54 by ppaglier          #+#    #+#             */
/*   Updated: 2022/01/18 14:45:29 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http/Route.hpp"

namespace Webserv {

	namespace Http {

		Route::Route(void) {
			this->init();
		}

		Route::~Route() {}

		void	Route::init(void) {
			this->mimesTypes.clear();

			this->error_pages.clear();
			this->client_max_body_size = DEFAULT_CLIENT_MAX_BODY_SIZE;
			this->upload_store = DEFAULT_UPLOAD_STORE;

			this->_return = DEFAULT_RETURN;
			this->autoindex = DEFAULT_AUTOINDEX;
			this->root = DEFAULT_ROOT;
			this->index.clear();

			this->limit_except.clear();

			this->routes.clear();
		}

		bool	Route::fromBlocks(const block_vector& blocks) {
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
						} else if (directive == "error_page") {
							error_pages_pair errorPage;
							if (!directive_type::parseErrorPage(values, errorPage)) {
								std::cerr << directive_type::InvalidValueDirectiveException(directive).what() << std::endl;
								return false;
							}
							this->error_pages.insert(errorPage);
						} else if (directive == "limit_except") {
							if (!directive_type::parseLimitExcept(values, this->limit_except)) {
								std::cerr << directive_type::InvalidValueDirectiveException(directive).what() << std::endl;
								return false;
							}
						} else if (directive == "client_max_body_size") {
							if (!directive_type::parseClientMaxBodySize(values, this->client_max_body_size, DEFAULT_CLIENT_MAX_BODY_SIZE)) {
								std::cerr << directive_type::InvalidValueDirectiveException(directive).what() << std::endl;
								return false;
							}
						} else if (directive == "return") {
							if (!directive_type::parseReturn(values, this->_return, DEFAULT_RETURN)) {
								std::cerr << directive_type::InvalidValueDirectiveException(directive).what() << std::endl;
								return false;
							}
						} else if (directive == "autoindex") {
							if (!directive_type::parseAutoIndex(values, this->autoindex, DEFAULT_AUTOINDEX)) {
								std::cerr << directive_type::InvalidValueDirectiveException(directive).what() << std::endl;
								return false;
							}
						} else if (directive == "root") {
							if (!directive_type::parseRoot(values, this->root, DEFAULT_ROOT)) {
								std::cerr << directive_type::InvalidValueDirectiveException(directive).what() << std::endl;
								return false;
							}
						} else if (directive == "index") {
							if (!directive_type::parseIndex(values, this->index)) {
								std::cerr << directive_type::InvalidValueDirectiveException(directive).what() << std::endl;
								return false;
							}
						} else if (directive == "upload_store") {
							if (!directive_type::parseUploadStore(values, this->upload_store, DEFAULT_UPLOAD_STORE)) {
								std::cerr << directive_type::InvalidValueDirectiveException(directive).what() << std::endl;
								return false;
							}
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

		void	Route::setMimesTypes(const mimes_types_type& mimesTypes) {
			this->mimesTypes = mimesTypes;
		}

		void 	Route::addRoute(const routes_map::key_type& path, const routes_map::mapped_type& route) {
			this->routes[path] = route;
		}

		const Route::routes_map	&Route::getRoutes(void) const {
			return this->routes;
		}

	} // namespace Http

} // namespace Webserv
