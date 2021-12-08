/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 14:03:05 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/08 10:08:02 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROUTE_HPP
# define ROUTE_HPP

# include <cstddef>
# include <string>
# include <vector>
# include <map>

# include "../utils/Directive.hpp"

# include "../utils/Byte.hpp"
# include "../utils/Block.hpp"
# include "../utils/MimeTypes.hpp"

namespace Webserv {

	namespace Http {

		class Route {
			public:
				typedef	Webserv::Utils::Byte		byte_type;
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

				typedef Route						route_type;
				typedef std::map<std::string, route_type>	routes_map;

			protected:
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
				~Route();

				void	init(void);

				bool	fromBlocks(const block_vector &blocks);

				void	setMimesTypes(const mimes_types_type &mimesTypes);

				void	addRoute(const routes_map::key_type &path, const routes_map::mapped_type &route);

		};

	} // namespace Http

} // namespace Webserv

#endif