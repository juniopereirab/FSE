#include "display.h"

int fd;

void printDisplay(float TI,float TR, float TE)   {

  if (wiringPiSetup () == -1) exit (1);

  fd = wiringPiI2CSetup(I2C_ADDR);

  connectDisplay();
  displayLoc(LINE1);
  typeln("TI");
  typeFloat(TI);
  typeln(" TR");
  typeFloat(TR);
  displayLoc(LINE2);
  typeln("TE");
  typeFloat(TE);
}

void typeFloat(float myFloat)   {
  char buffer[20];
  sprintf(buffer, "%4.2f",  myFloat);
  typeln(buffer);
}


void ClrDisplay(void)   {
  displayByte(0x01, LCD_CMD);
  displayByte(0x02, LCD_CMD);
}

void displayLoc(int line)   {
  displayByte(line, LCD_CMD);
}

void typeln(const char *s)   {

  while ( *s ) displayByte(*(s++), LCD_CHR);

}

void displayByte(int bits, int mode)   {
  int bits_high;
  int bits_low;
  bits_high = mode | (bits & 0xF0) | LCD_BACKLIGHT ;
  bits_low = mode | ((bits << 4) & 0xF0) | LCD_BACKLIGHT ;

  wiringPiI2CReadReg8(fd, bits_high);
  displayToggleEnable(bits_high);

  wiringPiI2CReadReg8(fd, bits_low);
  displayToggleEnable(bits_low);
}

void displayToggleEnable(int bits)   {
  delayMicroseconds(500);
  wiringPiI2CReadReg8(fd, (bits | ENABLE));
  delayMicroseconds(500);
  wiringPiI2CReadReg8(fd, (bits & ~ENABLE));
  delayMicroseconds(500);
}


void connectDisplay()   {
  displayByte(0x33, LCD_CMD);
  displayByte(0x32, LCD_CMD);
  displayByte(0x06, LCD_CMD);
  displayByte(0x0C, LCD_CMD);
  displayByte(0x28, LCD_CMD);
  displayByte(0x01, LCD_CMD);
  delayMicroseconds(500);
}
