#ifndef UART_H
#define UART_H

int initUart();
void requestUartInteger(int uart0_filestream);
void requestUartFloat(int uart0_filestream);
void requestUartString(int uart0_filestream);

#endif