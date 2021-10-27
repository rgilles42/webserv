/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 15:47:40 by ppaglier          #+#    #+#             */
/*   Updated: 2021/10/27 18:41:09 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.hpp"

volatile sig_atomic_t stop;

void handleSignals(sig_atomic_t signum) {
	(void)signum;
	std::cout << signum << std::endl;
	stop = 1;
}

ssize_t receive_basic(SOCKET s, std::string &result)
{
	ssize_t		size_read = 0;
	ssize_t		total_size = 0;
	char	buffer[BUFFER_SIZE + 1];

	while (size_read >= 0) {
		memset(buffer, 0, BUFFER_SIZE + 1);
		size_read =  read(s, buffer, BUFFER_SIZE + 1);
		if(size_read < 0) {
			continue;
		}
		printf("%ld\n", size_read);
		if (size_read == 0) {
			break;
		}
		total_size += size_read;
		result.append(buffer);
	}
	printf("%s\n", result.c_str());
	return total_size;
}

int main(void) {
	int error = 0;
	#if defined (_WIN32) || defined (_WIN64)
		WSADATA WSAData;
		error = WSAStartup(MAKEWORD(2,2), &WSAData);
	#endif

	if (error) {
		perror("windows startup");
		return EXIT_FAILURE;
	}

	SOCKET server_socket;
	SOCKADDR_IN server_addr;
	SOCKET client_socket;
	SOCKADDR_IN client_addr;
	socklen_t recsize = sizeof(client_addr);
	std::string message;
	int sock_err;

	if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket create");
		return EXIT_FAILURE;
	}

	/* Configuration */
	server_addr.sin_addr.s_addr    = htonl(INADDR_ANY);   /* Adresse IP automatique */
	server_addr.sin_family         = AF_INET;             /* Protocole familial (IP) */
	server_addr.sin_port           = htons(PORT);         /* Listage du port */

	if (bind(server_socket, (SOCKADDR*) &server_addr, sizeof(server_addr)) < 0) {
		perror("socket bind");
		return EXIT_FAILURE;
	}

	printf("La socket %d est maintenant ouverte en mode TCP/IP (%d:%d)\n", server_socket, ntohl(server_addr.sin_addr.s_addr), ntohs(server_addr.sin_port));


	/* Démarrage du listage (mode server) */
	if (listen(server_socket, MAX_CONNECTION) < 0) {
		perror("socket listen");
		return EXIT_FAILURE;
	}

	printf("Listage du port %d...\n", ntohs(server_addr.sin_port));

	/* Attente pendant laquelle le client se connecte */
	printf("Patientez pendant que le client se connecte sur le port %d...\n", ntohs(server_addr.sin_port));

	signal(SIGINT, handleSignals);
	while (!stop) {

		client_socket = accept(server_socket, (SOCKADDR*)&client_addr, &recsize);
		printf("Un client se connecte avec la socket %d de %s:%d\n", client_socket, inet_ntoa(client_addr.sin_addr), htons(client_addr.sin_port));

		if(!(fcntl(client_socket, F_GETFL) & O_NONBLOCK)) {
			if(fcntl(client_socket, F_SETFL, fcntl(client_socket, F_GETFL) | O_NONBLOCK) < 0) {

			}
		}

	// sock_err = receive_basic(client_socket, message);

	// if (sock_err < 0) {
	// 	printf("error de récéption\n");
	// } else {
	// 	printf("Chaine reçu : %s\n", message.c_str());
	// }

		HttpRequest request;
		Ressource currentRessource("./default_pages/index.html");

		currentRessource.setContent(getFileContents(currentRessource.getUrl()));
		currentRessource.setContentType(getContentTypeByFile(currentRessource.getUrl()));

		request.prepareRequest();
		request.setContentType(currentRessource.getContentType());
		request.setContentLength(currentRessource.getContent().length());

		sock_err = send(client_socket, request.formatToString().c_str(), request.formatToString().length(), 0);

		if (sock_err < 0) {
			printf("error de transmission\n");
		} else {
			printf("Chaine envoyée : %s\n", request.formatToString().c_str());
		}

		/* Il ne faut pas oublier de fermer la connexion (fermée dans les deux sens) */
		shutdown(client_socket, 2);
		closesocket(client_socket);
	}

	/* Fermeture de la socket */
	printf("Fermeture de la socket...\n");
	closesocket(server_socket);

	printf("Fermeture du serveur terminee\n");
	#if defined (_WIN32) || defined (_WIN64)
		WSACleanup();
	#endif

	return EXIT_SUCCESS;
}

