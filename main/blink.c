#include <driver/i2c.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>
#include "sdkconfig.h"
#include "driver/gpio.h"
#include "hal_i2c.h"
#include "oled.h"



void app_main()
{
	uint8_t chr[] = "uint8_t chr[] uint8_t chr[]";
//    xTaskCreate(task_i2cscanner, "task_i2cscanner", 1024 * 2, (void* ) 0, 10, NULL);
	OLED_Init();
	OLED_Clean();
//	OLED_ShowChar(0,0,'x',6);
//	OLED_ShowChar(0,0,'X',16);
//	OLED_ShowString(2, 2, chr, 6);
//	OLED_ShowString(2, 4, chr, 16);
	OLED_ShowNum(0,1,520,3,12);
	OLED_ShowNum(30,0,520,3,16);
	OLED_ShowCHinese(0,2,0);
	OLED_ShowCHinese(16,2,1);

}

