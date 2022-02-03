/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directive.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 15:13:45 by ppaglier          #+#    #+#             */
/*   Updated: 2022/02/03 17:58:29 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTIVE_HPP
# define DIRECTIVE_HPP

# include <string>						// For string
# include <vector>						// For vector
# include <map>							// For map
# include <algorithm>					// For
# include <exception>					// For exceptions
# include <sstream>						// For stringstream

# include "Token.hpp"					// For Token
# include "Address.hpp"					// For Address
# include "Byte.hpp"					// For Byte
# include "../http/HttpStatusCode.hpp"	// For HttpStatusCode

# define DEFAULT_SERVER_NAME Webserv::Utils::Directive::dir_server_name_type("")
# define DEFAULT_LISTEN Webserv::Utils::Directive::dir_listen_type("0.0.0.0:80")
# define DEFAULT_CLIENT_MAX_BODY_SIZE Webserv::Utils::Directive::dir_client_max_body_size_type(1, Webserv::Utils::Directive::dir_client_max_body_size_type::U_MB)
# define DEFAULT_RETURN Webserv::Utils::Directive::dir_return_type(Webserv::Utils::Directive::http_status_code_type::unknown, "")
# define DEFAULT_AUTOINDEX Webserv::Utils::Directive::dir_autoindex_type(false)
# define DEFAULT_ROOT Webserv::Utils::Directive::dir_root_type("html")
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
				typedef std::pair<http_status_code_type::StatusCode, std::string>					dir_error_page_type;
				typedef std::map<dir_error_page_type::first_type, dir_error_page_type::second_type>	error_pages_type;
				typedef Webserv::Utils::Byte									dir_client_max_body_size_type;
				typedef std::vector<std::string>								dir_limit_except_type;
				typedef std::pair<http_status_code_type::StatusCode, std::string>	dir_return_type;
				typedef bool													dir_autoindex_type;
				typedef std::string												dir_root_type;
				typedef std::vector<std::string>								dir_index_type;
				typedef std::string												dir_upload_store_type;
				typedef std::string												dir_cgi_pass_type;
				typedef std::vector<std::string>								dir_cgi_ext_type;


				class DirectiveException : public std::exception {
					protected:
						std::string	msg;
						name_type	directive;

					public:
						DirectiveException(const name_type& directive = name_type(), const std::string& msg = "") : std::exception() {
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
						UnknownDirectiveException(const name_type& directive = name_type()) : DirectiveException(directive) {
							std::ostringstream ss;

							ss << "Unknown directive \"" << this->directive << "\"";

							this->msg = ss.str();
						}
				};

				class InvalidValueDirectiveException : public DirectiveException {
					public:
						InvalidValueDirectiveException(const name_type& directive = name_type()) : DirectiveException(directive) {
							std::ostringstream ss;

							ss << "\"" << this->directive << "\" directive invalid value";

							this->msg = ss.str();
						}
				};


			protected:
				name_type		name;
				argc_type		argc;
				context_vector	contexts;

			public:
				Directive(const name_type& name = name_type(), const argc_type& argc = argc_type(-1, -1), const context_vector& contexts = context_vector());
				Directive(const Directive& other);
				virtual ~Directive();

				Directive&				operator=(const Directive& other);

				const name_type&		getName(void) const;
				const context_vector&	getContexts(void) const;
				const argc_type&		getArgc(void) const;

				bool					isContextValid(const context_type& context) const;

				bool					isSrcValueIsValid(const src_value_type& src) const;

				static const error_pages_type	getDefaultErrorPages(void);

				// Parsing with static methods

				static bool				parseListen(const src_value_type& src, dir_listen_type& value, const dir_listen_type& defaultValue = dir_listen_type());

				static bool				parseServerName(const src_value_type& src, dir_server_name_type& value, const dir_server_name_type& defaultValue = dir_server_name_type());

				static bool				parseErrorPage(const src_value_type& src, dir_error_page_type& value, const dir_error_page_type& defaultValue = dir_error_page_type());

				static bool				parseClientMaxBodySize(const src_value_type& src, dir_client_max_body_size_type& value, const dir_client_max_body_size_type& defaultValue = dir_client_max_body_size_type());

				static bool				parseLimitExcept(const src_value_type& src, dir_limit_except_type& value, const dir_limit_except_type& defaultValue = dir_limit_except_type());

				static bool				parseReturn(const src_value_type& src, dir_return_type& value, const dir_return_type& defaultValue = dir_return_type());

				static bool				parseAutoIndex(const src_value_type& src, dir_autoindex_type& value, const dir_autoindex_type& defaultValue = dir_autoindex_type());

				static bool				parseRoot(const src_value_type& src, dir_root_type& value, const dir_root_type& defaultValue = dir_root_type());

				static bool				parseIndex(const src_value_type& src, dir_index_type& value, const dir_index_type& defaultValue = dir_index_type());

				static bool				parseUploadStore(const src_value_type& src, dir_upload_store_type& value, const dir_upload_store_type& defaultValue = dir_upload_store_type());

				static bool				parseCgiPass(const src_value_type& src, dir_cgi_pass_type& value, const dir_cgi_pass_type& defaultValue = dir_cgi_pass_type());

				static bool				parseCgiExt(const src_value_type& src, dir_cgi_ext_type& value, const dir_cgi_ext_type& defaultValue = dir_cgi_ext_type());
		};

	} // namespace Utils

} // namespace Webserv

#endif