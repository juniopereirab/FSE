#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "tcpServer.h"
#include "socketKiller.h"
#include "gpio.h"
#include "dht22.h"

int main() {
    gpioSetup();
    killSetup();
    dht22_setup();
    turnDevicesOff();

    pthread_t tid[4];

    pthread_create(&tid[0], NULL, (void*)gpioHandler, (void*) NULL);
    pthread_create(&tid[1], NULL, (void*)peopleQuantityHandler, (void*) NULL);
    pthread_create(&tid[2], NULL, (void*)serverHandler, (void*) 0);
    pthread_create(&tid[3], NULL, (void*)serverHandler, (void*) 1);

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_join(tid[2], NULL);
    pthread_join(tid[3], NULL);
    
    return 0;
}