/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 14:03:05 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/26 17:16:56 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <cstddef>
# include <string>
# include <vector>
# include <map>

# include "../utils/Byte.hpp"

namespace Webserv {

	namespace Http {

		class Server {
			public:
				typedef	Webserv::Utils::Byte		byte_type;

				typedef std::string					name_type;
				typedef std::string					host_type;
				typedef std::string					port_type;

				typedef std::map<std::string, std::string>	error_pages_type;
				typedef	byte_type					client_max_body_size_type;
				typedef std::string					upload_store_type;

				typedef std::vector<std::string>	return_type;
				typedef bool						autoindex_type;
				typedef std::string					root_type;
				typedef std::vector<std::string>	index_type;

				// typedef std::vector<std::string>	limit_except_type;

				typedef std::string					route_type;
				typedef std::map<std::string, route_type>	routes_map;

			protected:
				name_type			serverName;
				host_type			host;
				port_type			port;

				// For routes too

				error_pages_type	error_pages;
				client_max_body_size_type	client_max_body_size;
				upload_store_type	upload_store;

				return_type			_return;
				autoindex_type		autoindex;
				root_type			root;
				index_type			index;

				// Only for routes

				// limit_except_type	limit_except;

				routes_map			routes;

			public:
				Server(void);
				~Server();

				void	setServerName(const name_type &serverName);
				void	setHost(const host_type &host = "0.0.0.0");
				void	setPort(const port_type &port = "80");

				void	setErrorPage(const error_pages_type::key_type &errorCode, const error_pages_type::mapped_type &page);
				void	setClientMaxBodySize(const client_max_body_size_type &size = client_max_body_size_type(1, client_max_body_size_type::U_MB));
		};


		Server::Server(void) {}

		Server::~Server() {}


	} // namespace Http

} // namespace Webserv

#endif