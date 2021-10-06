#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "menu.h"
#include "tcpClient.h"
#include "gpio.h"

#define SLEEP_TIME 1000000

Data appData;

void appInit() {
  DHT22 dht22;
  dht22.temperature = 0.0;
  dht22.humidity = 0.0;
  appData.dht22_ground_floor = dht22;
  appData.dht22_first_floor = dht22;
}

Data currentData() {
  return appData;
}

void* appHandler() {
  while (1) {
    DHT22 dht22_ground_floor = requestData(0);
    DHT22 dht22_first_floor = requestData(1);

    if (dht22_ground_floor.temperature > 0 && dht22_ground_floor.humidity > 0) {
      appData.dht22_ground_floor = dht22_ground_floor;
    }

    if (dht22_first_floor.temperature > 0 && dht22_first_floor.humidity > 0) {
      appData.dht22_first_floor = dht22_first_floor;
    }

    DevicesOut devOut;
    devOut = recoverDevicesOutData();
    appData.devOut = devOut;

    print_data(appData);
    usleep(TIME_SLEEP);
  }

  return NULL;
}
