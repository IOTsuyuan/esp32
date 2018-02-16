#ifndef _bmp180_H_
#define _bmp180_H_

#include "math.h"

#define BMP180_ADDRESS 0x77

enum
{
	BMP085_REGISTER_CAL_AC1            = 0xAA,  // R   Calibration data (16 bits)
	BMP085_REGISTER_CAL_AC2            = 0xAC,  // R   Calibration data (16 bits)
	BMP085_REGISTER_CAL_AC3            = 0xAE,  // R   Calibration data (16 bits)
	BMP085_REGISTER_CAL_AC4            = 0xB0,  // R   Calibration data (16 bits)
	BMP085_REGISTER_CAL_AC5            = 0xB2,  // R   Calibration data (16 bits)
	BMP085_REGISTER_CAL_AC6            = 0xB4,  // R   Calibration data (16 bits)
	BMP085_REGISTER_CAL_B1             = 0xB6,  // R   Calibration data (16 bits)
	BMP085_REGISTER_CAL_B2             = 0xB8,  // R   Calibration data (16 bits)
	BMP085_REGISTER_CAL_MB             = 0xBA,  // R   Calibration data (16 bits)
	BMP085_REGISTER_CAL_MC             = 0xBC,  // R   Calibration data (16 bits)
	BMP085_REGISTER_CAL_MD             = 0xBE,  // R   Calibration data (16 bits)
	BMP085_REGISTER_CHIPID             = 0xD0,
	BMP085_REGISTER_VERSION            = 0xD1,
	BMP085_REGISTER_SOFTRESET          = 0xE0,
	BMP085_REGISTER_CONTROL            = 0xF4,
	BMP085_REGISTER_TEMPDATA           = 0xF6,
	BMP085_REGISTER_PRESSUREDATA       = 0xF6,
	BMP085_REGISTER_READTEMPCMD        = 0x2E,
	BMP085_REGISTER_READPRESSURECMD    = 0x34 // 0011 0100
};

typedef enum
{
  BMP085_MODE_ULTRALOWPOWER          = 0,   
  BMP085_MODE_STANDARD               = 1,
  BMP085_MODE_HIGHRES                = 2,
  BMP085_MODE_ULTRAHIGHRES           = 3
} bmp085_mode_t;    //模式不同，默认是 1 更改精度可能要修改读取数据函数

void task_bmp180(void *ignore);


#endif
