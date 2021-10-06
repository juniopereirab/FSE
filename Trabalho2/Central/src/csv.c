#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "csv.h"

#define BUFFER_SIZE 1000

char buffer[BUFFER_SIZE];

void timeGenerate() {
  time_t currentTime;
  struct tm* timeInfo;

  time(&currentTime);
  timeInfo = localtime(&currentTime);

  strftime(buffer, BUFFER_SIZE, "%Y-%m-%d %H:%M:%S", timeInfo);
}

void csvSetup() {
  FILE *file = fopen("logs/data.csv", "w");
  fprintf(file, "DATETIME, DEVICE, STATE\n");
  fclose(file);
}

void writeData(Command command) {
  timeGenerate();

  FILE *file = fopen("logs/data.csv", "a");
  char *state = command.state == 1 ? "ON" : "OFF";

  fprintf(
    file,
    "%s,%s,%s\n",
    buffer,
    command.device,
    state
  );

  fclose(file);
}
