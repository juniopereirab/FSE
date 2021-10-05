/*
 *  dht.c:
 *      Author: Juergen Wolf-Hofer
 *      based on / adapted from http://www.uugear.com/portfolio/read-dht1122-temperature-humidity-sensor-from-raspberry-pi/
 *	reads temperature and humidity from DHT11 or DHT22 sensor and outputs according to selected mode
 */

#include "dht22.h"

#define PIN_GROUND_FLOOR 28
#define PIN_FIRST_FLOOR 29

int data[5] = { 0, 0, 0, 0, 0 };

// FUNCTION DECLARATIONS
int dht22_setup();
int dht22_read_data(float *temperature, float *humidity, int floor);

// FUNCTION DEFINITIONS
int dht22_read_data(float *temperature, float *humidity, int floor) {
	uint8_t laststate = HIGH;
	uint8_t counter	= 0;
	uint8_t j = 0;
	uint8_t i;

	data[0] = data[1] = data[2] = data[3] = data[4] = 0;

	/* pull pin down for 18 milliseconds */
	pinMode(floor == 0 ? PIN_GROUND_FLOOR : PIN_FIRST_FLOOR, OUTPUT);
	digitalWrite(floor == 0 ? PIN_GROUND_FLOOR : PIN_FIRST_FLOOR, LOW);
	delay(18);

	/* prepare to read the pin */
	pinMode(floor == 0 ? PIN_GROUND_FLOOR : PIN_FIRST_FLOOR, INPUT);

	/* detect change and read data */
	for ( i = 0; i < MAX_TIMINGS; i++ ) {
		counter = 0;
		while ( digitalRead( floor == 0 ? PIN_GROUND_FLOOR : PIN_FIRST_FLOOR ) == laststate ) {
			counter++;
			delayMicroseconds( 1 );
			if ( counter == 255 ) {
				break;
			}
		}
		laststate = digitalRead( floor == 0 ? PIN_GROUND_FLOOR : PIN_FIRST_FLOOR );

		if ( counter == 255 )
			break;

		/* ignore first 3 transitions */
		if ( (i >= 4) && (i % 2 == 0) ) {
			/* shove each bit into the storage bytes */
			data[j / 8] <<= 1;
			if ( counter > 16 )
				data[j / 8] |= 1;
			j++;
		}
	}

	/*
	 * check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
	 * print it out if data is good
	 */
	if ( (j >= 40) && (data[4] == ( (data[0] + data[1] + data[2] + data[3]) & 0xFF) ) ) {
		float h = (float)((data[0] << 8) + data[1]) / 10;
		// if ( h > 100 ) {
		// 	h = data[0];	// for DHT11
		// }
		float c = (float)(((data[2] & 0x7F) << 8) + data[3]) / 10;
		// if ( c > 125 ) {
		// 	c = data[2];	// for DHT11
		// }
		if ( data[2] & 0x80 ) {
			c = -c;
		}
		*temperature = c;
		*humidity = h;
		
		return 0; // OK
	} else {
		*temperature = *humidity = -1.0;
		
		return 1; // NOK
	}
}

int dht22_setup() {
	if (wiringPiSetup() == -1) {
		fprintf(stderr, "Failed to initialize wiringPi\n");
		exit(1);
		return 1;
	}
	return 0;
}
