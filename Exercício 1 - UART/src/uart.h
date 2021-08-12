#ifndef UART_H
#define UART_H

int initUart();
void requestUartInteger(int uart0_filestream);
void requestUartFloat(int uart0_filestream);
void requestUartString(int uart0_filestream);
void sendUartInteger(int uart0_filestream);
void sendUartFloat(int uart0_filestream);
void sendUartString(int uart0_filestream);

#endif