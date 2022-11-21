#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "button.h"
#include "gpschsk.h"

static const char *TAG = "BTN";
//extern volatile int dutyPan, dutyTilt;

// extern volatile ppoz homePoz, curPoz, antPoz, maxPoz;


void scanTask1(void* arg) {
  uint16_t ticks = 0;

	ESP_LOGI(TAG, "Waiting For Press btn1.");

	for (;;)
	{

		// Wait here to detect press
		while( gpio_get_level(BUTTON1) )
		{
			vTaskDelay(125 / portTICK_PERIOD_MS);
		}

		// Debounce
		vTaskDelay(50 / portTICK_PERIOD_MS);

		// Re-Read Button State After Debounce
		if (!gpio_get_level(BUTTON1))
		{
			ESP_LOGI(TAG, "BTN1 Pressed Down.");

			ticks = 0;

			// Loop here while pressed until user lets go, or longer that set time
			while ((!gpio_get_level(BUTTON1)) && (++ticks < LONG_PRESS_IN_SECONDS * 100))
			{
				vTaskDelay(10 / portTICK_PERIOD_MS);
			}

			// Did fall here because user held a long press or let go for a short press
			if (ticks >= LONG_PRESS_IN_SECONDS * 100)
			{
				ESP_LOGI(TAG, "Long Press");
        homePoz = curPoz;
        sprintf(strPrint[0],"Lat : %10d",homePoz.lat);
        sprintf(strPrint[1],"Lon : %10d",homePoz.lon);
        sprintf(strPrint[2],"Alt : %10d",homePoz.alt);
        sprintf(strPrint[3],"Home poz %7s","Saved..");
        printf("Lat : %d, Lon : %d, Alt : %d \n", curPoz.lat, curPoz.lon, curPoz.alt);
        sprintf(strDisplay1,"%s\n%s\n%s\n%s",strPrint[0], strPrint[1], strPrint[2], strPrint[3]);
        bufDisplay = &strDisplay1;
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        bufDisplay = &strDisplay;
			}
			else
			{
				ESP_LOGI(TAG, "Short Press");
        // dutyTilt += 200;
        ESP_LOGI(TAG, "Tilt - %d",dutyTilt);
        sprintf(strPrint[0],"Lat : %10d",homePoz.lat);
        sprintf(strPrint[1],"Lon : %10d",homePoz.lon);
        sprintf(strPrint[2],"Alt : %10d",homePoz.alt);
        printf("Lat : %d, Lon : %d, Alt : %d \n", curPoz.lat, curPoz.lon, curPoz.alt);
        sprintf(strDisplay1,"%s\n%s\n%s",strPrint[0], strPrint[1], strPrint[2]);
        bufDisplay = &strDisplay1;
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        bufDisplay = &strDisplay;

			}

			// Wait here if they are still holding it
			while(!gpio_get_level(BUTTON1))
			{
				vTaskDelay(100 / portTICK_PERIOD_MS);
			}

			ESP_LOGI(TAG, "BTN1 Released.");
		}

	}
}


void scanTask2(void* arg)
{
	uint16_t ticks = 0;

	ESP_LOGI(TAG, "Waiting For Press btn2.");

	for (;;)
	{

		// Wait here to detect press
		while( gpio_get_level(BUTTON2) )
		{
			vTaskDelay(125 / portTICK_PERIOD_MS);
		}

		// Debounce
		vTaskDelay(50 / portTICK_PERIOD_MS);

		// Re-Read Button State After Debounce
		if (!gpio_get_level(BUTTON2))
		{
			ESP_LOGI(TAG, "BTN2 Pressed Down.");

			ticks = 0;

			// Loop here while pressed until user lets go, or longer that set time
			while ((!gpio_get_level(BUTTON2)) && (++ticks < LONG_PRESS_IN_SECONDS * 100))
			{
				vTaskDelay(10 / portTICK_PERIOD_MS);
			}

			// Did fall here because user held a long press or let go for a short press
			if (ticks >= LONG_PRESS_IN_SECONDS * 100)
			{
				ESP_LOGI(TAG, "Long Press");
        antPoz = curPoz;
        sprintf(strPrint[0],"Lat : %10d",antPoz.lat);
        sprintf(strPrint[1],"Lon : %10d",antPoz.lon);
        sprintf(strPrint[2],"Alt : %10d",antPoz.alt);
        sprintf(strPrint[3],"Ant poz %8s","Saved..");
        sprintf(strDisplay1,"%s\n%s\n%s\n%s",strPrint[0], strPrint[1], strPrint[2], strPrint[3]);
        bufDisplay = &strDisplay1;
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        bufDisplay = &strDisplay;
			}
			else
			{
				ESP_LOGI(TAG, "Short Press");
        //dutyTilt -= 50;
        ESP_LOGI(TAG, "Tilt - %d",dutyTilt);
        sprintf(strPrint[0],"Lat : %10d",antPoz.lat);
        sprintf(strPrint[1],"Lon : %10d",antPoz.lon);
        sprintf(strPrint[2],"Alt : %10d",antPoz.alt);
        strcpy(strDisplay1,strNull);
        sprintf(strDisplay1,"%s\n%s\n%s",strPrint[0], strPrint[1], strPrint[2]);
        bufDisplay = &strDisplay1;
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        bufDisplay = &strDisplay;
			}

			// Wait here if they are still holding it
			while(!gpio_get_level(BUTTON2))
			{
				vTaskDelay(100 / portTICK_PERIOD_MS);
			}

			ESP_LOGI(TAG, "BTN2 Released.");
		}

	}
}
