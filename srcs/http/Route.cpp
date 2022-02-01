/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 17:34:54 by ppaglier          #+#    #+#             */
/*   Updated: 2022/02/01 18:21:38 by ppaglier         ###   ########.fr       */
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

		Route::~Route(void) {}

		Route&	Route::operator=(const Route &other)
		{
			if (this != &other) {
				this->currentPath = other.currentPath;
				this->parent = other.parent;
				this->mimesTypes = other.mimesTypes;
				this->error_pages = other.error_pages;
				this->client_max_body_size = other.client_max_body_size;
				this->upload_store = other.upload_store;
				this->_return = other._return;
				this->autoindex = other.autoindex;
				this->root = other.root;
				this->index = other.index;
				this->limit_except = other.limit_except;
				this->routes = other.routes;
			}
			return *this;
		}

		void	Route::init(void) {
			this->currentPath.clear();

			this->parent = NULL;

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

		void	Route::setParent(Route* parent) {
			this->parent = parent;
			if (this->parent) {
				this->mimesTypes = this->parent->mimesTypes;
				this->error_pages = this->parent->error_pages;
				this->client_max_body_size = this->parent->client_max_body_size;
				this->upload_store = this->parent->upload_store;
				this->_return = this->parent->_return;
				this->autoindex = this->parent->autoindex;
				this->root = this->parent->root;
				this->index = this->parent->index;
				this->limit_except = this->parent->limit_except;
			}
		}

		bool	Route::fromBlocks(const block_vector& blocks) {
			block_vector::const_iterator	blockIt = blocks.begin();
			while (blockIt != blocks.end()) {
				if (!blockIt->isComment()) {
					block_vector::value_type::values_type values = blockIt->getValues();
					if (values.size() > 0) {
						block_type::values_type::value_type::token_value directive = values.at(0).getValue();
						if (directive == "location") {
							Route	newRoute;
							newRoute.setParent(this);
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

		void	Route::setCurrentPath(const path_type& currentPath) {
			this->currentPath = currentPath;
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


		const  Route::path_type&					Route::getCurrentPath(void) const {
			return this->currentPath;
		}

		const Route*								Route::getParent(void) const {
			return this->parent;
		}

		const  Route::mimes_types_type&				Route::getMimesTypes(void) const {
			return this->mimesTypes;
		}

		const  Route::error_pages_type&				Route::getErrorPages(void) const {
			return this->error_pages;
		}

		const  Route::client_max_body_size_type&	Route::getClientMaxBodySize(void) const {
			return this->client_max_body_size;
		}

		const  Route::upload_store_type&			Route::getUploadStore(void) const {
			return this->upload_store;
		}

		const  Route::return_type&					Route::getReturn(void) const {
			return this->_return;
		}

		const  Route::autoindex_type&				Route::getAutoindex(void) const {
			return this->autoindex;
		}

		const  Route::root_type&					Route::getRoot(void) const {
			return this->root;
		}

		const  Route::index_type&					Route::getIndex(void) const {
			return this->index;
		}

		const  Route::limit_except_type&			Route::getLimitExcept(void) const {
			return this->limit_except;
		}

		const std::string							Route::getFilePath(const std::string& url) {
			return Webserv::Utils::getConcatURL(this->root, url);
		}


	} // namespace Http

} // namespace Webserv