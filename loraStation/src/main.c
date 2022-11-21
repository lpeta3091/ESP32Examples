#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/ledc.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include "button.h"
#include "servo.h"
#include "ssd1366.h"
#include "lora.h"
#include "gpschsk.h"
#include <math.h>

extern volatile char strDisplay[];
volatile char *bufDisplay;
volatile char strDisplay[128] = "Hello gps !";
volatile char strDisplay1[128] = "Saved...";
volatile char strDisplay2[128];
volatile uint32_t ccount = 0;

struct __dataSend {
  int32_t lat; // Latitude
  int32_t lon; // Longitude
  int32_t alt; // Altitude
  uint32_t counter;
} dataSend;

 volatile ppoz antPoz = { 505114000, 302353230, 5000 }, curPoz, homePoz = { 505134460, 302357000, 5000 }, maxPoz;


char strPrint[7][18];

long double toRadians(const long double degree)
{
			 // cmath library in C++
			 // defines the constant
			 // M_PI as the value of
			 // pi accurate to 1e-30
			 long double one_deg = (M_PI) / 180;
			 return (one_deg * degree);
}

long double distanCe(long double lat1, long double long1,
                                        long double lat2, long double long2)
{
        lat1 = toRadians(lat1);
        long1 = toRadians(long1);
        lat2 = toRadians(lat2);
        long2 = toRadians(long2);

        // Haversine Formula
        long double dlong = long2 - long1;
        long double dlat = lat2 - lat1;

        long double ans = pow(sin(dlat / 2), 2) +
                                                cos(lat1) * cos(lat2) *
                                                pow(sin(dlong / 2), 2);

        ans = 2 * asin(sqrt(ans));

        // Radius of Earth in
        // Kilometers, R = 6371
        // Use R = 3956 for miles
        long double R = 6371;
        // Calculate the result
        ans = ans * R;

        return ans * 1000;
}

void task_rx(void *p)
{
   int x;
	 double distance;
   for(;;) {
      lora_receive();    // put into receive mode
      while(lora_received()) {
         x = lora_receive_packet((uint8_t *)&dataSend, sizeof(dataSend));
         // strDisplay[x] = 0;
         printf("Size - %d",x);
         if (x == PACKETSIZE) {
             printf("Lat : %d, Lon : %d, Alt : %d, Count : %d \n", dataSend.lat, dataSend.lon, dataSend.alt, dataSend.counter);
    				 curPoz.lat = dataSend.lat;
    				 curPoz.lon = dataSend.lon;
    				 curPoz.alt = dataSend.alt;
    				 distance = distanCe(curPoz.lat / DIVIDER, curPoz.lon / DIVIDER, homePoz.lat / DIVIDER, homePoz.lon / DIVIDER);
    				 if (distance > 50000) distance = 0;

             printf("Home lat : %d ,lon : %d ,alt : %d\n", homePoz.lat, homePoz.lon, homePoz.alt);
             printf("Ant lat : %d ,lon : %d ,alt : %d\n", antPoz.lat, antPoz.lon, antPoz.alt);

    				 printf("Distance : %f \n", distance);
             distance = (distance) ? distance : (distance * -1);
             float angleTilt = altToTilt(distance, curPoz.alt / 1000.0f);
             printf("Tilt : %lf \n", angleTilt);
             dutyTilt = angleTilt;
             dutyPan = coordToPan();
             printf("Pan : %d \n", dutyPan);
             sprintf(strPrint[0],"Lat : %10d",dataSend.lat);
             sprintf(strPrint[1],"Lon : %10d",dataSend.lon);
             sprintf(strPrint[2],"Alt : %10d",dataSend.alt);
             sprintf(strPrint[3],"Count : %8d",dataSend.counter);
    				 sprintf(strPrint[4],"Distance : %2.1f", distance);
             sprintf(strPrint[5],"Tilt : %9d", dutyTilt);
             sprintf(strPrint[6],"Pan : %10d", dutyPan);
             sprintf(strDisplay,"%s\n%s\n%s\n%s\n%s\n%s\n%s",strPrint[0], strPrint[1], strPrint[2], strPrint[3], strPrint[4], strPrint[5], strPrint[6]);
    //				 display_text(strNull);
    //				 task_ssd1306_display_pattern(NULL);
          }
         lora_receive();
      }
      vTaskDelay(1);
   }
}

extern volatile char strDisplay[];

static void buttonScanTask1(void* arg)
{
	scanTask1(arg);
}

static void buttonScanTask2(void* arg)
{
	scanTask2(arg);
}

void Servo_task1(void *ignore)
{
	servoTask1(ignore);
}

void Servo_task2(void *ignore)
{
	servoTask2(ignore);
}

void app_main()
{
				lora_init();
				lora_set_frequency(868e6);
				lora_set_spreading_factor(7);
				lora_set_bandwidth(125E3);
				lora_enable_crc();
				xTaskCreate(&task_rx, "task_rx", 2048, NULL, 5, NULL);

				gpio_set_direction(BUTTON1, GPIO_MODE_INPUT);
				gpio_pullup_en(BUTTON1);
				gpio_set_direction(BUTTON2, GPIO_MODE_INPUT);
				gpio_pullup_en(BUTTON2);

				i2c_master_init();
		  	ssd1306_init();

				xTaskCreate(&buttonScanTask1, "buttonScanTask1", 2048, NULL, 5, NULL);
				xTaskCreate(&buttonScanTask2, "buttonScanTask2", 2048, NULL, 5, NULL);
        xTaskCreate(&Servo_task1,"Servo_task1",2048,NULL,10,NULL);
				xTaskCreate(&Servo_task2,"Servo_task2",2048,NULL,10,NULL);

				xTaskCreate(&task_ssd1306_display_clear, "ssd1306_display_clear",  2048, NULL, 6, NULL);
		  	vTaskDelay(100/portTICK_PERIOD_MS);
				bufDisplay = &strDisplay;
				xTaskCreate(&task_ssd1306_display_text, "ssd1306_display_text",  2048,	(void *)bufDisplay, 6, NULL);
}
