#ifndef TEMPERATURE_H_
#define TEMPERATURE_H_

#include "bme280.h"

struct bme280_dev connectBme();
void userDelayMs(uint32_t period);
void printSensorData(struct bme280_data *comp_data);
int8_t userI2cRead(uint8_t id, uint8_t reg_addr, uint8_t *data, uint16_t len);
int8_t userI2cWrite(uint8_t id, uint8_t reg_addr, uint8_t *data,
                      uint16_t len);
float getCurrentTemperature(struct bme280_dev *dev);

#endif /* TEMPERATURE_H_ */