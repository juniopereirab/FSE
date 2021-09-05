#include <gpio.h>
#include <softPwm.h>
#include <wiringPi.h>

#define RESISTOR_PIN 4
#define FAN_PIN 5

void turnResistanceOn(int new_resistor_value) {
  pinMode(RESISTOR_PIN, OUTPUT);
  softPwmCreate(RESISTOR_PIN, 0, 100);
  softPwmWrite(RESISTOR_PIN, new_resistor_value);
}

void turnResistanceOff() {
  pinMode(RESISTOR_PIN, OUTPUT);
  softPwmCreate(RESISTOR_PIN, 0, 100);
  softPwmWrite(RESISTOR_PIN, 0);
}

void turnFanOn(int new_fan_value) {
  pinMode(FAN_PIN, OUTPUT);
  softPwmCreate(FAN_PIN, 0, 100);
  softPwmWrite(FAN_PIN, new_fan_value);
}

void turnFanOff() {
  pinMode(FAN_PIN, OUTPUT);
  softPwmCreate(FAN_PIN, 0, 100);
  softPwmWrite(FAN_PIN, 0);
}

void pwmControl(int intensity_signal) {
  if (intensity_signal > 0) {
    turnResistanceOn(intensity_signal);
    turnFanOff();
  } else {
    if (intensity_signal <= -40)
      turnFanOn(intensity_signal * -1);
    else
      turnFanOff();
    turnResistanceOff();
  }
}
