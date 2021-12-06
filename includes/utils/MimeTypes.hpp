/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeTypes.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 16:49:32 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/06 17:15:43 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MIMETYPES_HPP
# define MIMETYPES_HPP

# include <string.h>
# include <map>
# include <vector>
# include <functional>

# include "../utils.hpp"

# include "Block.hpp"

# include "Ci_less.hpp"

# define DEFAULT_MIME_TYPE std::string("application/octet-stream")


namespace Webserv {

	namespace Utils {

		// Need to be into the http module
		class MimeTypes {

			protected:
				typedef Webserv::Utils::Block						block_type;
				typedef std::vector<block_type>						block_vector;

				typedef	std::map<std::string, std::string, Ci_less>	MapType;
				typedef std::vector<std::string>					ReturnExtensionsType;
				/**
				 * Mapped MIME types in key/value
				 * @param key This is the extension of the current type
				 * @param value This is the type of the current extension
				 * if types is not in config, it will not use 'default_type' and it will use another list wich is probably write in the source code of NGINX
				*/
				MapType		mappedTypes;

			public:
				MimeTypes(void);
				MimeTypes(const MimeTypes &x);
				MimeTypes(const std::string &fileContent);
				MimeTypes(const block_vector &blocks);

				// MimeTypes				operator=(const MimeTypes &x);

				void						merge(const MimeTypes &x);

				void						set(const std::string &key, const std::string &value);
				void						clear(void);

				const std::string			&getType(const std::string &path, const std::string &fallback = DEFAULT_MIME_TYPE) const;
				const ReturnExtensionsType	getExtensions(const std::string &type) const;

				void						fromString(const std::string &fileContent);
				void						fromBlocks(const block_vector &blocks);
		};

		// TODO: remove this
		std::string getContentTypeByFile(const std::string &filename, const std::string &fallback);

	}

}

#endif