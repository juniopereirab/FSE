#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <ncurses.h>

void finish(int signal) {
  printf("Finzalização completa . . .\n");
  exit(0);
}

void finishWithError(int signal) {
  printf("Erro interno no socket\n");
  exit(0);
}

void quitSetup() {
  signal(SIGINT, finish);
  signal(SIGPIPE, finishWithError);
}

void quitHandler(char *message) {
  refresh();
  endwin();

  int len = strlen(message);

  if (len > 0) {
    printf("%s\n", message);
  }

  exit(1);
}
