/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgilles <rgilles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 15:47:40 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/22 18:15:35 by rgilles          ###   ########.fr       */
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
	size_read = recv(s, buffer, BUFFER_SIZE + 1, 0);
	buffer[size_read] = '\0';
	result.append(buffer);
	return result.length();
}

int main(void) {

	//SOCKET server_socket;
	//SOCKADDR_IN server_addr;
	// SOCKET client_socket;
	SOCKADDR_IN client_addr;
	socklen_t recsize = sizeof(client_addr);
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
	Socket	ServerSocket("0.0.0.0", 80);

//	/* Configuration */
//	server_addr.sin_addr.s_addr	= htonl(INADDR_ANY);   /* Adresse IP automatique */
//	server_addr.sin_family		= AF_INET;             /* Protocole familial (IP) */
//	server_addr.sin_port		= htons(PORT);         /* Listage du port */

	if (ServerSocket.bind() < 0) {
		perror("socket bind");
		return EXIT_FAILURE;
	}

	printf("La socket %d est maintenant ouverte en mode TCP/IP (%d:%d)\n", ServerSocket.fd(), ntohl(((struct sockaddr_in*)&ServerSocket.addr())->sin_addr.s_addr), ntohs(((struct sockaddr_in*)&ServerSocket.addr())->sin_port));


	/* Démarrage du listage (mode server) */
	/*if (listen(ServerSocket.fd(), MAX_CONNECTION) < 0) {
		perror("socket listen");
		return EXIT_FAILURE;
	}*/

	printf("Listage du port %d...\n", ntohs(((struct sockaddr_in*)&ServerSocket.addr())->sin_port));

	/* Attente pendant laquelle le client se connecte */
	printf("Patientez pendant que le client se connecte sur le port %d...\n", ntohs(((struct sockaddr_in*)&ServerSocket.addr())->sin_port));

	signal(SIGINT, handleSignals);
	while (!stop) {


		SOCKET client_socket = accept(ServerSocket.fd(), (SOCKADDR*)&client_addr, &recsize);
		printf("Un client se connecte avec la socket %d de %s:%d\n", client_socket, inet_ntoa(client_addr.sin_addr), htons(client_addr.sin_port));

		// if(!(fcntl(client_socket, F_GETFL) & O_NONBLOCK)) {
		// 	if(fcntl(client_socket, F_SETFL, fcntl(client_socket, F_GETFL) | O_NONBLOCK) < 0) {

		// 	}
		// }
		message.clear();
		receive_basic(client_socket, message);

		// if (sock_err < 0) {
		// 	printf("error de récéption\n");
		// } else {
		// 	printf("Chaine reçu : %s\n", message.c_str());
		// }

		HttpRequest request(message);

		Ressource currentRessource("./default_pages/index.html");

		currentRessource.setContent(getFileContents(currentRessource.getUri()));
		currentRessource.setContentType(getContentTypeByFile(currentRessource.getUri()));

		HttpResponse response(currentRessource);

		send(client_socket, response.toString().c_str(), response.toString().length(), 0);

		// if (sock_err < 0) {
		// 	printf("error de transmission\n");
		// } else {
		// 	printf("Chaine envoyée : %s\n", response.toString().c_str());
		// }

		/* Il ne faut pas oublier de fermer la connexion (fermée dans les deux sens) */
		shutdown(client_socket, 2);
		closesocket(client_socket);
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

// int main(void) {
// 	std::cout << "|" << HttpStatusCode::getStatusCodeString(HttpStatusCode::unknown) << "|" << std::endl;
// 	std::cout << "|" << HttpStatusCode::getStatusCodeString(HttpStatusCode::information_continue) << "|" << std::endl;

// 	HttpStatusCode::StatusCode code;

// 	code = HttpStatusCode::getStatusCode(200);
// 	std::cout << "|" << HttpStatusCode::getStatusCodeString(code) << "|" << std::endl;
// 	code = HttpStatusCode::getStatusCode(201);
// 	std::cout << "|" << HttpStatusCode::getStatusCodeString(code) << "|" << std::endl;
// 	code = HttpStatusCode::getStatusCode(189);
// 	std::cout << "|" << HttpStatusCode::getStatusCodeString(code) << "|" << std::endl;
// 	code = HttpStatusCode::getStatusCode("502 Bad Gateway");
// 	std::cout << "|" << HttpStatusCode::getStatusCodeString(code) << "|" << std::endl;

// std::cout << "|" << HttpVersion::getVersion("") << "|" << std::endl;
// std::cout << "|" << HttpVersion::getVersion("HTTP/0.9") << "|" << std::endl;
// std::cout << "|" << HttpVersion::getVersion("HTTP/1.0") << "|" << std::endl;
// std::cout << "|" << HttpVersion::getVersion("HTTP/1.1") << "|" << std::endl;
// std::cout << "|" << HttpVersion::getVersion("HTTP/1.2") << "|" << std::endl;
// std::cout << "|" << HttpVersion::getVersion("HTTP/2") << "|" << std::endl;
// std::cout << "|" << HttpVersion::getVersion("HTTP/3") << "|" << std::endl;

// 	return EXIT_SUCCESS;
// }

// int main(void) {
// 	std::string message = "GET /test?oui=non HTTP/1.1\r\nHost: 127.0.0.1\r\nhost: 128.0.0.1\r\nHost: 129.0.0.1\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\nsec-ch-ua: \"Google Chrome\";v=\"95\", \"Chromium\";v=\"95\", \";Not A Brand\";v=\"99\"\r\nsec-ch-ua-mobile: ?0\r\nsec-ch-ua-platform: \"Windows\"\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/95.0.4638.54 Safari/537.36\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\nSec-Fetch-Site: cross-site\r\nSec-Fetch-Mode: navigate\r\nSec-Fetch-User: ?1\r\nSec-Fetch-Dest: document\r\nAccept-Encoding: gzip, deflate, br\r\nAccept-Language: fr-FR,fr;q=0.9,en-US;q=0.8,en;q=0.7\r\nIf-Modified-Since: Tue, 11 Nov 2021 19:20:25 GMT\r\n\r\n";
// 	HttpRequest request(message);

// 	std::cout << request.get("Host") << std::endl;
// 	std::cout << request.get("host") << std::endl;
// 	std::cout << request.get("Host") << std::endl;
// 	std::cout << request.get("Connection") << std::endl;
// 	std::cout << request.get("Cache-Control") << std::endl;

// 	return EXIT_SUCCESS;
// }
