#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "gpio.h"

void killHandler() {
  turnDevicesOff();
  exit(1);
}

void killSetup() {
  signal(SIGINT, killHandler);
}
