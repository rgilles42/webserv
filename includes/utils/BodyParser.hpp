/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BodyParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgilles <rgilles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 17:15:49 by rgilles           #+#    #+#             */
/*   Updated: 2022/02/08 17:43:24 by rgilles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BODYPARSER_HPP
#define BODYPARSER_HPP

# include <iostream>

namespace Webserv
{
	namespace Utils
	{
		class BodyParser
		{
			public:
				BodyParser(std::string to_parse);
				~BodyParser();
			private:
				std::string	_filename;
				std::string	_contentType;
				std::string	_content;
		};
	}
}

#endif