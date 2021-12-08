/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directive.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 15:13:45 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/08 15:15:32 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTIVE_HPP
# define DIRECTIVE_HPP

# include <string>
# include <vector>
# include <map>
# include <algorithm>
# include <exception>
# include <sstream>

# include <iostream>

# include "Token.hpp"
# include "Address.hpp"
# include "Byte.hpp"
# include "../http/HttpStatusCode.hpp"

# define DEFAULT_SERVER_NAME Webserv::Utils::Directive::dir_server_name_type("")
# define DEFAULT_LISTEN Webserv::Utils::Directive::dir_listen_type("0.0.0.0:80")
# define DEFAULT_CLIENT_MAX_BODY_SIZE Webserv::Utils::Directive::dir_client_max_body_size_type(1, Webserv::Utils::Directive::dir_client_max_body_size_type::U_MB)
# define DEFAULT_RETURN Webserv::Utils::Directive::dir_return_type(Webserv::Utils::Directive::http_status_code_type::unknown, "")
# define DEFAULT_AUTOINDEX Webserv::Utils::Directive::dir_autoindex_type(false)
# define DEFAULT_ROOT Webserv::Utils::Directive::dir_root_type("")
# define DEFAULT_UPLOAD_STORE Webserv::Utils::Directive::dir_upload_store_type("")

namespace Webserv {

	namespace Utils {

		class Directive {
			public:
				typedef std::string					name_type;
				typedef std::string					context_type;
				typedef	std::vector<context_type>	context_vector;
				typedef std::pair<size_t, size_t>	argc_type;
				typedef Webserv::Utils::Token		token_type;
				typedef	std::vector<token_type>		src_value_type;

				typedef std::map<name_type, Directive>	directive_map;

				typedef Webserv::Http::HttpStatusCode	http_status_code_type;

				typedef std::string												dir_server_name_type;
				typedef Webserv::Utils::Address									dir_listen_type;
				typedef std::pair<http_status_code_type::StatusCode, std::string>	dir_error_page_type;
				typedef Webserv::Utils::Byte									dir_client_max_body_size_type;
				typedef std::vector<std::string>								dir_limit_except_type;
				typedef std::pair<http_status_code_type::StatusCode, std::string>	dir_return_type;
				typedef bool													dir_autoindex_type;
				typedef std::string												dir_root_type;
				typedef std::vector<std::string>								dir_index_type;
				typedef std::string												dir_upload_store_type;
				typedef std::string												dir_cgi_pass_type;


				class DirectiveException : public std::exception {
					protected:
						std::string	msg;
						name_type	directive;

					public:
						DirectiveException(const name_type &directive = name_type(), const std::string &msg = "") : std::exception() {
							this->msg = msg;
							this->directive = directive;
						}
						virtual ~DirectiveException() throw() {}
						const name_type	getDirective() const {
							return this->directive;

						}
						virtual const char	*what() const throw() {
							return this->msg.c_str();
						}
				};

				class UnknownDirectiveException : public DirectiveException {
					public:
						UnknownDirectiveException(const name_type &directive = name_type()) : DirectiveException(directive) {
							std::ostringstream ss;

							ss << "Unknown directive \"" << this->directive << "\"";

							this->msg = ss.str();
						}
				};

				class InvalidValueDirectiveException : public DirectiveException {
					public:
						InvalidValueDirectiveException(const name_type &directive = name_type()) : DirectiveException(directive) {
							std::ostringstream ss;

							ss << "\"" << this->directive << "\" directive invalid value";

							this->msg = ss.str();
						}
				};


			protected:
				name_type			name;
				argc_type			argc;
				context_vector	contexts;

			public:
				Directive(const name_type &name = name_type(), const argc_type &argc = argc_type(-1, -1), const context_vector &contexts = context_vector()) : name(name), argc(argc), contexts(contexts) {};

				virtual ~Directive() {};

				const name_type			&getName(void) const {
					return this->name;
				}
				const context_vector	&getContexts(void) const {
					return this->contexts;
				}
				const argc_type			&getArgc(void) const {
					return this->argc;
				}

				bool					isContextValid(const context_type &context) const {
					context_vector::const_iterator it = std::find(this->contexts.begin(), this->contexts.end(), context);
					return it != this->contexts.end();
				}

				bool					isSrcValueIsValid(const src_value_type &src) const {
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

				// Parsing with static methods

				static bool				parseListen(const src_value_type &src, dir_listen_type &value, const dir_listen_type &defaultValue = dir_listen_type()) {
					value = defaultValue;
					if (src.size() > 1) {
						if (!value.fromString(src[1].getValue())) {
							return false;
						}
						if (src.size() > 2) {
							if (value.getPort() != 0) {
								return false;
							}
							value.setPort(std::atoi((src[2].getValue()).c_str()));
						}
					}
					return true;
				}

				static bool				parseServerName(const src_value_type &src, dir_server_name_type &value, const dir_server_name_type &defaultValue = dir_server_name_type()) {
					value = defaultValue;
					if (src.size() > 1) {
						value = src[1].getValue();
					}
					return true;
				}

				static bool				parseErrorPage(const src_value_type &src, dir_error_page_type &value, const dir_error_page_type &defaultValue = dir_error_page_type()) {
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

				static bool				parseClientMaxBodySize(const src_value_type &src, dir_client_max_body_size_type &value, const dir_client_max_body_size_type &defaultValue = dir_client_max_body_size_type()) {
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

				static bool				parseLimitExcept(const src_value_type &src, dir_limit_except_type &value, const dir_limit_except_type &defaultValue = dir_limit_except_type()) {
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

				static bool				parseReturn(const src_value_type &src, dir_return_type &value, const dir_return_type &defaultValue = dir_return_type()) {
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

				static bool				parseAutoIndex(const src_value_type &src, dir_autoindex_type &value, const dir_autoindex_type &defaultValue = dir_autoindex_type()) {
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

				static bool				parseRoot(const src_value_type &src, dir_root_type &value, const dir_root_type &defaultValue = dir_root_type()) {
					value = defaultValue;
					if (src.size() != 2) {
						return false;
					}
					value = src[1].getValue();
					return true;
				}

				static bool				parseIndex(const src_value_type &src, dir_index_type &value, const dir_index_type &defaultValue = dir_index_type()) {
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

				static bool				parseUploadStore(const src_value_type &src, dir_upload_store_type &value, const dir_upload_store_type &defaultValue = dir_upload_store_type()) {
					value = defaultValue;
					if (src.size() != 2) {
						return false;
					}
					value = src[1].getValue();
					return true;
				}

				static bool				parseCgiPass(const src_value_type &src, dir_cgi_pass_type &value, const dir_cgi_pass_type &defaultValue = dir_cgi_pass_type()) {
					value = defaultValue;
					if (src.size() != 2) {
						return false;
					}
					value = src[1].getValue();
					return true;
				}

		};

	} // namespace Utils

} // namespace Webserv

#endif