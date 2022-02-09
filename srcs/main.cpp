/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgilles <rgilles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 15:47:40 by ppaglier          #+#    #+#             */
/*   Updated: 2022/02/09 22:57:43 by rgilles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.hpp"

namespace Webserv {

	volatile std::sig_atomic_t	stop;

	void	sigint_handler(int signal)
	{
		extern volatile std::sig_atomic_t	stop;
		stop = signal;
	}
}

int	main(int argc, char *argv[], char *envp[]) {

	Webserv::Core &webserv = Webserv::Core::getInstance();

	if (!webserv.preInit(argc, argv, envp)) {
		return EXIT_FAILURE;
	}

	if (!webserv.init()) {
		return EXIT_FAILURE;
	}

	try {
		webserv.exec();
	}
	catch (const std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}

	std::cout << "Success" << std::endl;
	return EXIT_SUCCESS;
}