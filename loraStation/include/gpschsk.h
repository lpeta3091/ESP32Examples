#ifndef GPSCHSK_
#define GPSCHSK_

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define PACKETSIZE 16
#define DIVIDER    10000000.0

#define minPanValue   600
#define maxPanValue   4300
#define minTiltValue  600
#define maxTiltValue  1500

typedef struct __homePoz {
      int32_t lat;
      int32_t lon;
      int32_t alt;
} ppoz;

extern volatile int dutyPan, dutyTilt;
extern volatile ppoz homePoz, curPoz, antPoz, maxPoz;

extern volatile char strDisplay[128];
extern volatile char strDisplay1[128];
extern volatile char strDisplay2[128];
extern volatile char strDisplay3[128];
extern volatile char strDisplay4[128];
extern volatile char *bufDisplay;
extern char strPrint[7][18];
extern char strNull[6][16];

float altToTilt(float distance, float alt);
float coordToPan();

#endif /* GPSCHSK_ */
