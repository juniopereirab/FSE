#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#define I2C_ADDR 0x27

#define LCD_CHR 1
#define LCD_CMD 0

#define LINE1 0x80
#define LINE2 0xC0

#define LCD_BACKLIGHT 0x08

#define ENABLE 0b00000100

void connectDisplay(void);
void printDisplay(float tempI, float tempR, float tempE);
void displayByte(int bits, int mode);
void displayToggleEnable(int bits);

// added by Lewis
void typeFloat(float myFloat);
void displayLoc(int line);
void typeln(const char *s);

#endif