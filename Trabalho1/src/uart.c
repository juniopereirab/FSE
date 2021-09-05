#include <fcntl.h>  //Used for UART
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>  //Used for UART
#include <unistd.h>   //Used for UART

#include "uart_defs.h"

int initUart(){
    int uart_filestream = -1;
    char uart_path[] = "/dev/serial0";
    uart_filestream = open(uart_path, O_RDWR | O_NOCTTY | O_NDELAY);
    if(uart_filestream == -1){
        printf("Não foi possível iniciar a Uart.\n");
    }
    else {
        printf("UART inicializado.\n");
    }
    struct termios options;
    tcgetattr(uart_filestream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart_filestream, TCIFLUSH);
    tcsetattr(uart_filestream, TCSANOW, &options);
    return uart_filestream;
}

void requestToUart(int uart_filestream, unsigned char code){
    return;
}

void sendToUart(int uart_filestream, int value){
    return;
}

Number_type readFromUart(int uart_filestream, unsigned char code){
    Number_type number = {-1, -1.0};

    return number; 
}

void closeUart(int uart_filestream){
    printf("Conexão UART finalizada\n");
    close(uart_filestream);
}