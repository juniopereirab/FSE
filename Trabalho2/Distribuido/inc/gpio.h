#ifndef GPIO_H
#define GPIO_H

#include <wiringPi.h>

#define LS_T01 7
#define LS_T02 0
#define LC_T 2
#define AC_T 11
#define SP_T 25
#define SF_T 4
#define SJ_T01 13
#define SJ_T02 14
#define SPo_T 12
#define SC_IN 23
#define SC_OUT 24
#define ASP 27

#define LS_101 3
#define LS_102 6
#define LC_1 10
#define AC_1 26
#define SP_1 1
#define SF_1 5
#define SJ_101 21
#define SJ_102 22

void gpioSetup();
void* gpioHandler();
void* peopleQuantityHandler();
void setDeviceState(int device, int state);
void turnDevicesOff();

#endif
