/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 14:03:05 by ppaglier          #+#    #+#             */
/*   Updated: 2022/02/07 15:27:16 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>

# include <string>						// For string
# include <vector>						// For vector
# include <map>							// For map

# include "Route.hpp"					// For Route
# include "../utils/Directive.hpp"		// For Directive
# include "../utils/Block.hpp"			// For Block
# include "../utils/MimeTypes.hpp"		// For MimeTypes

namespace Webserv {

	namespace Http {

		class Server {
			public:
				typedef	Webserv::Utils::Block					block_type;
				typedef std::vector<block_type>					block_vector;

				typedef Webserv::Utils::MimeTypes				mime_types_type;

				typedef Webserv::Utils::Directive				directive_type;
				typedef directive_type::http_status_code_type	http_status_code_type;

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

				typedef directive_type::dir_cgi_pass_type		cgi_pass_type;
				typedef directive_type::dir_cgi_ext_type		cgi_ext_type;

				typedef Webserv::Http::Route				route_type;
				typedef std::map<std::string, route_type>	routes_map;

			protected:
				mime_types_type	mimeTypes;

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

				cgi_pass_type		cgi_pass;
				cgi_ext_type		cgi_ext;

				route_type			defaultRoute;
				routes_map			routes;

			public:
				Server(void);
				Server(const Server& other);
				~Server();

				Server&	operator=(const Server &other);

				void	init(void);

				bool	fromBlocks(const block_vector& blocks);

				void	setMimeTypes(const mime_types_type& mimetypes);

				void	addRoute(const routes_map::key_type& path, const routes_map::mapped_type& route);

				const name_type	&getServerName(void) const;
				const listen_type	&getListen(void) const;
				const route_type	&getDefaultRoute(void) const;
				const routes_map	&getRoutes(void) const;
				const upload_store_type &getUploadStore(void) const;
		};

	} // namespace Http

} // namespace Webserv

#endif