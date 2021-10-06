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

#define SERVER_DISTRIBUTED_IP "192.168.0.52"
#define SERVER_DISTRIBUTED_1_PORT 31438
#define SERVER_DISTRIBUTED_2_PORT 31638

int sendCommand(int device, int state, int floor) {
    struct sockaddr_in server_addr;
    int clientId = socket(AF_INET, SOCK_STREAM, 0);

    if (clientId < 0) {
        char *message = "Erro interno no Socket!";
        quitHandler(message);
    }

    memset(&server_addr, '0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_DISTRIBUTED_IP);
    server_addr.sin_port = htons(floor == 0 ? SERVER_DISTRIBUTED_1_PORT : SERVER_DISTRIBUTED_2_PORT);

    int connection = connect(clientId, (struct sockaddr*) &server_addr, sizeof(server_addr));
    if (connection < 0) {
        char *message = "Erro de conexão!";
        quitHandler(message);
    }

    char buf[16];
    snprintf(buf, 16, "%d %d %d", 1, device, state);
    int size = strlen(buf);
    int sender = send(clientId, buf, size, 0);
    if (sender != size) {
        char *message = "Erro ao enviar!";
        quitHandler(message);
    }

    char buffer[2];
    int size_rec = recv(clientId, buffer, 1, 0);
    if (size_rec < 0) {
        char *message = "Erro ao receber dados!";
        quitHandler(message);
    }

    int res;
    sscanf(buffer, "%d", &res);

    close(clientId);

    return res;
    }

    DHT22 requestData(int floor) {
    struct sockaddr_in server_addr;

    DHT22 dht22;
    dht22.temperature = 0.0;
    dht22.humidity = 0.0;

    int clientId = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    if (clientId < 0) {
        printf("clientId < 0\n");
        return dht22;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_DISTRIBUTED_IP);
    server_addr.sin_port = htons(floor == 0 ? SERVER_DISTRIBUTED_1_PORT : SERVER_DISTRIBUTED_2_PORT);

    int connection = connect(clientId, (struct sockaddr*) &server_addr, sizeof(server_addr));
    if (connection < 0) {
        return dht22;
    }

    char buf[6];
    snprintf(buf, 6, "%d %d", 2, floor);
    int size = strlen(buf);
    int sender = send(clientId, buf, size, 0);
    if (sender != size) {
        close(clientId);
        return dht22;
    }

    char buffer[16];
    int total_bytes_rec = 0;
    int size_rec;

    while (total_bytes_rec < size) {
        if ((size_rec = recv(clientId, buffer, 16 - 1, 0)) <= 0) {
            printf("Erro ao receber os bytes esperados.\n");
            close(clientId);
            return dht22;
        }
        total_bytes_rec += size_rec;
        buffer[size_rec] = '\0';
    }
        
    int command;
    float temperature = 0.0;
    float humidity = 0.0;

    sscanf(buffer, "%d", &command);

    if (command == 2) {
        sscanf(buffer, "%d %f %f", &command, &temperature, &humidity);
        dht22.temperature = temperature;
        dht22.humidity = humidity;
    }

    close(clientId);

    return dht22;
}
