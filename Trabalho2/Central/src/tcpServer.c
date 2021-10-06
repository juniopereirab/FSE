#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include "gpio.h"
#include "alarm.h"
#include "socketKiller.h"

#define SERVER_CENTRAL_PORT 31738

void processClientTcp(int clientId) {
    char buffer[6];
    int size = recv(clientId, buffer, 16, 0);
    if (size < 0) {
        char *message = "Erro ao receber mensagens!!";
        quitHandler(message);
    }

    while (size > 0) {
        int command;
        sscanf(buffer, "%d", &command);

        devicesInHandler(command);

        int sender = send(clientId, buffer, size, 0);
        if (sender != size) {
        char *message = "Erro ao enviar mensagens!!";
        quitHandler(message);
        }
        size = recv(clientId, buffer, 6, 0);
        if (size < 0) {
        char *message = "Erro ao receber mensagens!!";
        quitHandler(message);
        }
    }
}

void* recvMessage() {
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

	int serverId = socket(AF_INET, SOCK_STREAM, 0);

    if (serverId < 0) {
        char *message = "ERRO NO SOCKET DO SERVIDOR!";
        quitHandler(message);
    }

	memset(&server_addr, '0', sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(SERVER_CENTRAL_PORT);

    if (bind(serverId, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
        char *message = "ERRO NO BIND!";
        quitHandler(message);
    }
    
    int listener = listen(serverId, 10);
	if (listener < 0) {
		char *message = "Erro no listener!!";
		quitHandler(message);
	}

    while (1) {
        unsigned int clientSize = sizeof(client_addr);
        int clientId = accept(serverId, (struct sockaddr*) &client_addr, &clientSize);

        if (clientId < 0) {
            char *message = "Erro ao aceitar a conexão!!";
            quitHandler(message);
        }

        processClientTcp(clientId);
            
        close(clientId);
    }

	close(serverId);
}
