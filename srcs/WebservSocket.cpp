/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservSocket.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgilles <rgilles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 16:51:02 by rgilles           #+#    #+#             */
/*   Updated: 2021/11/15 18:01:27 by rgilles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/WebservSocket.hpp"

WebservSocket::WebservSocket() : _fd(socket(AF_INET, SOCK_STREAM, 0))
{
	if (this->-fd < 0)
		throw SocketNotCreatedException();
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_addr =
	this->_addr.sin_addr =

}

WebservSocket::~WebservSocket() {}