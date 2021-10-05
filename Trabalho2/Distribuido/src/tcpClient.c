#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "gpio.h"
#include "dht22.h"
#include "socketKiller.h"

#define SERVER_CENTRAL_IP "192.168.0.53"
#define SERVER_CENTRAL_PORT 10032

void sendCommand(int command) {
  struct sockaddr_in server_addr;
  int clientId = socket(AF_INET, SOCK_STREAM, 0);

  if (clientId < 0) {
    printf("Socket Error\n");
    killHandler();
  }

  memset(&server_addr, '0', sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(SERVER_CENTRAL_IP);
  server_addr.sin_port = htons(SERVER_CENTRAL_PORT);

  int connection = connect(clientId, (struct sockaddr *)&server_addr, sizeof(server_addr));
  
  if (connection < 0) {
    printf("Erro ao conectar ao socket!!\n");
    killHandler();
  }

  char buffer[6];

  snprintf(buffer, 6, "%d", command);
  int size = strlen(buffer);
  int sender = send(clientId, buffer, size, 0);
  if (sender != size) {
    printf("Erro ao enviar! Algo inesperado ocorreu!\n");
    killHandler();
  }

  close(clientId);
}
