#include "config.h"

extern uint8_t dataFromDevice[25];

void sendToDevice(void) {

}

void clearDataFromDevice(void) {
	int i = 0;
	for (i = 0; i < 25; ++i) {
		dataFromDevice[i] = 0;
	}
}


uint8_t getDistance(void) {
	int i = 0;
	int byteCounter = 0;
	int sum;

	for (i = 0; i < 25; ++i) {
		if (dataFromDevice[i] != 0) {
			sum += dataFromDevice[i];
			byteCounter++;
		}
	}

	return (uint8_t) sum / byteCounter;
}
