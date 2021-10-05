#include <stdio.h>
#include <unistd.h>
#include "gpio.h"
#include "tcpClient.h"

// Private functions signatures
void checkPresenceGround();
void checkSmokeGround();
void checkWindow1Ground();
void checkWindow2Ground();
void checkDoorGround();
void checkPeopleQuantityGround_IN();
void checkPeopleQuantityGround_OUT();
void checkPresenceFirstFloor();
void checkSmokeFirstFloor();
void checkWindow1FirstFloor();
void checkWindow2FirstFloor();


// Main functions

void gpioSetup() {
  wiringPiSetup();
}

void* gpioHandler() {
  pinMode(SP_T, OUTPUT);
  wiringPiISR(SP_T, INT_EDGE_BOTH, &checkPresenceGround);

  pinMode(SF_T, OUTPUT);
  wiringPiISR(SF_T, INT_EDGE_BOTH, &checkSmokeGround);

  pinMode(SJ_T01, OUTPUT);
  wiringPiISR(SJ_T01, INT_EDGE_BOTH, &checkWindow1Ground);
  
  pinMode(SJ_T02, OUTPUT);
  wiringPiISR(SJ_T02, INT_EDGE_BOTH, &checkWindow2Ground);

  pinMode(SPo_T, OUTPUT);
  wiringPiISR(SPo_T, INT_EDGE_BOTH, &checkDoorGround);

  pinMode(SP_1, OUTPUT);
  wiringPiISR(SP_1, INT_EDGE_BOTH, &checkPresenceFirstFloor);

  pinMode(SF_1, OUTPUT);
  wiringPiISR(SF_1, INT_EDGE_BOTH, &checkSmokeFirstFloor);

  pinMode(SJ_101, OUTPUT);
  wiringPiISR(SJ_101, INT_EDGE_BOTH, &checkWindow1FirstFloor);

  pinMode(SJ_102, OUTPUT);
  wiringPiISR(SJ_102, INT_EDGE_BOTH, &checkWindow2FirstFloor);

  for (;;) {
    sleep(1);
  }
}

void* peopleQuantityHandler() {
  pinMode(SC_IN, OUTPUT);
  wiringPiISR(SC_IN, INT_EDGE_BOTH, &checkPeopleQuantityGround_IN);

  pinMode(SC_OUT, OUTPUT);
  wiringPiISR(SC_OUT, INT_EDGE_BOTH, &checkPeopleQuantityGround_OUT);

  for (;;) {
    sleep(1);
  }
}

void setDeviceState(int device, int state) {
  pinMode(device, OUTPUT);
  digitalWrite(device, state);
}

void turnDevicesOff() {
  setDeviceState(LS_T01, LOW);
  setDeviceState(LS_T02, LOW);
  setDeviceState(LC_T, LOW);
  setDeviceState(AC_T, LOW);
  setDeviceState(ASP, LOW);
  setDeviceState(LS_101, LOW);
  setDeviceState(LS_102, LOW);
  setDeviceState(LC_1, LOW);
  setDeviceState(AC_1, LOW);
}

// Private functions to handlers

void checkPresenceGround() {
    printf("Térreo - Sensor de Presença Ativado!!!\n");
    sendCommand(SP_T);

    int sensorState = digitalRead(SP_T);
    int lampState = digitalRead(LC_T);

    if (sensorState == 1 && lampState == 0) {
        pinMode(LC_T, OUTPUT);
        digitalWrite(LC_T, HIGH);
        sendCommand(LC_T);

        sleep(10);

        pinMode(LC_T, OUTPUT);
        digitalWrite(LC_T, LOW);
        sendCommand(LC_T);
    }
}

void checkSmokeGround() {
    printf("Térro - Sensor de Fumaça Ativado!!! FOGOOO!!!\n");
    sendCommand(SF_T);

    int sensorState = digitalRead(SF_T);

    if (sensorState == 1) {
        pinMode(ASP, OUTPUT);
        digitalWrite(ASP, HIGH);
        sendCommand(ASP);
    } else {
        pinMode(ASP, OUTPUT);
        digitalWrite(ASP, LOW);
        sendCommand(ASP);
    }
}

void checkWindow1Ground() {
    printf("Térreo - Sensor da Janela 1 Ativado!!!\n");
    sendCommand(SJ_T01);
}

void checkWindow2Ground() {
    printf("Térreo - Sensor da Janela 2 Ativado!!!\n");
    sendCommand(SJ_T02);
}

void checkDoorGround() {
    printf("Térreo - Sensor da Porta Ativado!!!\n");
    sendCommand(SPo_T);
}

void checkPeopleQuantityGround_IN() {
    int sensorState = digitalRead(SC_IN);

    if (sensorState == 1) {
        printf("Térreo - Sensor de contagem interno ativado!!!\n");
        sendCommand(SC_IN);
        delay(280);
    }
}

void checkPeopleQuantityGround_OUT() {
    int sensorState = digitalRead(SC_OUT);

    if (sensorState == 1) {
        printf("Térreo - Sensor de contagem externo ativado!!!\n");
        sendCommand(SC_OUT);
        delay(280);
    }
}

void checkPresenceFirstFloor() {
    printf("Primeiro andar - Sensor de Presença Ativado!!!\n");
    sendCommand(SP_1);

    int sensorState = digitalRead(SP_1);
    int lampState = digitalRead(LC_1);

    if (sensorState == 1 && lampState == 0) {
        pinMode(LC_1, OUTPUT);
        digitalWrite(LC_1, HIGH);
        sendCommand(LC_1);

        sleep(10);

        pinMode(LC_1, OUTPUT);
        digitalWrite(LC_1, LOW);
        sendCommand(LC_1);
    }
}

void checkSmokeFirstFloor() {
    printf("Primeiro andar - Sensor de Fumaça Ativado!!! FOGOOO!!!\n");
    sendCommand(SF_1);

    int sensorState = digitalRead(SF_1);

    if (sensorState == 1) {
        pinMode(ASP, OUTPUT);
        digitalWrite(ASP, HIGH);
        sendCommand(ASP);
    } else {
        pinMode(ASP, OUTPUT);
        digitalWrite(ASP, LOW);
        sendCommand(ASP);
    }
}

void checkWindow1FirstFloor() {
    printf("Primeiro andar - Sensor da Janela 1 Ativado!!!\n");
    sendCommand(SJ_101);
}

void checkWindow2FirstFloor() {
    printf("Primeiro andar - Sensor da Janela 2 Ativado!!!\n");
    sendCommand(SJ_102);
}