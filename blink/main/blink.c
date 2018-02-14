#include <driver/i2c.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>
#include "sdkconfig.h"
#include "driver/gpio.h"
#include "hal_i2c.h"
#include "oledfont.h"

#define SDA_PIN GPIO_NUM_19
#define SCL_PIN GPIO_NUM_18
#define OLED_I2C_IO I2C_NUM_0
#define OLED_ADDR 0x3c
#define OLED_CMD 0x00
#define OLED_DAT 0x40
/*
static char tag[] = "i2cscanner";

void task_i2cscanner(void *ignore) {
	ESP_LOGD(tag, ">> i2cScanner");
	i2c_config_t conf;
	conf.mode = I2C_MODE_MASTER;
	conf.sda_io_num = SDA_PIN;
	conf.scl_io_num = SCL_PIN;
	conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
	conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
	conf.master.clk_speed = 100000;
	i2c_param_config(I2C_NUM_0, &conf);

	i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);

	int i;
	esp_err_t espRc;
	printf("     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\n");
	printf("00:         ");
	for (i=3; i< 0x78; i++) {
		i2c_cmd_handle_t cmd = i2c_cmd_link_create();
		i2c_master_start(cmd);
		i2c_master_write_byte(cmd, (i << 1) | I2C_MASTER_WRITE, 1 );
		i2c_master_stop(cmd);

		espRc = i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
		if (i%16 == 0) {
			printf("\n%.2x:", i);
		}
		if (espRc == 0) {
			printf(" %.2x", i);
		} else {
			printf(" --");
		}
		//ESP_LOGD(tag, "i=%d, rc=%d (0x%x)", i, espRc, espRc);
		i2c_cmd_link_delete(cmd);
	}
	printf("\n");
	vTaskDelete(NULL);
}*/
void OLED_Clean(){
	uint8_t i, n;
	for(i = 0;i < 8;i++){
		i2c_cmd_handle_t cmd = i2c_cmd_link_create();
		i2c_master_start(cmd);
		i2c_master_write_byte(cmd, (OLED_ADDR << 1) | I2C_MASTER_WRITE, 1 );
		i2c_master_write_byte(cmd, OLED_CMD, 1 );
		i2c_master_write_byte(cmd, 0xb0 + i, 1 );	
		i2c_master_write_byte(cmd, 0x00, 1 );
		i2c_master_write_byte(cmd, 0x10, 1 );	
		i2c_master_stop(cmd);
		i2c_master_cmd_begin(OLED_I2C_IO, cmd, 10/portTICK_PERIOD_MS);
		i2c_cmd_link_delete(cmd);

		i2c_cmd_handle_t cmd1 = i2c_cmd_link_create();
		i2c_master_start(cmd1);
		i2c_master_write_byte(cmd1, (OLED_ADDR << 1) | I2C_MASTER_WRITE, 1 );
		i2c_master_write_byte(cmd1, OLED_DAT, 1 );
		for(n = 0;n < 128;n++){
			i2c_master_write_byte(cmd1, 0x00, 1 );	
		}
		i2c_master_stop(cmd1);
		i2c_master_cmd_begin(OLED_I2C_IO, cmd1, 10/portTICK_PERIOD_MS);
		i2c_cmd_link_delete(cmd);
		

	}
}
//开启OLED显示    
void OLED_Display_On(void)
{
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (OLED_ADDR << 1) | I2C_MASTER_WRITE, 1 );
	i2c_master_write_byte(cmd, OLED_CMD, 1 );
	i2c_master_write_byte(cmd, 0X8D, 1 );	  //SET DCDC命令
	i2c_master_write_byte(cmd, 0x14, 1 );	  //DCDC ON
	i2c_master_write_byte(cmd, 0xaf, 1 );	  //DISPLAY ON
	i2c_master_stop(cmd);
	i2c_master_cmd_begin(OLED_I2C_IO, cmd, 10/portTICK_PERIOD_MS);
	i2c_cmd_link_delete(cmd);
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (OLED_ADDR << 1) | I2C_MASTER_WRITE, 1 );
	i2c_master_write_byte(cmd, OLED_CMD, 1 );
	i2c_master_write_byte(cmd, 0X8D, 1 );	  //SET DCDC命令
	i2c_master_write_byte(cmd, 0x10, 1 );	  //DCDC OFF
	i2c_master_write_byte(cmd, 0xae, 1 );	  //DISPLAY OFF
	i2c_master_stop(cmd);
	i2c_master_cmd_begin(OLED_I2C_IO, cmd, 10/portTICK_PERIOD_MS);
	i2c_cmd_link_delete(cmd);
}	
//坐标设置
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (OLED_ADDR << 1) | I2C_MASTER_WRITE, 1 );
	i2c_master_write_byte(cmd, OLED_CMD, 1 );
	i2c_master_write_byte(cmd, 0xb0+y, 1 );	  
	i2c_master_write_byte(cmd, ((x&0xf0)>>4)|0x10, 1 );	 
	i2c_master_write_byte(cmd, (x&0x0f), 1 );	 
	i2c_master_stop(cmd);
	i2c_master_cmd_begin(OLED_I2C_IO, cmd, 10/portTICK_PERIOD_MS);
	i2c_cmd_link_delete(cmd);
}   
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~7			 
//size:选择字体 16/8
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size)
{      	
	unsigned char c=0;	
		c=chr-' ';//得到偏移后的值			
		if(x>128-1){x=0;y=y+2;}//如果超出了128列就从0列开始并换到下一页
		if(Char_Size ==16)
			{
				OLED_Set_Pos(x,y);	
				hal_i2c_master_mem_write(OLED_I2C_IO, OLED_ADDR, OLED_DAT, &F8X16[c*16], 8);
				OLED_Set_Pos(x,y+1);
				hal_i2c_master_mem_write(OLED_I2C_IO, OLED_ADDR, OLED_DAT, &F8X16[c*16+8], 8);
			}
			else {	
				OLED_Set_Pos(x,y);
				hal_i2c_master_mem_write(OLED_I2C_IO, OLED_ADDR, OLED_DAT, &F6x8[c * 6], 6);
				
			}
}
//显示一个字符号串
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t chr[],uint8_t Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		
		OLED_ShowChar(x,y,chr[j],Char_Size);
		if(Char_Size == 16)
		{
			x += 8;
			if(x > (-(128 - x) / 8) + 128)	{x = 0;y +=2;}
		}
		else
		{
			x += 6;
			if(x > (-(128 - x) / 6) + 128)	{x = 0;y +=1;}
		}
			
			j++;
	}
}

void OLED_Init(){
	uint8_t Init_CMD[27] = {0xae,0x00,0x10,0x40,0xb0,0x81,0xff,0xa1,0xa6,0xa8,0x3f,0xc8,
	0xd3,0x00,0xd5,0x80,0xd8,0x05,0xd9,0xf1,0xda,0x12,0xdb,0x30,0x8d,0x14,0xaf};

	hal_i2c_init(OLED_I2C_IO, SDA_PIN, SCL_PIN);
	hal_i2c_master_mem_write(OLED_I2C_IO, OLED_ADDR, OLED_CMD, Init_CMD, 27);


}

void app_main()
{
	uint8_t chr[] = "uint8_t chr[] uint8_t chr[]";
//    xTaskCreate(task_i2cscanner, "task_i2cscanner", 1024 * 2, (void* ) 0, 10, NULL);
	OLED_Init();
	OLED_Clean();
//	OLED_ShowChar(0,0,'x',6);
	OLED_ShowChar(0,0,'X',16);
	OLED_ShowString(2, 2, chr, 6);
	OLED_ShowString(2, 4, chr, 16);
}

