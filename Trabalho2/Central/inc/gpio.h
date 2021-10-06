#ifndef GPIO_H_
#define GPIO_H_

#define LS_T01 7
#define LS_T02 0
#define LC_T 2
#define AC_T 11
#define ASP 27
#define LS_101 3
#define LS_102 6
#define LC_1 10
#define AC_1 26

#define SP_T 25
#define SF_T 4
#define SJ_T01 13
#define SJ_T02 14
#define SPo_T 12
#define SC_IN 23
#define SC_OUT 24
#define SP_1 1
#define SF_1 5
#define SJ_101 21
#define SJ_102 22

typedef struct {
  int lsT01;
  int lsT02;
  int lcT;
  int acT;
  int asp;
  int ls101;
  int ls102;
  int lc1;
  int ac1;
  int alarm;
  int alarmPlaying;
} DevicesOut;

typedef struct {
  int spT;
  int sfT;
  int sjT01;
  int sjT02;
  int spoT;
  int scIn;
  int scOut;
  int sp1;
  int sf1;
  int sj101;
  int sj102;
  int peopleQuantity;
} DevicesIn;

typedef struct {
  float temperature;
  float humidity;
} DHT22;

typedef struct {
  DHT22 dht22GroundFloor;
  DHT22 dht22FirstFloor;
  DevicesOut devOut;
} Data;

void dataInit();
void devicesInHandler(int command);
void storeDevicesOutUpdate(DevicesOut devOutUpdated);
DevicesOut recoverDevicesOutData();

#endif
