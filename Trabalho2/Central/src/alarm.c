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
  DevicesOut dev_out = recoverDevicesOutData();

  // Alarm active
  if (dev_out.alarm == 1) {
    dev_out.alarmPlaying = 1;
    playing = 1;
    pthread_t alarm;
    pthread_create(&alarm, NULL, (void *)play, (void *)NULL);
    storeDevicesOutUpdate(dev_out);
  }
}

void alarmOff() {
  playing = 0;
}
