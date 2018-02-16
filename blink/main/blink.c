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

void io_out(uint32_t level){
	gpio_set_level(DHT11_IO, level);
}

int io_in(){
	return gpio_get_level(DHT11_IO);
}

void DHT11_start()
{
   io_out(1);
   ets_delay_us(2);
   io_out(0);
   vTaskDelay(20/portTICK_PERIOD_MS);   //延时18ms以上
   io_out(1);
   ets_delay_us(40);
}

unsigned char DHT11_rec_byte()      //接收一个字节
{
   unsigned char i,dat=0;
  for(i=0;i<8;i++)    //从高到低依次接收8位数据
   {          
      while(!io_in());   ////等待50us低电平过去
      ets_delay_us(50);     //延时60us，如果还为高则数据为1，否则为0 
      dat<<=1;           //移位使正确接收8位数据，数据为0时直接移位
      if(io_in()==1)    //数据为1时，使dat加1来接收数据1
         dat+=1;
      while(io_in());  //等待数据线拉低    
    }  
    return dat;
}



void task_dht11(void *ignore) {

	unsigned char R_H,R_L,T_H,T_L,revise; 

	gpio_config_t DHT11_IO_CFG = {
		.pin_bit_mask = DHT11_IO,
		.mode = GPIO_MODE_OUTPUT,
		.pull_up_en = GPIO_PULLUP_ENABLE,
		.pull_down_en = GPIO_PULLDOWN_DISABLE,
		.intr_type = GPIO_INTR_DISABLE
	};
	gpio_config(&DHT11_IO_CFG);		//初始化与DHT11通信IO

	while(1) {
		DHT11_start();
		gpio_set_pull_mode(DHT11_IO,GPIO_MODE_INPUT);
		if(io_in()==0)
		{
			while(io_in()==0);   //等待拉高     
			ets_delay_us(40);  //拉高后延时80us
			R_H=DHT11_rec_byte();    //接收湿度高八位  
			R_L=DHT11_rec_byte();    //接收湿度低八位  
			T_H=DHT11_rec_byte();    //接收温度高八位  
			T_L=DHT11_rec_byte();    //接收温度低八位
			revise=DHT11_rec_byte(); //接收校正位

			ets_delay_us(25);    //结束
			printf("true: %c, %c, %c, %c",R_H,R_L,T_H,T_L);
			if((R_H+R_L+T_H+T_L)==revise)      //校正
			{
				
			} 
        /*数据处理，方便显示*/
		}
		gpio_set_pull_mode(DHT11_IO,GPIO_MODE_OUTPUT);
		vTaskDelay(1000/portTICK_PERIOD_MS);
	}
	vTaskDelete(NULL);
}



void app_main()
{
	OLED_Init();
	OLED_Clean();
//	xTaskCreate(&task_dht11, "task_dht11", 1024*5, NULL, 5, NULL);
//    xTaskCreate(&task_bmp180, "task_bmp180", 1024*4, NULL, 5, NULL);
}
