/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 15:47:40 by ppaglier          #+#    #+#             */
/*   Updated: 2022/01/29 22:28:00 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.hpp"

int main(int argc, char *argv[], char *envp[]) {

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

// volatile sig_atomic_t stop;

// void handleSignals(sig_atomic_t signum) {
// 	(void)signum;
// 	std::cout << signum << std::endl;
// 	stop = 1;
// }

// int main(void) {

// 	Socket	ServerSocket("0.0.0.0", 80, 1);
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


	// 	ClientSocket = ServerSocket.accept(1);
	// 	printf("Un client se connecte avec la socket %d de %s:%d\n",
	// 		ClientSocket.fd(), inet_ntoa(((struct sockaddr_in*)&ClientSocket.addr())->sin_addr), htons(((struct sockaddr_in*)&ClientSocket.addr())->sin_port));
	// 	message.clear();
	// 	printf("Client socket fd is %d\n", ClientSocket.fd());
	// 	size_read = ClientSocket.read(buffer, BUFFER_SIZE + 1);
	// 	perror("Read status code: ");
	// 	if (size_read >= 0)
	// 	{
	// 		buffer[size_read] = '\0';
	// 		message.append(buffer);
	// 	}
	// 	printf("Chaine reçue : %s\n", message.c_str());

	// 	Webserv::Http::HttpRequest request(message);
	// 	Webserv::Resource currentRessource("./default_pages/index.html");
	// 	Webserv::Http::HttpResponse response(currentRessource);
	// 	ClientSocket.write(response.toString().c_str(), response.toString().length());
	// 	printf("Chaine envoyée : %s\n", response.toString().c_str());
	// 	shutdown(ClientSocket.fd(), 2);
	// 	ClientSocket.close();
	// }
	// printf("Fermeture de la socket...\n");
	// ServerSocket.close();
	// printf("Fermeture du serveur terminee\n");

// 	return EXIT_SUCCESS;
// }

// std::string	intToHex(int i) {
// 	std::stringstream stream;
// 	stream << std::hex << i;
// 	return stream.str();
// }

// int main(void)
// {
// 	Webserv::Http::HttpRequestBuilder builder;

// 	builder.addMessage("                              ");
// 	std::cout << "empty: " << builder.checkBuffer() << std::endl;
// 	builder.addMessage("     GET /index HTTP/1.1\r\n");
// 	std::cout << "start-line: " << builder.checkBuffer() << std::endl;
// 	builder.addMessage("Host: code.tutsplus.com\r\n");
// 	std::cout << "1header: " << builder.checkBuffer() << std::endl;
// 	builder.addMessage("Connection: keep-alive\r\n");
// 	std::cout << "2header: " << builder.checkBuffer() << std::endl;
// 	builder.addMessage("Content-Lenght: "+ SSTR(message.length()) +"\r\n");
// 	std::cout << "3header: " << builder.checkBuffer() << std::endl;
// 	builder.addMessage("\r\n");
// 	std::cout << "start body: " << builder.checkBuffer() << std::endl;
// 	builder.addMessage(message);
// 	std::cout << "body: " << builder.checkBuffer() << std::endl;

// 	builder.addMessage("                              ");

// 	std::cout << "body: " << builder.parseRequests() << std::endl;

// 	builder.addMessage("                              ");
// 	std::cout << "empty: " << builder.checkBuffer() << std::endl;
// 	builder.addMessage("     GET /index HTTP/1.1\r\n");
// 	std::cout << "start-line: " << builder.checkBuffer() << std::endl;
// 	builder.addMessage("Host: code.tutsplus.com\r\n");
// 	std::cout << "1header: " << builder.checkBuffer() << std::endl;
// 	builder.addMessage("Connection: kip-alive\r\n");
// 	std::cout << "2header: " << builder.checkBuffer() << std::endl;
// 	builder.addMessage("Content-Lenght: "+ SSTR(message.length()) +"\r\n");
// 	std::cout << "3header: " << builder.checkBuffer() << std::endl;
// 	builder.addMessage("\r\n");
// 	std::cout << "start body: " << builder.checkBuffer() << std::endl;
// 	builder.addMessage(message);
// 	std::cout << "body: " << builder.checkBuffer() << std::endl;

// 	std::cout << "body: " << builder.parseRequests() << std::endl;

// 	std::cout << "body: " << builder.checkBuffer() << std::endl;

// 	Webserv::Http::HttpRequestBuilder::request_list::const_iterator it = builder.getAllRequests().begin();

// 	while (it != builder.getAllRequests().end()) {
// 		std::cout << it->toString() << std::endl;
// 		it++;
// 	}

// 	return 0;
// }

	// 	std::cout << "body: " << builder.checkBuffer() << std::endl;
	// }
// 	{
// 		std::string message = "GET / HTTP/1.1\r\nHost: 127.0.0.1:8080\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\nsec-ch-ua: \" Not A;Brand\";v=\"99\", \"Chromium\";v=\"96\", \"Google Chrome\";v=\"96\"\r\nsec-ch-ua-mobile: ?0\r\nsec-ch-ua-platform: \"macOS\"\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.110 Safari/537.36\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\nSec-Fetch-Site: none\r\nSec-Fetch-Mode: navigate\r\nSec-Fetch-User: ?1\r\nSec-Fetch-Dest: document\r\nAccept-Encoding: gzip, deflate, br\r\nAccept-Language: fr-FR,fr;q=0.9,en-US;q=0.8,en;q=0.7\r\n\r\n";
// 		builder.addMessage(message);

// 		std::cout << "body: " << builder.checkBuffer() << std::endl;

// 		std::cout << "body: " << builder.parseRequests() << std::endl;

// 		std::cout << "body: " << builder.checkBuffer() << std::endl;
// 	}

// 	Webserv::Http::HttpRequestBuilder::request_list::const_iterator it = builder.getAllRequests().begin();

// 	while (it != builder.getAllRequests().end()) {
// 		std::cout << "----------------" << std::endl;
// 		std::cout << it->toString() << std::endl;
// 		std::cout << "----------------" << std::endl;
// 		it++;
// 	}

// 	return 0;
// }

