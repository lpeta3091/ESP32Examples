#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/ledc.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include "servo.h"
#include "ssd1366.h"
#include "gpschsk.h"

static char tag1[] = "servo1";
static char tag2[] = "servo2";

volatile int dutyPan = 0, dutyTilt = 0;


void servoTask1(void *ignore) {
	int bitSize         = 15;
	int minValue        = minPanValue;    // micro seconds (uS)
	int maxValue        = maxPanValue; //2500; // micro seconds (uS)
	int sweepDuration   = 1500; // milliseconds (ms)
	int duty            = (1<<bitSize) * minValue / 20000 ;//1638
	//int direction       = 1; // 1 = up, -1 = down
	int valueChangeRate = 20; // msecs

	dutyPan = (maxValue - minValue) / 2;

	ESP_LOGD(tag1, ">> task_servo1");
	ledc_timer_config_t timer_conf;
	timer_conf.bit_num    = LEDC_TIMER_15_BIT;
	timer_conf.freq_hz    = 50;
	timer_conf.speed_mode = LEDC_HIGH_SPEED_MODE;
	timer_conf.timer_num  = LEDC_TIMER_0;
	ledc_timer_config(&timer_conf);

	ledc_channel_config_t ledc_conf;
	ledc_conf.channel    = LEDC_CHANNEL_0;
	ledc_conf.duty       = duty;
	ledc_conf.gpio_num   = 4; //16;
	ledc_conf.intr_type  = LEDC_INTR_DISABLE;
	ledc_conf.speed_mode = LEDC_HIGH_SPEED_MODE;
	ledc_conf.timer_sel  = LEDC_TIMER_0;
	ledc_channel_config(&ledc_conf);

	int changesPerSweep = sweepDuration / valueChangeRate;// 1500/20 -> 75
	int changeDelta = (maxValue-minValue) / changesPerSweep;// 2000/75 -> 26
	//int i;
	ESP_LOGD(tag1, "sweepDuration: %d seconds", sweepDuration);
	ESP_LOGD(tag1, "changesPerSweep: %d", changesPerSweep);
	ESP_LOGD(tag1, "changeDelta: %d", changeDelta);
	ESP_LOGD(tag1, "valueChangeRate: %d", valueChangeRate);
	while(1) {
		 ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, dutyPan);
		 ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
		 vTaskDelay(valueChangeRate/portTICK_PERIOD_MS);
		 //ESP_LOGD(tag2, "Pan middle");
	}

	  // End loop forever

	vTaskDelete(NULL);
}

void servoTask2(void *ignore) {
	int bitSize         = 15;
	int minValue        = minTiltValue;  // micro seconds (uS)
	int maxValue        = maxTiltValue; // micro seconds (uS)
	int sweepDuration   = 1500; // milliseconds (ms)
	int duty            = (1<<bitSize) * minValue / 20000 ;//1638
	// int direction       = 1; // 1 = up, -1 = down
	int valueChangeRate = 20; // msecs
	dutyTilt = minValue;

	ESP_LOGD(tag2, ">> task_servo2");
	ledc_timer_config_t timer_conf;
	timer_conf.bit_num    = LEDC_TIMER_15_BIT;
	timer_conf.freq_hz    = 50;
	timer_conf.speed_mode = LEDC_HIGH_SPEED_MODE;
	timer_conf.timer_num  = LEDC_TIMER_1;
	ledc_timer_config(&timer_conf);

	ledc_channel_config_t ledc_conf;
	ledc_conf.channel    = LEDC_CHANNEL_1;
	ledc_conf.duty       = duty;
	ledc_conf.gpio_num   = 0; //16;
	ledc_conf.intr_type  = LEDC_INTR_DISABLE;
	ledc_conf.speed_mode = LEDC_HIGH_SPEED_MODE;
	ledc_conf.timer_sel  = LEDC_TIMER_1;
	ledc_channel_config(&ledc_conf);

	int changesPerSweep = sweepDuration / valueChangeRate;// 1500/20 -> 75
	int changeDelta = (maxValue-minValue) / changesPerSweep;// 2000/75 -> 26
	// int i;
	ESP_LOGD(tag2, "sweepDuration: %d seconds", sweepDuration);
	ESP_LOGD(tag2, "changesPerSweep: %d", changesPerSweep);
	ESP_LOGD(tag2, "changeDelta: %d", changeDelta);
	ESP_LOGD(tag2, "valueChangeRate: %d", valueChangeRate);
	while(1) {
		/*
		for (i=0; i<changesPerSweep; i++) {
			if (direction > 0) {
				duty += changeDelta;
			} else {
				duty -= changeDelta;
			}
			*/
			ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, dutyTilt);
			ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1);
			vTaskDelay(valueChangeRate/portTICK_PERIOD_MS);
			//ESP_LOGD(tag2, "Tilt 1000");
		// }

		//direction = -direction;
		//ESP_LOGD(tag2, "Direction now %d", direction);
	} // End loop forever

	vTaskDelete(NULL);
}
