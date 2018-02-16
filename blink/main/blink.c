#include <driver/i2c.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "sdkconfig.h"
#include "hal_i2c.h"
#include "bmp180.h"
#include "oled.h"
#include "rom/ets_sys.h"
#include "driver/gpio.h"

#define DHT11_IO GPIO_NUM_27

gpio_config_t DHT11_IO_IN



void app_main()
{
	OLED_Init();
	OLED_Clean();
    xTaskCreate(&task_bmp180, "task_bmp180", 1024*5, NULL, 5, NULL);
}
