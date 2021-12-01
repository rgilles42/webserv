/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 14:03:05 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/01 16:23:43 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROUTE_HPP
# define ROUTE_HPP

# include <cstddef>
# include <string>
# include <vector>
# include <map>

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

				typedef std::map<std::string, std::string>	error_pages_type;
				typedef	byte_type					client_max_body_size_type;
				typedef std::string					upload_store_type;

				typedef std::string					return_type;
				typedef bool						autoindex_type;
				typedef std::string					root_type;
				typedef std::vector<std::string>	index_type;

				typedef std::vector<std::string>	limit_except_type;

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

				void	fromBlocks(const block_vector &blocks);

				void	setErrorPage(const error_pages_type::key_type &errorCode, const error_pages_type::mapped_type &page);
				void	setClientMaxBodySize(const client_max_body_size_type &size = client_max_body_size_type(1, client_max_body_size_type::U_MB));
				void	setUploadStore(const upload_store_type &upload_store = "/");

				void	setReturn(const return_type &_return);
				void	setAutoindex(const autoindex_type &autoindex);
				void	setRoot(const root_type &root);
				void	setIndex(const index_type &index);

				void	setLimitExcept(const limit_except_type &limit_except);

				void	addRoute(const routes_map::key_type &path, const routes_map::mapped_type &route);

		};

	} // namespace Http

} // namespace Webserv

#endif