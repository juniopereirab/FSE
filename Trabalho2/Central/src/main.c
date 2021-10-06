#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "application.h"
#include "socketKiller.h"
#include "mainMenu.h"
#include "gpio.h"
#include "tcpServer.h"
#include "csv.h"

int main() {
  quit_setup();
  csv_setup();
  app_init();

  pthread_t tid[3];

  pthread_create(&tid[0], NULL, (void *)appHandler, (void *)NULL);
  pthread_create(&tid[1], NULL, (void *)menuHandler, (void *)NULL);
  pthread_create(&tid[2], NULL, (void *)recvMessage, (void *)NULL);

  pthread_join(tid[0], NULL);
  pthread_join(tid[1], NULL);
  pthread_join(tid[2], NULL);

  return 0;
}
