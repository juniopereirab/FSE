#ifndef GPIO_H_
#define GPIO_H_

void pwmControl(int intensity_signal);
void turnResistanceOff();
void turnResistanceOn(int new_resistor_value);
void turnFanOff();
void turnFanOn(int new_fan_value);

#endif /* GPIO_H_ */