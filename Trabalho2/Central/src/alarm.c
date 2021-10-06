#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "alarm.h"
#include "gpio.h"

int playing = 0;

void* play() {
  while (playing == 1) {
    system("cd ..; omxplayer central_beep.mp3 > /dev/null");
  }

  return NULL;
}

void alarmHandler() {
  DevicesOut dev_out = recover_devices_out_data();

  // Alarm active
  if (dev_out.alarm == 1) {
    dev_out.alarm_playing = 1;
    playing = 1;
    pthread_t alarm;
    pthread_create(&alarm, NULL, (void *)play, (void *)NULL);
    store_devices_out_update(dev_out);
  }
}

void alarmOff() {
  playing = 0;
}
