/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpStatusCode.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 00:38:38 by ppaglier          #+#    #+#             */
/*   Updated: 2022/02/03 19:55:01 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPSTATUSCODE_HPP
# define HTTPSTATUSCODE_HPP

# include <string>	// For string
# include <map>		// For map

namespace Webserv {

	namespace Http {

		class HttpStatusCode {
			public:
				enum StatusCodeOffset {
					unknown_offset = 0,

					information_offset = 100,
					success_offset = 200,
					redirection_offset = 300,
					client_error_offset = 400,
					server_error_offset = 500,
					misc_error_offset = 600,
				};

				enum StatusCode {
					unknown = 0,

					information_continue = 100,
					information_switching_protocols,
					information_processing,
					information_early_hints,

					success_ok = 200,
					success_created,
					success_accepted,
					success_non_authoritative_information,
					success_no_content,
					success_reset_content,
					success_partial_content,
					success_multi_status,
					success_already_reported,
					success_im_used = 226,

					redirection_multiple_choices = 300,
					redirection_moved_permanently,
					redirection_found,
					redirection_see_other,
					redirection_not_modified,
					redirection_use_proxy,
					redirection_switch_proxy, // unused
					redirection_temporary_redirect,
					redirection_permanent_redirect,

					client_error_bad_request = 400,
					client_error_unauthorized,
					client_error_payment_required,
					client_error_forbidden,
					client_error_not_found,
					client_error_method_not_allowed,
					client_error_not_acceptable,
					client_error_proxy_authentication_required,
					client_error_request_timeout,
					client_error_conflict,
					client_error_gone,
					client_error_length_required,
					client_error_precondition_failed,
					client_error_payload_too_large,
					client_error_uri_too_long,
					client_error_unsupported_media_type,
					client_error_range_not_satisfiable,
					client_error_expectation_failed,
					client_error_im_a_teapot,
					client_error_misdirection_required = 421,
					client_error_unprocessable_entity,
					client_error_locked,
					client_error_failed_dependency,
					client_error_too_early,
					client_error_upgrade_required,
					client_error_precondition_required = 428,
					client_error_too_many_requests,
					client_error_request_header_fields_too_large = 431,
					client_error_unavailable_for_legal_reasons = 451,

					server_error_internal_server_error = 500,
					server_error_not_implemented,
					server_error_bad_gateway,
					server_error_service_unavailable,
					server_error_gateway_timeout,
					server_error_http_version_not_supported,
					server_error_variant_also_negotiates,
					server_error_insufficient_storage,
					server_error_loop_detected,
					server_error_not_extended = 510,
					server_error_network_authentication_required
				};
				typedef std::map<HttpStatusCode::StatusCode, std::string> StatusCodeStringMap;

			protected:
				StatusCode	statusCode;

				static const StatusCodeStringMap	HttpStatusCodeStrings;

				static const StatusCodeStringMap	fillMap(void);

			public:
				HttpStatusCode(void);
				HttpStatusCode(const int& statusCode);
				HttpStatusCode(const std::string& statusCodeString);
				HttpStatusCode(const HttpStatusCode& other);
				~HttpStatusCode();

				HttpStatusCode&		operator=(const HttpStatusCode& other);
				HttpStatusCode&		operator=(const int& statusCode);
				HttpStatusCode&		operator=(const std::string& statusCodeString);

				const StatusCode&	getStatusCode(void) const;
				const std::string	toString(void) const;

				void				setStatusCode(const int& statusCode);
				void				setStatusCode(const std::string& statusCodeString);

				bool				isInformation(void)  const;
				bool				isSuccess(void) const;
				bool				isRedirect(void) const;
				bool				isClientError(void) const;
				bool				isServerError(void) const;

				bool				isError(void) const;

				static HttpStatusCode::StatusCode	getStatusCode(const int& statusCode);
				static HttpStatusCode::StatusCode	getStatusCode(const std::string& statusCodeString);

				static const std::string			getStatusCodeString(const HttpStatusCode::StatusCode& statusCode);

				static bool			isInformation(const HttpStatusCode::StatusCode& statusCode);
				static bool			isSuccess(const HttpStatusCode::StatusCode& statusCode);
				static bool			isRedirect(const HttpStatusCode::StatusCode& statusCode);
				static bool			isClientError(const HttpStatusCode::StatusCode& statusCode);
				static bool			isServerError(const HttpStatusCode::StatusCode& statusCode);

				static bool			isError(const HttpStatusCode::StatusCode& statusCode);

				friend bool	operator==(const HttpStatusCode& lhs, const HttpStatusCode& rhs) {
					return (lhs.statusCode == rhs.statusCode);
				}

				friend bool	operator!=(const HttpStatusCode& lhs, const HttpStatusCode& rhs) {
					return (!(lhs == rhs));
				}

				friend bool	operator<(const HttpStatusCode& lhs, const HttpStatusCode& rhs) {
					return (lhs.statusCode < rhs.statusCode);
				}

				friend bool	operator<=(const HttpStatusCode& lhs, const HttpStatusCode& rhs) {
					return (!(rhs < lhs));
				}

				friend bool	operator>(const HttpStatusCode& lhs, const HttpStatusCode& rhs) {
					return (rhs < lhs);
				}

				friend bool	operator>=(const HttpStatusCode& lhs, const HttpStatusCode& rhs) {
					return (!(lhs < rhs));
				}

		};

	} // namespace Utils

} // namespace Webserv

#endif