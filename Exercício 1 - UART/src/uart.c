#include <stdio.h>
#include <string.h>
#include <unistd.h>         //Used for UART
#include <fcntl.h>          //Used for UART
#include <termios.h>        //Used for UART
#include "uart.h"

int initUart () {
    int uart0_filestream = -1;

    uart0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);      //Open in non blocking read/write mode
    if (uart0_filestream == -1)
    {
        printf("Erro - Não foi possível iniciar a UART.\n");
    }
    else
    {
        printf("UART inicializada!\n");
    }    
    struct termios options;
    tcgetattr(uart0_filestream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;     //<Set baud rate
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);

    return uart0_filestream;
}


void readUart(int uart0_filestream, char *type) {
    // Read up to 255 characters from the port if they are there
    int rx_length;
    int received_int;
    float received_float;
    char received[256];
    
    if(strcmp(type, "integer") == 0){
        rx_length = read(uart0_filestream, &received_int, 4);
        
    }
    else if(strcmp(type, "float") == 0){
        rx_length = read(uart0_filestream, &received_float, 4);
    }
    else if(strcmp(type, "string") == 0){
        rx_length = read(uart0_filestream, received, 256);
        received[rx_length] = '\0';
    }

    if (rx_length < 0)
    {
        printf("Erro na leitura.\n"); //An error occured (will occur if there are no bytes)
    }
    else if (rx_length == 0)
    {
        printf("Nenhum dado disponível.\n"); //No data waiting
    }
    else
    {
        if(strcmp(type, "integer") == 0){
            printf("Valor Inteiro Recebido: %d\n", received_int);
            
        }
        else if(strcmp(type, "float") == 0){
            printf("Valor Float Recebido: %f\n", received_float);

        }
        else if(strcmp(type, "string") == 0){
            printf("String Recebida: %s\n", received);
        }
    }
}

void writeUart(int uart0_filestream, char * payload, int length){
    if (uart0_filestream != -1)
    {
        printf("Escrevendo caracteres na UART ...");
        int count = write(uart0_filestream, payload, length);
        if (count < 0)
        {
            printf("UART TX error\n");
        }
        else
        {
            printf("escrito.\n");
        }
    }

    sleep(1);
}

void requestUartInteger(int uart0_filestream){

    char payload[5] = {0xA1, 1, 4, 3, 8};

    printf("Buffers de memória criados!\n");

    writeUart(uart0_filestream, payload, 5);
    
    readUart(uart0_filestream, "integer");
}

void requestUartFloat(int uart0_filestream){
    
    char payload[5] = {0xA2, 1, 4, 3, 8};

    printf("Buffers de memória criados!\n");

    writeUart(uart0_filestream, payload, 5);

    readUart(uart0_filestream, "float");
}

void requestUartString(int uart0_filestream){

    char payload[5] = {0xA3, 1, 4, 3, 8};
    
    printf("Buffers de memória criados!\n");

    writeUart(uart0_filestream, payload, 5);

    readUart(uart0_filestream, "string");
}

void sendUartInteger(int uart0_filestream){
    char payload[9] = {0xB1, 0, 0, 5, 0, 1, 4, 3, 8};

    printf("Buffers de memória criados!\n");

    writeUart(uart0_filestream, payload, 9);
    readUart(uart0_filestream, "integer");
}

void sendUartFloat(int uart0_filestream){
    char payload[9] = {0xB2, 0, 0, 50, 50, 1, 4, 3, 8};

    printf("Buffers de memória criados!\n");

    writeUart(uart0_filestream, payload, 9);
    readUart(uart0_filestream, "float");
}

void sendUartString(int uart0_filestream){
    
    char payload[9] = {0xB3, 3, 'o', 'l', 'a', 1, 4, 3, 8};
    printf("Buffers de memória criados!\n");

    writeUart(uart0_filestream, payload, 9);
    readUart(uart0_filestream, "string");
}