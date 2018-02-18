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
#include "DHT11.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_log.h"

static const char *BMP180_I2C_LOG_TAG = "BMP180 I2C Read";
static const char *DHT11_LOG_TAG = "DHT11 Read";

unsigned char DHT11_TEM[] = "DTEM:";
unsigned char DHT11_HUM[] = "DHUM:";
unsigned char BMP180_PRESSURE[] = "PRE:";
unsigned char BMP180_ALTITUDE[] = "ALT:";
unsigned char BMP180_TEM[] = "BTEM:";
unsigned char LOADING[] = "LOADING...";

#define REFERENCE_PRESSURE 101325l

#define I2C_PIN_SDA 12
#define I2C_PIN_SCL 13

#define blink_gpio GPIO_NUM_5

void blink_task(void *pvParameter)
{
    gpio_set_direction(blink_gpio, GPIO_MODE_OUTPUT);
    gpio_set_level(blink_gpio,0);
    while(1)
    {
        gpio_set_level(blink_gpio, 1);
        vTaskDelay(500 / portTICK_RATE_MS);
        gpio_set_level(blink_gpio, 0);
        vTaskDelay(500 / portTICK_RATE_MS);
    }
}

void DHT_task(void *pvParameter)
{
	int tem_hum[3] = {0};
   setDHTPin(26);
//   getData(tem_hum);
   printf("Starting DHT measurement!\n");
   while(1)
   {
	    getData(tem_hum);
        ESP_LOGI(DHT11_LOG_TAG, "Temperature reading %d\n",tem_hum[1]);
        ESP_LOGI(DHT11_LOG_TAG, "F temperature is %d\n", tem_hum[2]);
        ESP_LOGI(DHT11_LOG_TAG, "Humidity reading %d\n",tem_hum[0]);
        OLED_ShowNum(40, 0, tem_hum[1], 2, 16);
        OLED_ShowNum(104, 0, tem_hum[0], 2, 16);
		vTaskDelay(3000 / portTICK_RATE_MS);
   }
}

void bmp180_i2c_task(void *pvParameter)
{
    while(1) {
        esp_err_t err;
        uint32_t pressure;
        float altitude;
        float temperature;

        err = bmp180_read_pressure(&pressure);
        if (err != ESP_OK) {
            ESP_LOGE(BMP180_I2C_LOG_TAG, "Reading of pressure from BMP180 failed, err = %d", err);
        }
        err = bmp180_read_altitude(REFERENCE_PRESSURE, &altitude);
        if (err != ESP_OK) {
            ESP_LOGE(BMP180_I2C_LOG_TAG, "Reading of altitude from BMP180 failed, err = %d", err);
        }
        err = bmp180_read_temperature(&temperature);
        if (err != ESP_OK) {
            ESP_LOGE(BMP180_I2C_LOG_TAG, "Reading of temperature from BMP180 failed, err = %d", err);
        }
        ESP_LOGI(BMP180_I2C_LOG_TAG, "Pressure %d Pa, Altitude %.1f m, Temperature : %.1f oC", pressure, altitude, temperature);
        OLED_ShowNum(32, 2, pressure, 6, 16);
        if(altitude < 0){
            OLED_ShowChar(32, 4, '-', 16);
            OLED_ShowNum(40, 4, -altitude, 2, 16);
            OLED_ShowChar(56, 4, '.', 16);
            OLED_ShowNum(64, 4, (uint32_t)(altitude / 1 * 10), 1, 16);
        }
        else{
            OLED_ShowNum(32, 4, altitude, 2, 16);
            OLED_ShowChar(48, 4, '.', 16);
            OLED_ShowNum(56, 4, (uint32_t)(altitude / 1 * 10), 1, 16);
        }
        if(temperature < 0){
            OLED_ShowChar(40, 6, '-', 16);
            OLED_ShowNum(48, 6, -temperature, 2, 16);
            OLED_ShowChar(64, 6, '.', 16);
            OLED_ShowNum(72, 6, (uint32_t)(temperature / 1 * 10), 1, 16);
        }
        else{
            OLED_ShowNum(40, 6, temperature, 2, 16);
            OLED_ShowChar(56, 6, '.', 16);
            OLED_ShowNum(64, 6, (uint32_t)(temperature / 1 * 10), 1, 16);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void oled_show()
{
    OLED_Init();
    OLED_Clean();
    OLED_ShowString(14, 3, LOADING, 16);
    vTaskDelay(1000 / portTICK_RATE_MS);    //等待dht11稳定下来
    OLED_Clean();
    OLED_ShowString(0, 0, DHT11_TEM, 16);
    OLED_ShowString(64, 0, DHT11_HUM, 16);
    OLED_ShowString(0, 2, BMP180_PRESSURE, 16);
    OLED_ShowString(0, 4, BMP180_ALTITUDE, 16);
    OLED_ShowString(0, 6, BMP180_TEM, 16);
}

void app_main()
{

    esp_err_t err;
    
    xTaskCreate(&blink_task, "blink_task", 1024, NULL, 5, NULL);
    ESP_LOGI(BMP180_I2C_LOG_TAG, "Main application is starting...");
    err = bmp180_init(I2C_PIN_SDA, I2C_PIN_SCL);
    if(err == ESP_OK){
        xTaskCreate(&bmp180_i2c_task, "bmp180_i2c_task", 1024*4, NULL, 5, NULL);
    } else {
        ESP_LOGE(BMP180_I2C_LOG_TAG, "BMP180 init failed with error = %d", err);
    }

    xTaskCreate(&DHT_task, "DHT_task", 2048, NULL, 5, NULL);
    
}



