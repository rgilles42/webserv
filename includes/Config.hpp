/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 16:30:57 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/01 18:38:28 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <string>
# include <vector>
# include <map>
# include <algorithm>

# include "./utils/Lexer.hpp"
# include "./utils/Parser.hpp"
# include "./utils/Block.hpp"

# include "./http/Server.hpp"

# include "./utils/MimeTypes.hpp"

namespace Webserv {

	class Config {
		public:
			typedef	std::string						file_type;
			typedef std::vector<file_type>			file_vector;

			typedef	Webserv::Utils::Lexer			lexer_type;
			typedef	Webserv::Utils::Parser			parser_type;
			typedef	parser_type::block_vector		block_vector;
			typedef	parser_type::block_type			block_type;

			typedef Webserv::Http::Server			server_type;
			typedef std::vector<server_type>		server_vector;

			typedef Webserv::Utils::MimeTypes		mimes_types_type;

		protected:
			file_vector							files;

			std::map<file_type, std::string>	filesMap;
			std::map<file_type, lexer_type>		lexerMap;
			std::map<file_type, parser_type>	parserMap;

			block_vector						blocks;

			server_vector						servers;
			mimes_types_type					globalMimesTypes;

			void	formatBlocks(block_vector &blocks);
		public:
			Config(void);
			~Config();

			bool	addConfigFile(const file_type &file);

			bool	processFiles(void);
	};

} // namespace Webserv

#endif