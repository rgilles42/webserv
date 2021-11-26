/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 16:30:57 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/26 12:03:55 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <string>
# include <vector>
# include <map>

# include "./Lexer.hpp"
# include "./Parser.hpp"
# include "./Block.hpp"
# include "./MimeTypes.hpp"

namespace Webserv {

	namespace Utils {

		class Config {
			public:
				typedef	std::string						file_type;
				typedef std::vector<file_type>			file_vector;

				typedef	Webserv::Utils::Lexer			lexer_type;
				typedef	Webserv::Utils::Parser			parser_type;

				typedef std::string						server_type;
				typedef std::vector<server_type>		server_vector;

				typedef Webserv::Utils::MimeTypes		mimes_types_type;

			protected:
				file_vector							files;

				std::map<file_type, std::string>	filesMap;
				std::map<file_type, lexer_type>		lexerMap;
				std::map<file_type, parser_type>	parserMap;

				server_vector						servers;
				mimes_types_type					globalMimesTypes;

			public:
				Config(void);
				~Config();

				bool	addConfigFile(const file_type &file);

				bool	processFiles(void);
		};

	} // namespace Utils

} // namespace Webserv

#endif