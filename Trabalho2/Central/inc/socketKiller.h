#ifndef SOCKETKILLER_H_
#define SOCKETKILLER_H_

void finish(int signal);
void finishWithError(int signal);
void quitSetup();
void quitHandler(char *message);

#endif
