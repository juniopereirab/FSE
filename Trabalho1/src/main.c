#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "uart.h"

#define TRUE 1
#define FALSE 0

int terminal = FALSE;
int uart_filestream, key_gpio = 1;

void exit_program(){
    system("clear");
    printf("Programa encerrado!\n");
    closeUart(uart_filestream);
    exit(0);
}

void change_routine(){
    if(terminal){
        if(key_gpio == 1){
            key_gpio = 0;
        }
        else {
            key_gpio = 1;
        }
    }
}

void start_program(){
    uart_filestream = initUart();
    system("clear");
}

int main() {
    start_program();
    signal(SIGINT, exit_program);
    signal(SIGQUIT, change_routine);
    printf(
        "\n"
        "█████████ Damaso Junio Pereira Brasileo █████████\n"
        "████████████████ Projeto 1 - FSE ████████████████\n\n"
    );

    return 0;
}