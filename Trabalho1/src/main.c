#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "uart.h"
#include "bme280.h"
#include "display.h"
#include "pid.h"
#include "gpio.h"
#include "temperature.h"

#define TRUE 1
#define FALSE 0

int terminal = FALSE;
int uart_filestream, key_gpio = 1;
struct bme280_dev bme_connection;

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

void menu() {
    int selection, option = 0;
    printf("Escolha uma forma para controlar o programa\n1) Terminal\n2) Chave\n");
    scanf("%d", &selection);
    if(selection == 1){
        system("clear");
        printf("Como deseja que sua rotina funcione?\n1) Controle ON / OFF\n2) Controle PID\n");
        scanf("%d", &option);
        while(option != 1 || option != 2){
            printf("Opção inválida. Escolha entre as disponíveis:\n\t1) Controle ON / OFF\n\t2) Controle PID\n");
            scanf("%d", &option);
        }
        option--;
        terminal = TRUE;
    }
    else if(selection == 2){
        getchar();
        system("clear");
        requestToUart(uart_filestream, GET_KEY_VALUE);
        option = readFromUart(uart_filestream, GET_KEY_VALUE).int_value;
        printf("O valor inicial da chave é: %d\n", option);
        printf("\nQuando a chave for igual a 0:\n");
        printf("Iniciar rotina ON/OFF\n");
        printf("\nQuando a chave for igual a 1:\n");
        printf("Iniciar rotina PID\n");
        printf("\nPrecione Enter para continuar\n");
        char enter = 0;
        while (enter != '\r' && enter != '\n') {
            enter = getchar();
        }
    }
    else {
        system("clear");
        printf("Opção inválida!");
        menu();
    }

    if(option){
        //Rotina PID
    }
    else {
        //Rotina ON / OFF
    }
}

void start_program(){
    wiringPiSetup();
    turnResistanceOff();
    turnFanOff();
    connectDisplay();
    bme_connection = connectBme();
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
    menu();
    return 0;
}