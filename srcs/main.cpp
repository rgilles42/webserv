/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 15:47:40 by ppaglier          #+#    #+#             */
/*   Updated: 2021/10/20 16:14:35 by ppaglier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.hpp"

#if defined (_WIN32) || defined (_WIN64)
	#include <winsock2.h>
	typedef int socklen_t;
#elif defined (__unix__)
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <unistd.h>
	#define INVALID_SOCKET -1
	#define SOCKET_ERROR -1
	#define closesocket(s) close(s)
	typedef int SOCKET;
	typedef struct sockaddr_in SOCKADDR_IN;
	typedef struct sockaddr SOCKADDR;
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string>

#define PORT 80

int main(void) {
	int erreur = 0;
	#if defined (_WIN32) || defined (_WIN64)
		WSADATA WSAData;
		erreur = WSAStartup(MAKEWORD(2,2), &WSAData);
	#endif

	SOCKET sock;
	SOCKADDR_IN sin;
	SOCKET csock;
	SOCKADDR_IN csin;
	char buffer[32] = "";
	socklen_t recsize = sizeof(csin);
	int sock_err;

	/* Si les sockets Windows fonctionnent */
	if (!erreur) {
		sock = socket(AF_INET, SOCK_STREAM, 0);

		/* Si la socket est valide */
		if (sock != INVALID_SOCKET) {
			/* Configuration */
			sin.sin_addr.s_addr    = htonl(INADDR_ANY);   /* Adresse IP automatique */
			sin.sin_family         = AF_INET;             /* Protocole familial (IP) */
			sin.sin_port           = htons(PORT);         /* Listage du port */

			printf("La socket %d est maintenant ouverte en mode TCP/IP (%d:%d)\n", sock, ntohl(sin.sin_addr.s_addr), ntohs(sin.sin_port));

			sock_err = bind(sock, (SOCKADDR*)&sin, sizeof(sin));

			/* Si la socket fonctionne */
			if (sock_err != SOCKET_ERROR) {
				/* Démarrage du listage (mode server) */
				sock_err = listen(sock, 5);
				printf("Listage du port %d...\n", ntohs(sin.sin_port));

				/* Si la socket fonctionne */
				if (sock_err != SOCKET_ERROR) {
					/* Attente pendant laquelle le client se connecte */
					printf("Patientez pendant que le client se connecte sur le port %d...\n", ntohs(sin.sin_port));

					csock = accept(sock, (SOCKADDR*)&csin, &recsize);
					printf("Un client se connecte avec la socket %d de %s:%d\n", csock, inet_ntoa(csin.sin_addr), htons(csin.sin_port));

					sock_err = recv(csock, buffer, 32, 0);

					if (sock_err != SOCKET_ERROR) {
						printf("Chaine reçu : %s\n", buffer);
					} else {
						printf("Erreur de récéption\n");
					}

					sock_err = send(csock, buffer, 32, 0);

					if (sock_err != SOCKET_ERROR) {
						printf("Chaine envoyée : %s\n", buffer);
					} else {
						printf("Erreur de transmission\n");
					}

					/* Il ne faut pas oublier de fermer la connexion (fermée dans les deux sens) */
					shutdown(csock, 2);
				} else {
					perror("listen");
				}
			} else {
				perror("bind");
			}

			/* Fermeture de la socket */
			printf("Fermeture de la socket...\n");
			closesocket(sock);
			printf("Fermeture du serveur terminee\n");
		}

		#if defined (_WIN32) || defined (_WIN64)
			WSACleanup();
		#endif
	}

	return EXIT_SUCCESS;
}