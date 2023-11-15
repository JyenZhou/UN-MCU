#ifndef __SHT20__H__
#define __SHT20__H__
#include "JHAL.h"
#ifdef __cplusplus
extern "C" {
#endif




typedef enum
{
    TRIG_TEMP_MEASUREMENT_HM   = 0xE3, //command trig. temp meas. hold master
    TRIG_HUMI_MEASUREMENT_HM   = 0xE5, //command trig. humidity meas. hold master
    TRIG_TEMP_MEASUREMENT_POLL = 0xF3, //command trig. temp meas. no hold master
    TRIG_HUMI_MEASUREMENT_POLL = 0xF5, //command trig. humidity meas. no hold master
    USER_REG_W                 = 0xE6, //command writing user register
    USER_REG_R                 = 0xE7, //command reading user register
    SOFT_RESET                 = 0xFE  //command soft reset
} SHT20Command;


typedef enum
{
    I2C_ADR_W                  = 0x80, //sensor I2C address + write bit
    I2C_ADR_R                  = 0x81  //sensor I2C address + read bit
} SHT20I2cHeader;






void  SHT20_Init(void);
uint16 SHT20_MeasureTemp(void);
uint16 SHT20_MeasureHumi(void);

u8 SHT20_ReadUserReg(void);
u8 SHT20_WriteUserReg(u8 reg);
void SHT20_SoftReset(void);
void SHT20_Test(void);




#ifdef __cplusplus
}
#endif

#endif




