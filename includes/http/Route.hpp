/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 14:03:05 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/26 14:29:25 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROUTE_HPP
# define ROUTE_HPP

# include <cstddef>
# include <string>
# include <vector>
# include <map>

namespace Webserv {

	namespace Http {

		class Route {
			public:

				typedef std::map<std::string, std::string>	error_pages_type;
				typedef	std::size_t					client_max_body_size_type;
				typedef std::string					upload_store_type;

				typedef std::string					return_type;
				typedef bool						autoindex_type;
				typedef std::string					root_type;
				typedef std::vector<std::string>	index_type;

				typedef std::vector<std::string>	limit_except_type;

				typedef Route						route_type;
				typedef std::map<std::string, route_type>	routes_map;

			protected:

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
		};


		Route::Route(void) {}

		Route::~Route() {}


	} // namespace Http

} // namespace Webserv

#endif