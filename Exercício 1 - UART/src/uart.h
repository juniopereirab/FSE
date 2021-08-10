#ifndef UART_H
#define UART_H

int initUart();
void writeUart(int uart0_filestream, unsigned char tx_buffer[20], unsigned char *p_tx_buffer);
void readUart(int uart0_filestream);

#endif