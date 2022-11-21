#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define	BUTTON1	GPIO_NUM_34
#define	BUTTON2	GPIO_NUM_35

#define LONG_PRESS_IN_SECONDS 1

void scanTask1(void* arg);
void scanTask2(void* arg);
