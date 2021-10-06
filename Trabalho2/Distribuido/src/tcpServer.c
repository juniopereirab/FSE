#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "gpio.h"
#include "dht22.h"
#include "socketKiller.h"

#define SERVER_DISTRIBUTED_1_PORT 31738
#define SERVER_DISTRIBUTED_2_PORT 31838

void processClient(int clientId) {
  char buffer[16];
  int received = recv(clientId, buffer, 16, 0);

  if (received < 0) {
    printf("Erro ao receber mensagem...\n");
    killHandler();
  }

  int command;
  sscanf(buffer, "%d", &command);

  if (command == 1) {
    int device;
    int state;

    sscanf(buffer, "%d %d %d", &command, &device, &state);

    setDeviceState(device, state);

    char buf[2];
    snprintf(buf, 2, "%d", 1);

    int buf_size = strlen(buf);
    int sender = send(clientId, buf, buf_size, 0);
    if (sender != buf_size) {
      printf("Erro ao enviar mensagem...\n");
    }
  }

  if (command == 2) {
    int floor;

    sscanf(buffer, "%d %d", &command, &floor);

    float temperature = 0.0;
    float humidity  = 0.0;

    dht22_read_data(&temperature, &humidity, floor);

    char buf[16];
    snprintf(buf, 16, "%d %4.2f %4.2f", 2, temperature, humidity);

    int buf_size = strlen(buf);
    int sender = send(clientId, buf, buf_size, 0);
    if (sender != buf_size) {
      printf("Erro ao enviar mensagem...\n");
    }
  }
}

void* serverHandler(int floor) {
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    int serverId = socket(AF_INET, SOCK_STREAM, 0);

    if (serverId < 0) {
        printf("ERRO NO SOCKET DO SERVIDOR!\n");
        killHandler();
    }

    memset(&server_addr, '0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(floor == 0 ? SERVER_DISTRIBUTED_1_PORT: SERVER_DISTRIBUTED_2_PORT);

    int binder = bind(serverId, (struct sockaddr*) &server_addr, sizeof(server_addr));
    if (binder < 0) {
        printf("Erro na função de Bind...\n");
        killHandler();
    }
    int listener = listen(serverId, 10);
    if (listener < 0) {
        printf("Erro na função Listen...\n");
        killHandler();
    }

  while (1) {
    unsigned int client_length = sizeof(client_addr);

    int clientId = accept(serverId, (struct sockaddr *)&client_addr, &client_length);
    
    if (clientId < 0) {
        printf("Erro na função Accept\n");
	    killHandler();
    }

    processClient(clientId);

    close(clientId);
  }
  close(serverId);
}
