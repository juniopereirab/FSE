#ifndef DHT22_H_
#define DHT22_H_

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// CONSTANTS 
#define MAX_TIMINGS	85
#define DEBUG 1
#define WAIT_TIME 2000

int dht22_setup();
int dht22_read_data(float *temperature, float *humidity, int floor);

#endif
