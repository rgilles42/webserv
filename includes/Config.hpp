/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 16:30:57 by ppaglier          #+#    #+#             */
/*   Updated: 2022/01/28 19:25:39 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <string>					// For string
# include <vector>					// For vector
# include <map>						// For map
# include <algorithm>				// For

# include "./utils/Lexer.hpp"		// For Lexer
# include "./utils/Parser.hpp"		// For Parser
# include "./utils/Block.hpp"		// For Block
# include "./http/Server.hpp"		// For Server
# include "./utils/MimeTypes.hpp"	// For MimeTypes
# include "./Socket.hpp"			// For Socket

namespace Webserv {

	class Config {
		public:
			typedef	std::string						file_type;
			typedef std::vector<file_type>			file_vector;

			typedef	Webserv::Utils::Lexer			lexer_type;
			typedef	Webserv::Utils::Parser			parser_type;
			typedef	parser_type::block_vector		block_vector;
			typedef	parser_type::block_type			block_type;
			typedef parser_type::directive_type		directive_type;
			typedef parser_type::directive_map		directive_map;

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

			void	formatBlocks(block_vector& blocks);

			const directive_map	getDirectiveMap(void) const;
		public:
			Config(void);
			~Config();

			void	reset(void);

			bool	addConfigFile(const file_type& file);

			bool	processFiles(void);

			server_vector	&getServers(void);

			const server_type&	getServer(const std::string& address, const int& port, const std::string& host) const;
	};

} // namespace Webserv

#endif