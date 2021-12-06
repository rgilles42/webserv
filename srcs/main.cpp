/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 15:47:40 by ppaglier          #+#    #+#             */
/*   Updated: 2021/12/06 22:55:12 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.hpp"

volatile sig_atomic_t stop;

void handleSignals(sig_atomic_t signum) {
	(void)signum;
	std::cout << signum << std::endl;
	stop = 1;
}

// int main(void) {

// 	Socket	ServerSocket("0.0.0.0", 8000);
// 	Socket	ClientSocket;
// 	ssize_t	size_read = 0;
// 	char	buffer[BUFFER_SIZE + 1];
// 	std::string	message;


// 	if (ServerSocket.bind() < 0) {
// 		perror("socket bind ");
// 		return EXIT_FAILURE;
// 	}
// 	if (ServerSocket.listen() < 0) {
// 		perror("socket listen ");
// 		return EXIT_FAILURE;
// 	}
// 	printf("La socket %d est maintenant ouverte en mode TCP/IP (%d:%d)\n",
// 		ServerSocket.fd(), ntohl(((struct sockaddr_in*)&ServerSocket.addr())->sin_addr.s_addr),
// 		ntohs(((struct sockaddr_in*)&ServerSocket.addr())->sin_port));
// 	printf("Listage du port %d...\n", ntohs(((struct sockaddr_in*)&ServerSocket.addr())->sin_port));
// 	printf("Patientez pendant que le client se connecte sur le port %d...\n",
// 		ntohs(((struct sockaddr_in*)&ServerSocket.addr())->sin_port));
// 	signal(SIGINT, handleSignals);
// 	while (!stop) {


// 		ClientSocket = ServerSocket.accept();
// 		printf("Un client se connecte avec la socket %d de %s:%d\n",
// 			ClientSocket.fd(), inet_ntoa(((struct sockaddr_in*)&ClientSocket.addr())->sin_addr), htons(((struct sockaddr_in*)&ClientSocket.addr())->sin_port));
// 		message.clear();
// 		printf("Client socket fd is %d\n", ClientSocket.fd());
// 		size_read = ClientSocket.read(buffer, BUFFER_SIZE + 1);
// 		perror("Read status code: ");
// 		if (size_read >= 0)
// 		{
// 			buffer[size_read] = '\0';
// 			message.append(buffer);
// 		}
// 		printf("Chaine reçue : %s\n", message.c_str());

// 		Webserv::Http::HttpRequest request(message);
// 		Ressource currentRessource("./default_pages/index.html");
// 		currentRessource.setContent(getFileContents(currentRessource.getUri()));
// 		currentRessource.setContentType(Webserv::Utils::getContentTypeByFile(currentRessource.getUri(), "text/plain"));
// 		Webserv::Http::HttpResponse response(currentRessource);
// 		ClientSocket.write(response.toString().c_str(), response.toString().length());
// 		printf("Chaine envoyée : %s\n", response.toString().c_str());
// 		shutdown(ClientSocket.fd(), 2);
// 		ClientSocket.close();
// 	}
// 	printf("Fermeture de la socket...\n");
// 	ServerSocket.close();
// 	printf("Fermeture du serveur terminee\n");

// 	return EXIT_SUCCESS;
// }

int main(void) {

	// Webserv::Core webserv;

	// if (!webserv.init()) {
	// 	std::cout << "Fail" << std::endl;
	// 	return EXIT_FAILURE;
	// }
	// std::cout << "Success" << std::endl;

	std::vector<std::string> vec;

	vec.push_back("server_name");
	vec.push_back("google.fr");


	Webserv::Utils::DirectiveServerName dir;

	dir.parseValue(vec);

	std::cout << dir.getName() << std::endl;
	std::cout << dir.getDefaultValue() << std::endl;
	std::cout << dir.getValue() << std::endl;

	return EXIT_SUCCESS;
}
