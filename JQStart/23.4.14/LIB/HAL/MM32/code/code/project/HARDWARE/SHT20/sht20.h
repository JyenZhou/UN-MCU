#ifndef __SHT20_H
#define __SHT20_H
#include "main.h"
#include "i2c.h"

typedef struct
{

	unsigned short speed;

} IIC_INFO;

extern IIC_INFO iic_info;

#define IIC_OK		0
#define IIC_Err		1

#define SHT20_ADDRESS  0X40
#define SHT20_Measurement_RH_HM  0XE5
#define SHT20_Measurement_T_HM  0XE3
#define SHT20_Measurement_RH_NHM  0XF5
#define SHT20_Measurement_T_NHM  0XF3
#define SHT20_READ_REG  0XE7
#define SHT20_WRITE_REG  0XE6
#define SHT20_SOFT_RESET  0XFE

typedef struct
{
    float tempreture;
    float humidity;
	
} SHT20_INFO;

extern SHT20_INFO sht20_info;

void SHT20_GetValue(void);

#endif
