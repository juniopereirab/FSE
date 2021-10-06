#ifndef CSV_H_
#define CSV_H_

typedef struct {
  char* device;
  int state;
} Command;

void csvSetup();
void writeData(Command command);

#endif
