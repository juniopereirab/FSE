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

void exitProgram(){
    system("clear");
    printf("Programa encerrado!\n");
    closeUart(uart_filestream);
    exit(0);
}

void changeRoutine(){
    if(terminal){
        if(key_gpio == 1){
            key_gpio = 0;
        }
        else {
            key_gpio = 1;
        }
    }
}

void onOff(int key){
    system("clear");
    float hys, TI, TR, TE;
    int value = 0;
    printf("\n██████████████████ Método ON / OFF ██████████████████\n");
    printf("\nDefina uma histerese: ");
    scanf("%f", &hys);
    do{
        requestToUart(uart_filestream, GET_INTERNAL_TEMP);
        TI = readFromUart(uart_filestream, GET_INTERNAL_TEMP).float_value;
        requestToUart(uart_filestream, GET_POTENTIOMETER);
        TR = readFromUart(uart_filestream, GET_POTENTIOMETER).float_value;
        TE = getCurrentTemperature(&bme_connection);
        printf("\tTI: %.2f⁰C - TR: %.2f⁰C - TE: %.2f⁰C\n", TI, TR, TE);
        printDisplay(TI, TR, TE);

        if(TR - hys >= TI){
            turnResistanceOn(100);
            turnFanOff();
            value = 100;
        }
        else if(TR + hys <= TI){
            turnFanOn(100);
            turnResistanceOff();
            value = -100;
        }

        if(!terminal){
            requestToUart(uart_filestream, GET_KEY_VALUE);
            key_gpio = readFromUart(uart_filestream, GET_KEY_VALUE).int_value;
        }

        sendToUart(uart_filestream, value);
    } while (key_gpio == key);
    printf("██████████████████████████████████████████████████████\n");
    PID(key_gpio);
}

void PID(int key){
    system("clear");
    float hysteresis, TI, TR, TE;
    printf("\n██████████████████ Método PID ██████████████████\n");
    pidSetupConstants(5, 1, 5);
    do{
        requestToUart(uart_filestream, GET_INTERNAL_TEMP);
        TI = readFromUart(uart_filestream, GET_INTERNAL_TEMP).float_value;

        double value = pidControl(TI);
        pwmControl(value);

        requestToUart(uart_filestream, GET_POTENTIOMETER);
        TR = readFromUart(uart_filestream, GET_POTENTIOMETER).float_value;

        pidUpdateReference(TR);

        TE = getCurrentTemperature(&bme_connection);
        printf("\tTI: %.2f⁰C - TR: %.2f⁰C - TE: %.2f⁰C\n", TI, TR, TE);
        printDisplay(TI, TR, TE);

        if(!terminal){
            requestToUart(uart_filestream, GET_KEY_VALUE);
            key_gpio = readFromUart(uart_filestream, GET_KEY_VALUE).int_value;
        }

        sendToUart(uart_filestream, value);
    } while(key_gpio == key);
    printf("██████████████████████████████████████████████████████\n");
    onOff(key_gpio);
}

void menu() {
    int selection, option = 0;
    printf("Escolha uma forma para controlar o programa\n1) Terminal\n2) Chave\n");
    scanf("%d", &selection);
    if(selection == 1){
        system("clear");
        printf("Como deseja que sua rotina funcione?\n1) Controle ON / OFF\n2) Controle PID\n");
        scanf("%d", &option);
        option--;
        terminal = TRUE;
        key_gpio = option;
    }
    else if(selection == 2){
        getchar();
        system("clear");
        requestToUart(uart_filestream, GET_KEY_VALUE);
        option = readFromUart(uart_filestream, GET_KEY_VALUE).int_value;
        printf("O valor inicial da chave é: %d\n", option);
        printf("\nQuando a chave for igual a 0:\n");
        printf("█████████Iniciar rotina ON/OFF█████████\n");
        printf("\nQuando a chave for igual a 1:\n");
        printf("█████████Iniciar rotina PID█████████\n");
        printf("\nPrecione Enter para continuar\n");
        getchar();
    }
    else {
        system("clear");
        printf("Opção inválida!");
        menu();
    }

    if(option){
        PID(option);
    }
    else {
        onOff(option);
    }
}

void start_program(){
    wiringPiSetup();
    turnResistanceOff();
    turnFanOff();
    connectDisplay();
    bme_connection = connectBme();
    uart_filestream = initUart();
}

int main() {
    start_program();
    signal(SIGINT, exitProgram);
    signal(SIGQUIT, changeRoutine);
    printf(
        "\n"
        "█████████ Damaso Junio Pereira Brasileo █████████\n"
        "████████████████ Projeto 1 - FSE ████████████████\n\n"
    );
    menu();
    return 0;
}