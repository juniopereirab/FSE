#include "gpio.h"

#ifndef TCPCLIENT_H_
#define TCPCLIENT_H_

int sendCommand(int device, int state, int floor);
DHT22 requestData(int floor);

#endif
