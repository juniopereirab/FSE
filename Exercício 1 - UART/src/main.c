#include <stdio.h>
#include <unistd.h>         //Used for UART
#include <fcntl.h>          //Used for UART
#include <termios.h>        //Used for UART

#include "uart.h"


int main(int argc, const char * argv[]) {

    int uart0_filestream = initUart();
 
    requestUartInteger(uart0_filestream);
    requestUartFloat(uart0_filestream);
    requestUartString(uart0_filestream);
    
    close(uart0_filestream);
   return 0;
}
