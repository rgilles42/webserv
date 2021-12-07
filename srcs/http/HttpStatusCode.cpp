/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpStatusCode.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 01:17:46 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/07 18:06:24 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http/HttpStatusCode.hpp"

namespace Webserv {

	namespace Http {

		const HttpStatusCode::StatusCodeStringMap HttpStatusCode::HttpStatusCodeStrings =  HttpStatusCode::fillMap();

		const HttpStatusCode::StatusCodeStringMap HttpStatusCode::fillMap(void) {
			StatusCodeStringMap map;

			map[HttpStatusCode::unknown] = "";

			map[HttpStatusCode::information_continue] = "100 Continue";
			map[HttpStatusCode::information_switching_protocols] = "101 Switching Protocols";
			map[HttpStatusCode::information_processing] = "102 Processing";
			map[HttpStatusCode::information_early_hints] = "103 Early Hints";

			map[HttpStatusCode::success_ok] = "200 OK";
			map[HttpStatusCode::success_created] = "201 Created";
			map[HttpStatusCode::success_accepted] = "202 Accepted";
			map[HttpStatusCode::success_non_authoritative_information] = "203 Non-Authoritative Information";
			map[HttpStatusCode::success_no_content] = "204 No Content";
			map[HttpStatusCode::success_reset_content] = "205 Reset Content";
			map[HttpStatusCode::success_partial_content] = "206 Partial Content";
			map[HttpStatusCode::success_multi_status] = "207 Multi-Status";
			map[HttpStatusCode::success_already_reported] = "208 Already Reported";
			map[HttpStatusCode::success_im_used] = "226 IM Used";

			map[HttpStatusCode::redirection_multiple_choices] = "300 Multiple Choices";
			map[HttpStatusCode::redirection_moved_permanently] = "301 Moved Permanently";
			map[HttpStatusCode::redirection_found] = "302 Found";
			map[HttpStatusCode::redirection_see_other] = "303 See Other";
			map[HttpStatusCode::redirection_not_modified] = "304 Not Modified";
			map[HttpStatusCode::redirection_use_proxy] = "305 Use Proxy";
			map[HttpStatusCode::redirection_switch_proxy] = "306 Switch Proxy"; // unused
			map[HttpStatusCode::redirection_temporary_redirect] = "307 Temporary Redirect";
			map[HttpStatusCode::redirection_permanent_redirect] = "308 Permanent Redirect";

			map[HttpStatusCode::client_error_bad_request] = "400 Bad Request";
			map[HttpStatusCode::client_error_unauthorized] = "401 Unauthorized";
			map[HttpStatusCode::client_error_payment_required] = "402 Payment Required";
			map[HttpStatusCode::client_error_forbidden] = "403 Forbidden";
			map[HttpStatusCode::client_error_not_found] = "404 Not Found";
			map[HttpStatusCode::client_error_method_not_allowed] = "405 Method Not Allowed";
			map[HttpStatusCode::client_error_not_acceptable] = "406 Not Acceptable";
			map[HttpStatusCode::client_error_proxy_authentication_required] = "407 Proxy Authentication Required";
			map[HttpStatusCode::client_error_request_timeout] = "408 Request Timeout";
			map[HttpStatusCode::client_error_conflict] = "409 Conflict";
			map[HttpStatusCode::client_error_gone] = "410 Gone";
			map[HttpStatusCode::client_error_length_required] = "411 Length Required";
			map[HttpStatusCode::client_error_precondition_failed] = "412 Precondition Failed";
			map[HttpStatusCode::client_error_payload_too_large] = "413 Payload Too Large";
			map[HttpStatusCode::client_error_uri_too_long] = "414 URI Too Long";
			map[HttpStatusCode::client_error_unsupported_media_type] = "415 Unsupported Media Type";
			map[HttpStatusCode::client_error_range_not_satisfiable] = "416 Range Not Satisfiable";
			map[HttpStatusCode::client_error_expectation_failed] = "417 Expectation Failed";
			map[HttpStatusCode::client_error_im_a_teapot] = "418 I'm a teapot";
			map[HttpStatusCode::client_error_misdirection_required] = "421 Misdirected Request";
			map[HttpStatusCode::client_error_unprocessable_entity] = "422 Unprocessable Entity";
			map[HttpStatusCode::client_error_locked] = "423 Locked";
			map[HttpStatusCode::client_error_failed_dependency] = "424 Failed Dependency";
			map[HttpStatusCode::client_error_too_early] = "424 Too Early";
			map[HttpStatusCode::client_error_upgrade_required] = "426 Upgrade Required";
			map[HttpStatusCode::client_error_precondition_required] = "428 Precondition Required";
			map[HttpStatusCode::client_error_too_many_requests] = "429 Too Many Requests";
			map[HttpStatusCode::client_error_request_header_fields_too_large] = "431 Request Header Fields Too Large";
			map[HttpStatusCode::client_error_unavailable_for_legal_reasons] = "451 Unavailable For Legal Reasons";

			map[HttpStatusCode::server_error_internal_server_error] = "500 Internal Server Error";
			map[HttpStatusCode::server_error_not_implemented] = "501 Not Implemented";
			map[HttpStatusCode::server_error_bad_gateway] = "502 Bad Gateway";
			map[HttpStatusCode::server_error_service_unavailable] = "503 Service Unavailable";
			map[HttpStatusCode::server_error_gateway_timeout] = "504 Gateway Timeout";
			map[HttpStatusCode::server_error_http_version_not_supported] = "505 HTTP Version Not Supported";
			map[HttpStatusCode::server_error_variant_also_negotiates] = "506 Variant Also Negotiates";
			map[HttpStatusCode::server_error_insufficient_storage] = "507 Insufficient Storage";
			map[HttpStatusCode::server_error_loop_detected] = "508 Loop Detected";
			map[HttpStatusCode::server_error_not_extended] = "510 Not Extended";
			map[HttpStatusCode::server_error_network_authentication_required] = "511 Network Authentication Required";

			return map;
		}

		HttpStatusCode::StatusCode	HttpStatusCode::getStatusCode(const int &statusCode) {
			const HttpStatusCode::StatusCode newStatusCode = static_cast<HttpStatusCode::StatusCode>(statusCode);
			if (HttpStatusCode::HttpStatusCodeStrings.count(newStatusCode) <= 0) {
				return HttpStatusCode::unknown;
			}
			return newStatusCode;
		}

		HttpStatusCode::StatusCode	HttpStatusCode::getStatusCode(const std::string &statusCodeString) {
			HttpStatusCode::StatusCodeStringMap::const_iterator it = HttpStatusCode::HttpStatusCodeStrings.begin();

			while (it != HttpStatusCode::HttpStatusCodeStrings.end()) {
				if (it->second == statusCodeString) {
					return it->first;
				}
				it++;
			}
			return HttpStatusCode::unknown;
		}

		std::string					HttpStatusCode::getStatusCodeString(const HttpStatusCode::StatusCode &statusCode) {
			if (HttpStatusCode::HttpStatusCodeStrings.count(statusCode) <= 0) {
				return HttpStatusCode::HttpStatusCodeStrings.at(HttpStatusCode::unknown);
			}
			return HttpStatusCode::HttpStatusCodeStrings.at(statusCode);
		}

		bool						HttpStatusCode::isInformation(const HttpStatusCode::StatusCode &statusCode) {
			int newStatusCode = static_cast<int>(statusCode);
			return (HttpStatusCode::information_offset <= newStatusCode && newStatusCode < HttpStatusCode::success_offset);
		}

		bool						HttpStatusCode::isSuccess(const HttpStatusCode::StatusCode &statusCode) {
			int newStatusCode = static_cast<int>(statusCode);
			return (HttpStatusCode::success_offset <= newStatusCode && newStatusCode < HttpStatusCode::redirection_offset);
		}

		bool						HttpStatusCode::isRedirect(const HttpStatusCode::StatusCode &statusCode) {
			int newStatusCode = static_cast<int>(statusCode);
			return (HttpStatusCode::redirection_offset <= newStatusCode && newStatusCode < HttpStatusCode::client_error_offset);
		}

		bool						HttpStatusCode::isClientError(const HttpStatusCode::StatusCode &statusCode) {
			int newStatusCode = static_cast<int>(statusCode);
			return (HttpStatusCode::client_error_offset <= newStatusCode && newStatusCode < HttpStatusCode::server_error_offset);
		}

		bool						HttpStatusCode::isServerError(const HttpStatusCode::StatusCode &statusCode) {
			int newStatusCode = static_cast<int>(statusCode);
			return (HttpStatusCode::server_error_offset <= newStatusCode && newStatusCode < HttpStatusCode::misc_error_offset);
		}

	} // namespace Http

} // namespace Webserv
