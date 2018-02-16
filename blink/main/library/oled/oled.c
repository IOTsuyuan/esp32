#include "oledfont.h"
#include "hal_i2c.h"
#include "oled.h"

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
		i2c_cmd_link_delete(cmd1);
		

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
//m^n函数
uint32_t oled_pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}			
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size2)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
} 
//显示汉字
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no)
{      			    
	OLED_Set_Pos(x,y);	
		hal_i2c_master_mem_write(OLED_I2C_IO, OLED_ADDR, OLED_DAT, &Zx[2*no], 16);
		
	OLED_Set_Pos(x,y+1);	
		hal_i2c_master_mem_write(OLED_I2C_IO, OLED_ADDR, OLED_DAT, &Zx[2*no+1], 16);
}

void OLED_Init(){
	uint8_t Init_CMD[27] = {0xae,0x00,0x10,0x40,0xb0,0x81,0xff,0xa1,0xa6,0xa8,0x3f,0xc8,
	0xd3,0x00,0xd5,0x80,0xd8,0x05,0xd9,0xf1,0xda,0x12,0xdb,0x30,0x8d,0x14,0xaf};

	hal_i2c_init(OLED_I2C_IO, SDA_PIN, SCL_PIN);
	hal_i2c_master_mem_write(OLED_I2C_IO, OLED_ADDR, OLED_CMD, Init_CMD, 27);


}