#include <stdio.h>
#include <unistd.h>         //Used for UART
#include <fcntl.h>          //Used for UART
#include <termios.h>        //Used for UART

#include "uart.h"


int main(int argc, const char * argv[]) {

    int uart0_filestream = initUart();

    unsigned char tx_buffer[20];
    unsigned char *p_tx_buffer;
    
    p_tx_buffer = &tx_buffer[0];
    *p_tx_buffer++ = '163';
    *p_tx_buffer++ = '3';
    *p_tx_buffer++ = 'O';
    *p_tx_buffer++ = 'l';
    *p_tx_buffer++ = 'a';
    *p_tx_buffer++ = '1';
    *p_tx_buffer++ = '4';
    *p_tx_buffer++ = '3';
    *p_tx_buffer++ = '8';
    

    printf("Buffers de memória criados!\n");
    
    writeUart(uart0_filestream, tx_buffer, p_tx_buffer);

    //----- CHECK FOR ANY RX BYTES -----
    if (uart0_filestream != -1){
        readUart(uart0_filestream);
    }
    close(uart0_filestream);
   return 0;
}
