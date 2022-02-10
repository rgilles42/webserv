/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgilles <rgilles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 17:34:54 by ppaglier          #+#    #+#             */
/*   Updated: 2022/02/10 16:56:24 by rgilles          ###   ########.fr       */
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
				this->mimeTypes = other.mimeTypes;
				this->error_pages = other.error_pages;
				this->client_max_body_size = other.client_max_body_size;
				this->upload_store = other.upload_store;
				this->_return = other._return;
				this->autoindex = other.autoindex;
				this->root = other.root;
				this->index = other.index;
				this->enable_limit_except = other.enable_limit_except;
				this->limit_except = other.limit_except;
				this->cgi_pass = other.cgi_pass;
				this->cgi_ext = other.cgi_ext;
				this->routes = other.routes;
			}
			return *this;
		}

		void	Route::init(void) {
			this->currentPath.clear();

			this->parent = NULL;

			this->mimeTypes.clear();

			this->error_pages = directive_type::getDefaultErrorPages();

			this->client_max_body_size = DEFAULT_CLIENT_MAX_BODY_SIZE;
			this->upload_store = DEFAULT_UPLOAD_STORE;

			this->_return = DEFAULT_RETURN;
			this->autoindex = DEFAULT_AUTOINDEX;
			this->root = DEFAULT_ROOT;
			this->index.clear();
			this->cgi_pass.clear();
			this->cgi_ext.clear();

			this->enable_limit_except = false;
			this->limit_except.clear();

			this->routes.clear();
		}

		void	Route::setParent(Route* parent) {
			this->parent = parent;
			if (this->parent) {
				this->mimeTypes = this->parent->mimeTypes;
				this->error_pages = this->parent->error_pages;
				this->client_max_body_size = this->parent->client_max_body_size;
				this->upload_store = this->parent->upload_store;
				this->_return = this->parent->_return;
				this->autoindex = this->parent->autoindex;
				this->root = this->parent->root;
				this->index = this->parent->index;
				this->enable_limit_except = this->parent->enable_limit_except;
				this->limit_except = this->parent->limit_except;
				this->cgi_pass = this->parent->cgi_pass;
				this->cgi_ext = this->parent->cgi_ext;
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
							this->mimeTypes.clear();
							if (!this->mimeTypes.fromBlocks(blockIt->getChilds())) {
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
							this->enable_limit_except = true;
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
						} else if (directive == "cgi_pass") {
							if (!directive_type::parseCgiPass(values, this->cgi_pass)) {
								throw directive_type::InvalidValueDirectiveException(directive);
								return false;
							}
						} else if (directive == "cgi_ext") {
							if (!directive_type::parseCgiExt(values, this->cgi_ext)) {
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

		void	Route::setMimeTypes(const mime_types_type& mimeTypes) {
			this->mimeTypes = mimeTypes;
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
		void	Route::setEnableLimitExcept(const bool& val) {
			this->enable_limit_except = val;
		}
		void	Route::setCgiPass(const cgi_pass_type& cgiPass) {
			this->cgi_pass = cgiPass;
		}
		void	Route::setCgiExt(const cgi_ext_type& cgiExt) {
			this->cgi_ext = cgiExt;
		}

		void 	Route::addRoute(const routes_map::key_type& path, const routes_map::mapped_type& route) {
			this->routes[path] = route;
		}

		const Route::routes_map	&Route::getRoutes(void) const {
			return this->routes;
		}


		const Route::path_type&					Route::getCurrentPath(void) const {
			return this->currentPath;
		}

		const Route*							Route::getParent(void) const {
			return this->parent;
		}

		const Route::mime_types_type&			Route::getMimeTypes(void) const {
			return this->mimeTypes;
		}

		const Route::error_pages_type&			Route::getErrorPages(void) const {
			return this->error_pages;
		}

		const Route::client_max_body_size_type&	Route::getClientMaxBodySize(void) const {
			return this->client_max_body_size;
		}

		const Route::upload_store_type&			Route::getUploadStore(void) const {
			return this->upload_store;
		}

		const Route::return_type&				Route::getReturn(void) const {
			return this->_return;
		}

		const Route::autoindex_type&			Route::getAutoindex(void) const {
			return this->autoindex;
		}

		const Route::root_type&					Route::getRoot(void) const {
			return this->root;
		}

		const Route::index_type&				Route::getIndex(void) const {
			return this->index;
		}

		const bool&								Route::getEnableLimitExcept(void) const {
			return this->enable_limit_except;
		}

		const Route::limit_except_type&			Route::getLimitExcept(void) const {
			return this->limit_except;
		}

		const Route::cgi_pass_type&				Route::getCgiPass(void) const {
			return this->cgi_pass;
		}

		const Route::cgi_ext_type&				Route::getCgiExt(void) const {
			return this->cgi_ext;
		}

		const Route::error_pages_pair			Route::getErrorPage(const error_pages_type::key_type& statusCode) {
			error_pages_pair errorPage;
			if (this->error_pages.count(statusCode) > 0) {
				return *(this->error_pages.find(statusCode));
			}
			if (statusCode.isClientError()) {
				if (this->error_pages.count(directive_type::http_status_code_type::client_error_bad_request) > 0) {
					return *(this->error_pages.find(directive_type::http_status_code_type::client_error_bad_request));
				}
			} else if (statusCode.isServerError()) {
				if (this->error_pages.count(directive_type::http_status_code_type::server_error_internal_server_error) > 0) {
					return *(this->error_pages.find(directive_type::http_status_code_type::server_error_internal_server_error));
				}
			}
			return error_pages_pair(directive_type::http_status_code_type::unknown, "");
		}

		const std::string							Route::getFilePath(const std::string& url) {
			struct stat	s;
			std::string	newpath;
			std::string	req_path = Webserv::Utils::getConcatURL(this->root, url);
			if (stat(req_path.c_str(), &s) == 0 && S_ISDIR(s.st_mode))
			{
				for (std::vector<std::string>::const_iterator it = this->getIndex().begin(); it != this->getIndex().end(); it++)
				{
					if (stat((newpath = Webserv::Utils::getConcatURL(req_path, *it)).c_str(), &s) == 0)
					{
						req_path = newpath;
						break ;
					}
				}
			}
			return (Webserv::Utils::url_decode(req_path));
		}


	} // namespace Http

} // namespace Webserv