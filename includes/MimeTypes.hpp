/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeTypes.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 16:49:32 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/12 16:27:31 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MIMETYPES_HPP
# define MIMETYPES_HPP

# include <string.h>
# include <map>
# include <vector>
# include <functional>

# include "utils.hpp"

# define DEFAULT_MIME_TYPE std::string("application/octet-stream")

class MimeTypes {

	protected:
		typedef	std::map<std::string, std::string, ci_less>	MapType;
		typedef std::vector<std::string>					ReturnExtensionsType;
		/**
		 * Mapped MIME types in key/value
		 * @param key This is the extension of the current type
		 * @param value This is the type of the current extension
		*/
		MapType		mappedTypes;

	public:
		MimeTypes(void);
		MimeTypes(const MimeTypes &x);
		MimeTypes(const std::string &fileContent);

		// MimeTypes				operator=(const MimeTypes &x);

		void						merge(const MimeTypes &x);

		void						set(const std::string &key, const std::string &value);

		const std::string			getType(const std::string &path, const std::string &fallback = DEFAULT_MIME_TYPE) const;
		const ReturnExtensionsType	getExtensions(const std::string &type) const;

		void						fromString(const std::string &fileContent);
};

#endif