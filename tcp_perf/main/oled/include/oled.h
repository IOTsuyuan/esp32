#ifndef __OLED_H
#define __OLED_H  

#include "stdint.h"

#define SDA_PIN GPIO_NUM_12
#define SCL_PIN GPIO_NUM_13
#define OLED_I2C_IO I2C_NUM_0
#define OLED_ADDR 0x3c
#define OLED_CMD 0x00
#define OLED_DAT 0x40

void OLED_Clean();
//开启OLED显示    
void OLED_Display_On(void);
//关闭OLED显示 
void OLED_Display_Off(void);
//坐标设置
void OLED_Set_Pos(unsigned char x, unsigned char y);
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~7			 
//size:选择字体 16/8
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size);
//显示一个字符号串
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t chr[],uint8_t Char_Size);
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//num:数值(0~4294967295);	
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size2);
//显示汉字
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no);
void OLED_Init();

#endif
