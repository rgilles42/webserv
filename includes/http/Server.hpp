/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 14:03:05 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/07 18:23:32 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <cstddef>
# include <string>
# include <vector>
# include <map>
# include <algorithm>

# include "Route.hpp"

# include "../utils/Directive.hpp"

# include "../utils/Block.hpp"
# include "../utils/Byte.hpp"
# include "../utils/MimeTypes.hpp"

# define DEFAULT_SERVER_NAME Webserv::Http::Server::name_type("")
# define DEFAULT_LISTEN Webserv::Http::Server::listen_type("0.0.0.0:80")
# define DEFAULT_CLIENT_MAX_BODY_SIZE Webserv::Http::Server::client_max_body_size_type(1, Webserv::Http::Server::client_max_body_size_type::U_MB)

namespace Webserv {

	namespace Http {

		class Server {
			public:
				typedef	Webserv::Utils::Byte					byte_type;
				typedef	Webserv::Utils::Block					block_type;
				typedef std::vector<block_type>					block_vector;

				typedef Webserv::Utils::MimeTypes				mimes_types_type;

				typedef Webserv::Utils::Directive				directive_type;

				typedef directive_type::dir_server_name_type	name_type;
				typedef directive_type::dir_listen_type			listen_type;

				typedef directive_type::dir_error_page_type		error_pages_pair;

				typedef std::map<error_pages_pair::first_type, error_pages_pair::second_type>	error_pages_type;
				typedef directive_type::dir_client_max_body_size_type	client_max_body_size_type;
				typedef directive_type::dir_upload_store_type	upload_store_type;

				typedef directive_type::dir_return_type			return_type;
				typedef directive_type::dir_autoindex_type		autoindex_type;
				typedef directive_type::dir_root_type			root_type;
				typedef directive_type::dir_index_type			index_type;

				// typedef directive_type::dir_limit_except_type	limit_except_type;

				typedef Webserv::Http::Route				route_type;
				typedef std::map<std::string, route_type>	routes_map;

			protected:
				mimes_types_type	mimesTypes;

				name_type			serverName;
				listen_type			listen;

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

				bool	fromBlocks(const block_vector &blocks);

				void	setMimesTypes(const mimes_types_type &mimestypes);

				void	setServerName(const name_type &serverName = DEFAULT_SERVER_NAME);
				void	setListen(const listen_type &listen = DEFAULT_LISTEN);

				void	setErrorPage(const error_pages_type::key_type &errorCode, const error_pages_type::mapped_type &page);
				void	setClientMaxBodySize(const client_max_body_size_type &client_max_body_size = DEFAULT_CLIENT_MAX_BODY_SIZE);
				void	setUploadStore(const upload_store_type &upload_store);

				void	setReturn(const return_type &_return);
				void	setAutoindex(const autoindex_type &autoindex);
				void	setRoot(const root_type &root);
				void	setIndex(const index_type &index);

				void	addRoute(const routes_map::key_type &path, const routes_map::mapped_type &route);

		};

	} // namespace Http

} // namespace Webserv

#endif