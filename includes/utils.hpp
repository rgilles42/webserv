/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 15:47:23 by ppaglier          #+#    #+#             */
/*   Updated: 2021/10/28 18:06:48 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <sstream>
# include <fstream>
# include <streambuf>
# include <string>
# include <ctime>
# include <algorithm>

#include <iostream>

# define SSTR( x ) static_cast< std::ostringstream & >( ( std::ostringstream() << std::dec << x ) ).str()

std::string	&ltrim(std::string &s);
std::string	&rtrim(std::string &s);
std::string	&trim(std::string &s);

std::string	getFormatedDate(const time_t &rawtime);

std::string	getFileContents(const std::string &filename);
std::string	getFileExtension(const std::string &filename);

std::string getContentTypeByFile(const std::string &filename, const std::string &fallback = "text/plain");

#endif