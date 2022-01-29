/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 17:34:54 by ppaglier          #+#    #+#             */
/*   Updated: 2022/01/29 02:16:06 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http/Route.hpp"

namespace Webserv {

	namespace Http {

		Route::Route(void) {
			this->init();
		}

		Route::Route(const Route& other) {
			*this = other;
		}

		Route	&Route::operator = (const Route &old)
		{
			this->mimesTypes = old.mimesTypes;
			this->error_pages = old.error_pages;
			this->client_max_body_size = old.client_max_body_size;
			this->upload_store = old.upload_store;
			this->_return = old._return;
			this->autoindex = old.autoindex;
			this->root = old.root;
			this->index = old.index;
			this->limit_except = old.limit_except;
			this->routes = old.routes;
			return *this;
		}

		Route::~Route(void) {}


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

		void	Route::fromParent(const Route& parent) {
			this->mimesTypes = parent.mimesTypes;
			this->error_pages = parent.error_pages;
			this->client_max_body_size = parent.client_max_body_size;
			this->upload_store = parent.upload_store;
			this->_return = parent._return;
			this->autoindex = parent.autoindex;
			this->root = parent.root; // maybe don't do that
			this->index = parent.index;
			this->limit_except = parent.limit_except;
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
							newRoute.fromParent(*this);
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
								throw directive_type::InvalidValueDirectiveException(directive);
								return false;
							}
							this->error_pages.insert(errorPage);
						} else if (directive == "limit_except") {
							if (!directive_type::parseLimitExcept(values, this->limit_except)) {
								throw directive_type::InvalidValueDirectiveException(directive);
								return false;
							}
						} else if (directive == "client_max_body_size") {
							if (!directive_type::parseClientMaxBodySize(values, this->client_max_body_size, DEFAULT_CLIENT_MAX_BODY_SIZE)) {
								throw directive_type::InvalidValueDirectiveException(directive);
								return false;
							}
						} else if (directive == "return") {
							if (!directive_type::parseReturn(values, this->_return, DEFAULT_RETURN)) {
								throw directive_type::InvalidValueDirectiveException(directive);
								return false;
							}
						} else if (directive == "autoindex") {
							if (!directive_type::parseAutoIndex(values, this->autoindex, DEFAULT_AUTOINDEX)) {
								throw directive_type::InvalidValueDirectiveException(directive);
								return false;
							}
						} else if (directive == "root") {
							if (!directive_type::parseRoot(values, this->root, DEFAULT_ROOT)) {
								throw directive_type::InvalidValueDirectiveException(directive);
								return false;
							}
						} else if (directive == "index") {
							if (!directive_type::parseIndex(values, this->index)) {
								throw directive_type::InvalidValueDirectiveException(directive);
								return false;
							}
						} else if (directive == "upload_store") {
							if (!directive_type::parseUploadStore(values, this->upload_store, DEFAULT_UPLOAD_STORE)) {
								throw directive_type::InvalidValueDirectiveException(directive);
								return false;
							}
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

		void	Route::setMimesTypes(const mimes_types_type& mimesTypes) {
			this->mimesTypes = mimesTypes;
		}

		void	Route::setErrorPages(const error_pages_type& errorPages) {
			this->error_pages = errorPages;
		}

		void	Route::setClientMaxBodySize(const client_max_body_size_type& clientMaxBodySize) {
			this->client_max_body_size = clientMaxBodySize;
		}
		void	Route::setUploadStore(const upload_store_type& uploadStore) {
			this->upload_store = uploadStore;
		}
		void	Route::setReturn(const return_type& _return) {
			this->_return = _return;
		}
		void	Route::setAutoindex(const autoindex_type& autoindex) {
			this->autoindex = autoindex;
		}
		void	Route::setRoot(const root_type& root) {
			this->root = root;
		}
		void	Route::setIndex(const index_type& index) {
			this->index = index;
		}
		void	Route::setLimitExcept(const limit_except_type& limitExcept) {
			this->limit_except = limitExcept;
		}

		void 	Route::addRoute(const routes_map::key_type& path, const routes_map::mapped_type& route) {
			this->routes[path] = route;
		}

		const Route::routes_map	&Route::getRoutes(void) const {
			return this->routes;
		}
		const Route::root_type	&Route::getRoot(void) const {
			return this->root;
		}

	} // namespace Http

} // namespace Webserv