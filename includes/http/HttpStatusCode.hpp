/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpStatusCode.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 00:38:38 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/03 12:41:58 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPSTATUSCODE_HPP
# define HTTPSTATUSCODE_HPP

# include <string>
# include <map>

namespace Webserv {

	namespace Http {
		// Need MIME config

		struct HttpMethod {

			enum Method {
				unknown = 0,

				get = 1,
			};

		};

		struct HttpStatusCode {

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

			static HttpStatusCode::StatusCode	getStatusCode(const int &statusCode = 0);
			static HttpStatusCode::StatusCode	getStatusCode(const std::string &statusCodeString);

			static std::string	getStatusCodeString(const HttpStatusCode::StatusCode &statusCode = HttpStatusCode::unknown);

			bool				isInformation(const HttpStatusCode::StatusCode &statusCode);
			bool				isSuccess(const HttpStatusCode::StatusCode &statusCode);
			bool				isRedirect(const HttpStatusCode::StatusCode &statusCode);
			bool				isClientError(const HttpStatusCode::StatusCode &statusCode);
			bool				isServerError(const HttpStatusCode::StatusCode &statusCode);

		};

		static const std::pair<HttpStatusCode::StatusCode, std::string> HttpStatusCodeStringData[] = {
			std::make_pair(HttpStatusCode::unknown, ""),

			std::make_pair(HttpStatusCode::information_continue, "100 Continue"),
			std::make_pair(HttpStatusCode::information_switching_protocols, "101 Switching Protocols"),
			std::make_pair(HttpStatusCode::information_processing, "102 Processing"),
			std::make_pair(HttpStatusCode::information_early_hints, "103 Early Hints"),

			std::make_pair(HttpStatusCode::success_ok, "200 OK"),
			std::make_pair(HttpStatusCode::success_created, "201 Created"),
			std::make_pair(HttpStatusCode::success_accepted, "202 Accepted"),
			std::make_pair(HttpStatusCode::success_non_authoritative_information, "203 Non-Authoritative Information"),
			std::make_pair(HttpStatusCode::success_no_content, "204 No Content"),
			std::make_pair(HttpStatusCode::success_reset_content, "205 Reset Content"),
			std::make_pair(HttpStatusCode::success_partial_content, "206 Partial Content"),
			std::make_pair(HttpStatusCode::success_multi_status, "207 Multi-Status"),
			std::make_pair(HttpStatusCode::success_already_reported, "208 Already Reported"),
			std::make_pair(HttpStatusCode::success_im_used, "226 IM Used"),

			std::make_pair(HttpStatusCode::redirection_multiple_choices, "300 Multiple Choices"),
			std::make_pair(HttpStatusCode::redirection_moved_permanently, "301 Moved Permanently"),
			std::make_pair(HttpStatusCode::redirection_found, "302 Found"),
			std::make_pair(HttpStatusCode::redirection_see_other, "303 See Other"),
			std::make_pair(HttpStatusCode::redirection_not_modified, "304 Not Modified"),
			std::make_pair(HttpStatusCode::redirection_use_proxy, "305 Use Proxy"),
			std::make_pair(HttpStatusCode::redirection_switch_proxy, "306 Switch Proxy"), // unused
			std::make_pair(HttpStatusCode::redirection_temporary_redirect, "307 Temporary Redirect"),
			std::make_pair(HttpStatusCode::redirection_permanent_redirect, "308 Permanent Redirect"),

			std::make_pair(HttpStatusCode::client_error_bad_request, "400 Bad Request"),
			std::make_pair(HttpStatusCode::client_error_unauthorized, "401 Unauthorized"),
			std::make_pair(HttpStatusCode::client_error_payment_required, "402 Payment Required"),
			std::make_pair(HttpStatusCode::client_error_forbidden, "403 Forbidden"),
			std::make_pair(HttpStatusCode::client_error_not_found, "404 Not Found"),
			std::make_pair(HttpStatusCode::client_error_method_not_allowed, "405 Method Not Allowed"),
			std::make_pair(HttpStatusCode::client_error_not_acceptable, "406 Not Acceptable"),
			std::make_pair(HttpStatusCode::client_error_proxy_authentication_required, "407 Proxy Authentication Required"),
			std::make_pair(HttpStatusCode::client_error_request_timeout, "408 Request Timeout"),
			std::make_pair(HttpStatusCode::client_error_conflict, "409 Conflict"),
			std::make_pair(HttpStatusCode::client_error_gone, "410 Gone"),
			std::make_pair(HttpStatusCode::client_error_length_required, "411 Length Required"),
			std::make_pair(HttpStatusCode::client_error_precondition_failed, "412 Precondition Failed"),
			std::make_pair(HttpStatusCode::client_error_payload_too_large, "413 Payload Too Large"),
			std::make_pair(HttpStatusCode::client_error_uri_too_long, "414 URI Too Long"),
			std::make_pair(HttpStatusCode::client_error_unsupported_media_type, "415 Unsupported Media Type"),
			std::make_pair(HttpStatusCode::client_error_range_not_satisfiable, "416 Range Not Satisfiable"),
			std::make_pair(HttpStatusCode::client_error_expectation_failed, "417 Expectation Failed"),
			std::make_pair(HttpStatusCode::client_error_im_a_teapot, "418 I'm a teapot"),
			std::make_pair(HttpStatusCode::client_error_misdirection_required, "421 Misdirected Request"),
			std::make_pair(HttpStatusCode::client_error_unprocessable_entity, "422 Unprocessable Entity"),
			std::make_pair(HttpStatusCode::client_error_locked, "423 Locked"),
			std::make_pair(HttpStatusCode::client_error_failed_dependency, "424 Failed Dependency"),
			std::make_pair(HttpStatusCode::client_error_too_early, "424 Too Early"),
			std::make_pair(HttpStatusCode::client_error_upgrade_required, "426 Upgrade Required"),
			std::make_pair(HttpStatusCode::client_error_precondition_required, "428 Precondition Required"),
			std::make_pair(HttpStatusCode::client_error_too_many_requests, "429 Too Many Requests"),
			std::make_pair(HttpStatusCode::client_error_request_header_fields_too_large, "431 Request Header Fields Too Large"),
			std::make_pair(HttpStatusCode::client_error_unavailable_for_legal_reasons, "451 Unavailable For Legal Reasons"),

			std::make_pair(HttpStatusCode::server_error_internal_server_error, "500 Internal Server Error"),
			std::make_pair(HttpStatusCode::server_error_not_implemented, "501 Not Implemented"),
			std::make_pair(HttpStatusCode::server_error_bad_gateway, "502 Bad Gateway"),
			std::make_pair(HttpStatusCode::server_error_service_unavailable, "503 Service Unavailable"),
			std::make_pair(HttpStatusCode::server_error_gateway_timeout, "504 Gateway Timeout"),
			std::make_pair(HttpStatusCode::server_error_http_version_not_supported, "505 HTTP Version Not Supported"),
			std::make_pair(HttpStatusCode::server_error_variant_also_negotiates, "506 Variant Also Negotiates"),
			std::make_pair(HttpStatusCode::server_error_insufficient_storage, "507 Insufficient Storage"),
			std::make_pair(HttpStatusCode::server_error_loop_detected, "508 Loop Detected"),
			std::make_pair(HttpStatusCode::server_error_not_extended, "510 Not Extended"),
			std::make_pair(HttpStatusCode::server_error_network_authentication_required, "511 Network Authentication Required"),
		};

		static const HttpStatusCode::StatusCodeStringMap HttpStatusCodeStrings(HttpStatusCodeStringData, HttpStatusCodeStringData + (sizeof HttpStatusCodeStringData / sizeof HttpStatusCodeStringData[0]));

	} // namespace Utils

} // namespace Webserv

#endif