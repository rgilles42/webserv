/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directive.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 14:17:02 by ppaglier          #+#    #+#             */
/*   Updated: 2022/02/01 18:39:20 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils/Directive.hpp"

namespace Webserv {

	namespace Utils {

				Directive::Directive(const name_type& name, const argc_type& argc, const context_vector& contexts) : name(name), argc(argc), contexts(contexts) {};

				Directive::Directive(const Directive& other) {
					*this = other;
				}

				Directive::~Directive() {};

				Directive&							Directive::operator=(const Directive& other) {
					if (this != &other) {
						this->name = other.name;
						this->argc = other.argc;
						this->contexts = other.contexts;
					}
					return *this;
				}

				const Directive::name_type&			Directive::getName(void) const {
					return this->name;
				}
				const Directive::context_vector&	Directive::getContexts(void) const {
					return this->contexts;
				}
				const Directive::argc_type&			Directive::getArgc(void) const {
					return this->argc;
				}

				bool	Directive::isContextValid(const context_type& context) const {
					context_vector::const_iterator it = std::find(this->contexts.begin(), this->contexts.end(), context);
					return it != this->contexts.end();
				}

				bool	Directive::isSrcValueIsValid(const src_value_type& src) const {
					size_t size = src.size();
					if (size > 0) {
						// Check if the directive is the good one
						if (src[0].getValue() != this->name) {
							return false;
						}
						// Don't count the directive keyword
						size -= 1;
					}
					if (this->argc.first != (size_t)-1 && size < this->argc.first) {
						return false;
					}
					if (this->argc.second != (size_t)-1 && size > this->argc.second) {
						return false;
					}
					return true;
				}

				const Directive::error_pages_type	Directive::getDefaultErrorPages(void) {
					error_pages_type	errorPages;

					errorPages[http_status_code_type::client_error_bad_request] = "./default_pages/400.html";
					errorPages[http_status_code_type::client_error_unauthorized] = "./default_pages/401.html";
					errorPages[http_status_code_type::client_error_forbidden] = "./default_pages/403.html";
					errorPages[http_status_code_type::client_error_not_found] = "./default_pages/404.html";
					errorPages[http_status_code_type::server_error_internal_server_error] = "./default_pages/500.html";
					errorPages[http_status_code_type::server_error_not_implemented] = "./default_pages/501.html";
					errorPages[http_status_code_type::server_error_bad_gateway] = "./default_pages/502.html";
					errorPages[http_status_code_type::server_error_service_unavailable] = "./default_pages/503.html";

					return errorPages;
				}

				// Parsing with static methods

				bool	Directive::parseListen(const src_value_type& src, dir_listen_type& value, const dir_listen_type& defaultValue) {
					dir_listen_type tmp;

					value = defaultValue;
					if (src.size() > 1) {
						if (!tmp.fromString(src[1].getValue())) {
							return false;
						}
						if (src.size() > 2) {
							if (tmp.getPort() != 0) {
								return false;
							}
							tmp.setPort(std::atoi((src[2].getValue()).c_str()));
						}
					}
					if (tmp.isAddressValid()) {
						value.setAddress(tmp.getAddress());
					}
					if (tmp.getPort() != 0) {
						value.setPort(tmp.getIntPort());
					}
					return true;
				}

				bool	Directive::parseServerName(const src_value_type& src, dir_server_name_type& value, const dir_server_name_type& defaultValue) {
					value = defaultValue;
					if (src.size() > 1) {
						value = src[1].getValue();
					}
					return true;
				}

				bool	Directive::parseErrorPage(const src_value_type& src, dir_error_page_type& value, const dir_error_page_type& defaultValue) {
					value = defaultValue;
					if (src.size() != 3) {
						return false;
					}
					src_value_type::value_type::token_value tokValue1 = src[1].getValue();
					if (tokValue1.find_first_not_of("0123456789") != tokValue1.npos) {
						return false;
					}
					value.first = http_status_code_type::getStatusCode(std::atoi(tokValue1.c_str()));
					if (value.first <= 0 || !http_status_code_type::isError(value.first)) {
						return false;
					}
					value.second = src[2].getValue();
					return true;
				}

				bool	Directive::parseClientMaxBodySize(const src_value_type& src, dir_client_max_body_size_type& value, const dir_client_max_body_size_type& defaultValue) {
					value = defaultValue;
					if (src.size() != 2) {
						return false;
					}
					src_value_type::value_type::token_value tokValue = src[1].getValue();
					value.setValue(std::atoi((tokValue).c_str()));
					size_t pos = tokValue.find_first_not_of("0123456789");
					if (pos == tokValue.npos) {
						return false;
					}
					tokValue = tokValue.substr(pos, tokValue.length() - pos);
					if (tokValue.empty() || !dir_client_max_body_size_type::isUnitValid(tokValue)) {
						return false;
					}
					value.setUnit(dir_client_max_body_size_type::getUnitByStr(tokValue));
					return true;
				}

				bool	Directive::parseLimitExcept(const src_value_type& src, dir_limit_except_type& value, const dir_limit_except_type& defaultValue) {
					value = defaultValue;
					if (src.size() < 2) {
						return false;
					}
					src_value_type::const_iterator it = src.begin() + 1;
					while (it != src.end()) {
						value.push_back(it->getValue());
						it++;
					}
					return true;
				}

				bool	Directive::parseReturn(const src_value_type& src, dir_return_type& value, const dir_return_type& defaultValue) {
					value = defaultValue;
					if (src.size() < 2) {
						return false;
					}
					if (src.size() == 2) {
						value.first = http_status_code_type::unknown;
						value.second = src[1].getValue();
					} else {
						src_value_type::value_type::token_value tokValue1 = src[1].getValue();
						if (tokValue1.find_first_not_of("0123456789") != tokValue1.npos) {
							return false;
						}
						value.first = http_status_code_type::getStatusCode(std::atoi(tokValue1.c_str()));
						if (value.first <= 0) {
							return false;
						}
						if (src.size() > 2) {
							value.second = src[2].getValue();
						}
					}
					return true;
				}

				bool	Directive::parseAutoIndex(const src_value_type& src, dir_autoindex_type& value, const dir_autoindex_type& defaultValue) {
					value = defaultValue;
					if (src.size() != 2) {
						return false;
					}
					src_value_type::value_type::token_value tokValue = src[1].getValue();
					if (tokValue == "on" || tokValue == "true") {
						value = true;
						return true;
					}
					if (tokValue == "off" || tokValue == "false") {
						value = false;
						return true;
					}
					return false;
				}

				bool	Directive::parseRoot(const src_value_type& src, dir_root_type& value, const dir_root_type& defaultValue) {
					value = defaultValue;
					if (src.size() != 2) {
						return false;
					}
					value = src[1].getValue();
					return true;
				}

				bool	Directive::parseIndex(const src_value_type& src, dir_index_type& value, const dir_index_type& defaultValue) {
					value = defaultValue;
					if (src.size() < 2) {
						return false;
					}
					src_value_type::const_iterator it = src.begin() + 1;
					while (it != src.end()) {
						value.push_back(it->getValue());
						it++;
					}
					return true;
				}

				bool	Directive::parseUploadStore(const src_value_type& src, dir_upload_store_type& value, const dir_upload_store_type& defaultValue) {
					value = defaultValue;
					if (src.size() != 2) {
						return false;
					}
					value = src[1].getValue();
					return true;
				}

				bool	Directive::parseCgiPass(const src_value_type& src, dir_cgi_pass_type& value, const dir_cgi_pass_type& defaultValue) {
					value = defaultValue;
					if (src.size() != 2) {
						return false;
					}
					value = src[1].getValue();
					return true;
				}

	} // namespace Utils

} // namespace Webserv
