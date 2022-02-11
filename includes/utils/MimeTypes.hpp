/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeTypes.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 16:49:32 by ppaglier          #+#    #+#             */
/*   Updated: 2022/02/11 22:56:57 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MIMETYPES_HPP
# define MIMETYPES_HPP
# include <string>			// For string
# include <map>				// For map
# include <vector>			// For vector
# include <algorithm>		// For find_if, ptr_fun
# include <cctype>			// For isspace
# include <iterator>		// For distance
# include <sstream>			// For stringstream

# include "common.hpp"		// For trim
# include "Block.hpp"		// For Block
# include "ci_less.hpp"		// For ci_less

# define DEFAULT_MIME_TYPE std::string("application/octet-stream")


namespace Webserv {

	namespace Utils {

		// Need to be into the http module
		class MimeTypes {

			protected:
				typedef Webserv::Utils::Block		block_type;
				typedef std::vector<block_type>		block_vector;

				typedef	std::map<std::string, std::string, Webserv::Utils::ci_less>	MapType;
				typedef std::vector<std::string>	ReturnExtensionsType;
				/**
				 * Mapped MIME types in key/value
				 * @param key This is the extension of the current type
				 * @param value This is the type of the current extension
				 * if types is not in config, it will not use 'default_type' and it will use another list wich is probably write in the source code of NGINX
				*/
				MapType		mappedTypes;

			public:
				MimeTypes(void);
				MimeTypes(const MimeTypes& other);
				MimeTypes(const block_vector& blocks);

				MimeTypes&					operator=(const MimeTypes& x);

				~MimeTypes();

				void						merge(const MimeTypes& x);

				void						set(const std::string& key, const std::string& value);
				void						clear(void);

				const std::string&			getType(const std::string& path, const std::string& fallback = DEFAULT_MIME_TYPE) const;
				const ReturnExtensionsType	getExtensions(const std::string& type) const;

				void						fromString(const std::string& fileContent);
				bool						fromBlocks(const block_vector& blocks);
		};

	}

}

#endif