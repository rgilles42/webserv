/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 14:03:05 by ppaglier          #+#    #+#             */
/*   Updated: 2022/02/01 17:56:43 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROUTE_HPP
# define ROUTE_HPP

# include <iostream>

# include <string>						// For string
# include <vector>						// For vector
# include <map>							// For map

# include "../utils/Directive.hpp"		// For Directive
# include "../utils/Block.hpp"			// For Block
# include "../utils/MimeTypes.hpp"		// For MimeTypes
# include "../utils/common.hpp"		// For MimeTypes

namespace Webserv {

	namespace Http {

		class Route {
			public:
				typedef	std::string					path_type;
				typedef	Webserv::Utils::Block		block_type;
				typedef std::vector<block_type>		block_vector;

				typedef Webserv::Utils::MimeTypes	mimes_types_type;

				typedef Webserv::Utils::Directive				directive_type;
				typedef directive_type::http_status_code_type	http_status_code_type;

				typedef directive_type::dir_error_page_type		error_pages_pair;

				typedef std::map<error_pages_pair::first_type, error_pages_pair::second_type>	error_pages_type;
				typedef directive_type::dir_client_max_body_size_type	client_max_body_size_type;
				typedef directive_type::dir_upload_store_type	upload_store_type;

				typedef directive_type::dir_return_type			return_type;
				typedef directive_type::dir_autoindex_type		autoindex_type;
				typedef directive_type::dir_root_type			root_type;
				typedef directive_type::dir_index_type			index_type;

				typedef directive_type::dir_limit_except_type	limit_except_type;

				typedef std::map<std::string, Route>	routes_map;

			protected:
				path_type			currentPath;
				Route*				parent;

				mimes_types_type	mimesTypes;

				error_pages_type	error_pages;
				client_max_body_size_type	client_max_body_size;
				upload_store_type	upload_store;

				return_type			_return;
				autoindex_type		autoindex;
				root_type			root;
				index_type			index;

				limit_except_type	limit_except;

				routes_map			routes;

			public:
				Route(void);
				Route(const Route& other);
				~Route();

				Route&	operator=(const Route &other);

				void	init(void);

				bool	fromBlocks(const block_vector& blocks);

				void	setParent(Route* parent = NULL);
				void	setCurrentPath(const path_type& currentPath);

				void	setMimesTypes(const mimes_types_type& mimesTypes);

				void	setErrorPages(const error_pages_type& errorPages);
				void	setClientMaxBodySize(const client_max_body_size_type& clientMaxBodySize);
				void	setUploadStore(const upload_store_type& uploadStore);
				void	setReturn(const return_type& _return);
				void	setAutoindex(const autoindex_type& autoindex);
				void	setRoot(const root_type& root);
				void	setIndex(const index_type& index);
				void	setLimitExcept(const limit_except_type& limitExcept);

				void	addRoute(const routes_map::key_type& path, const routes_map::mapped_type& route);

				const routes_map&					getRoutes(void) const;

				const path_type&					getCurrentPath(void) const;
				const Route*						getParent(void) const;
				const mimes_types_type&				getMimesTypes(void) const;
				const error_pages_type&				getErrorPages(void) const;
				const client_max_body_size_type&	getClientMaxBodySize(void) const;
				const upload_store_type&			getUploadStore(void) const;
				const return_type&					getReturn(void) const;
				const autoindex_type&				getAutoindex(void) const;
				const root_type&					getRoot(void) const;
				const index_type&					getIndex(void) const;
				const limit_except_type&			getLimitExcept(void) const;

				const std::string					getFilePath(const std::string& url);
		};

	} // namespace Http

} // namespace Webserv

#endif