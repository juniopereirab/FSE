#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "mainMenu.h"
#include "tcpClient.h"
#include "gpio.h"

#define SLEEP_TIME 1000000

Data appData;

void appInit() {
  DHT22 dht22;
  dht22.temperature = 0.0;
  dht22.humidity = 0.0;
  appData.dht22GroundFloor = dht22;
  appData.dht22FirstFloor = dht22;
}

Data currentData() {
  return appData;
}

void* appHandler() {
  while (1) {
    DHT22 dht22GroundFloor = requestData(0);
    DHT22 dht22FirstFloor = requestData(1);

    if (dht22GroundFloor.temperature > 0 && dht22GroundFloor.humidity > 0) {
      appData.dht22GroundFloor = dht22GroundFloor;
    }

    if (dht22FirstFloor.temperature > 0 && dht22FirstFloor.humidity > 0) {
      appData.dht22FirstFloor = dht22FirstFloor;
    }

    DevicesOut devOut;
    devOut = recoverDevicesOutData();
    appData.devOut = devOut;

    printData(appData);
    usleep(SLEEP_TIME);
  }

  return NULL;
}
