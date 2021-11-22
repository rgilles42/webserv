/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgilles <rgilles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 15:47:40 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/22 18:32:35 by rgilles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.hpp"

/*int main(void)
{
	Socket	ServerSocket("0.0.0.0", 80);
	Socket	ClientSocket;
	char 	req[4096] = {0};
	const char* 	resp = "HTTP/1.1 200 OK\r\nContent-Length: 10\r\n\r\nhi there!\n\r\n";

	ServerSocket.bind();
	ClientSocket = ServerSocket.accept();
	ClientSocket.read(req, 4096);
	printf("%suwu\n", req);
	ClientSocket.write(resp, strlen(resp));
}*/

volatile sig_atomic_t stop;

void handleSignals(sig_atomic_t signum) {
	(void)signum;
	std::cout << signum << std::endl;
	stop = 1;
}

ssize_t receive_basic(SOCKET s, std::string &result)
{
	ssize_t		size_read = 0;
	char	buffer[BUFFER_SIZE + 1];

	// while (size_read >= 0) {
	// 	memset(buffer, 0, BUFFER_SIZE + 1);
	// 	size_read =  read(s, buffer, BUFFER_SIZE + 1);
	// 	if(size_read < 0) {
	// 		continue;
	// 	}
	// 	printf("%ld\n", size_read);
	// 	if (size_read == 0) {
	// 		break;
	// 	}
	// 	result.append(buffer);
	// }
	size_read = read(s, buffer, BUFFER_SIZE + 1);
	buffer[size_read] = '\0';
	result.append(buffer);
	return result.length();
}

int main(void) {

	//SOCKET server_socket;
	//SOCKADDR_IN server_addr;
	// SOCKET ClientSocket;
	//SOCKADDR_IN client_addr;
	//socklen_t recsize = sizeof(client_addr);
	std::string message;

	/*if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket create");
		return EXIT_FAILURE;
	}
	int optval = 1;
	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
		perror("setsockopt(SO_REUSEADDR) failed");
		return EXIT_FAILURE;
	}*/
	Socket	ServerSocket("0.0.0.0", 8000);
	Socket	ClientSocket;

//	/* Configuration */
//	server_addr.sin_addr.s_addr	= htonl(INADDR_ANY);   /* Adresse IP automatique */
//	server_addr.sin_family		= AF_INET;             /* Protocole familial (IP) */
//	server_addr.sin_port		= htons(PORT);         /* Listage du port */

	if (ServerSocket.bind() < 0) {
		perror("socket bind");
		return EXIT_FAILURE;
	}

	printf("La socket %d est maintenant ouverte en mode TCP/IP (%d:%d)\n",
		ServerSocket.fd(), ntohl(((struct sockaddr_in*)&ServerSocket.addr())->sin_addr.s_addr),
		ntohs(((struct sockaddr_in*)&ServerSocket.addr())->sin_port));


	/* Démarrage du listage (mode server) */
	/*if (listen(ServerSocket.fd(), MAX_CONNECTION) < 0) {
		perror("socket listen");
		return EXIT_FAILURE;
	}*/

	printf("Listage du port %d...\n", ntohs(((struct sockaddr_in*)&ServerSocket.addr())->sin_port));

	/* Attente pendant laquelle le client se connecte */
	printf("Patientez pendant que le client se connecte sur le port %d...\n",
		ntohs(((struct sockaddr_in*)&ServerSocket.addr())->sin_port));

	signal(SIGINT, handleSignals);
	while (!stop) {


		ClientSocket = ServerSocket.accept();
		printf("Un client se connecte avec la socket %d de %s:%d\n",
			ClientSocket.fd(), inet_ntoa(((struct sockaddr_in*)&ClientSocket.addr())->sin_addr), htons(((struct sockaddr_in*)&ClientSocket.addr())->sin_port));

		// if(!(fcntl(ClientSocket.fd(), F_GETFL) & O_NONBLOCK)) {
		// 	if(fcntl(ClientSocket.fd(), F_SETFL, fcntl(ClientSocket.fd(), F_GETFL) | O_NONBLOCK) < 0) {

		// 	}
		// }
		message.clear();
		receive_basic(ClientSocket.fd(), message);

		// if (sock_err < 0) {
		// 	printf("error de récéption\n");
		// } else {
		printf("Chaine reçue : %s\n", message.c_str());
		// }

		HttpRequest request(message);

		Ressource currentRessource("./default_pages/index.html");

		currentRessource.setContent(getFileContents(currentRessource.getUri()));
		currentRessource.setContentType(getContentTypeByFile(currentRessource.getUri()));

		HttpResponse response(currentRessource);

		send(ClientSocket.fd(), response.toString().c_str(), response.toString().length(), 0);

		// if (sock_err < 0) {
		// 	printf("error de transmission\n");
		// } else {
		// 	printf("Chaine envoyée : %s\n", response.toString().c_str());
		// }

		/* Il ne faut pas oublier de fermer la connexion (fermée dans les deux sens) */
		shutdown(ClientSocket.fd(), 2);
	}

	/* Fermeture de la socket */
	printf("Fermeture de la socket...\n");
	//closesocket(ServerSocket.fd());

	printf("Fermeture du serveur terminee\n");
	#if defined (_WIN32) || defined (_WIN64)
		WSACleanup();
	#endif

	return EXIT_SUCCESS;
}

