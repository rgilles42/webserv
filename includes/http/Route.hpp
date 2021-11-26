/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 14:03:05 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/26 17:45:25 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROUTE_HPP
# define ROUTE_HPP

# include <cstddef>
# include <string>
# include <vector>
# include <map>

# include "../utils/Byte.hpp"

namespace Webserv {

	namespace Http {

		class Route {
			public:
				typedef	Webserv::Utils::Byte		byte_type;

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

		Route::Route(void) {}

		Route::~Route() {}

		void 	Route::setErrorPage(const error_pages_type::key_type &errorCode, const error_pages_type::mapped_type &page) {
			this->error_pages[errorCode] = page;
		}

		void 	Route::setClientMaxBodySize(const client_max_body_size_type &client_max_body_size) {
			this->client_max_body_size = client_max_body_size;
		}

		void 	Route::setUploadStore(const upload_store_type &upload_store) {
			this->upload_store = upload_store;
		}

		void 	Route::setReturn(const return_type &_return) {
			this->_return = _return;
		}


		void 	Route::setAutoindex(const autoindex_type &autoindex) {
			this->autoindex = autoindex;
		}

		void 	Route::setRoot(const root_type &root) {
			this->root = root;
		}

		void 	Route::setIndex(const index_type &index) {
			this->index = index;
		}

		void 	Route::setLimitExcept(const limit_except_type &limit_except) {
			this->limit_except = limit_except;
		}

		void 	Route::addRoute(const routes_map::key_type &path, const routes_map::mapped_type &route) {
			this->routes[path] = route;
		}

	} // namespace Http

} // namespace Webserv

#endif