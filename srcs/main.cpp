/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgilles <rgilles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 15:47:40 by ppaglier          #+#    #+#             */
/*   Updated: 2021/11/22 14:42:18 by rgilles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.hpp"

int main(void)
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
