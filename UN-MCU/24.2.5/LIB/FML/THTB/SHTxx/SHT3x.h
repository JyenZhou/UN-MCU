#ifndef _SHT3X_H
#define _SHT3X_H


#include "JHAL.h"





typedef enum
{

    //ADDR PIN ->VSS 0x44  <<1
    SHT3x_ADDRESS_VSS=0x88,
    //ADDR PIN ->VDD  0x45 <<1
    SHT3x_ADDRESS_VDD=0x8A,

} SHT3x_Addr;





typedef struct
{

//这个是延时系数最小值应该为1 要是降低速率可以修改这里值  注意IO有翻转时间的间隔太小可能导致时序不准 所以0则全程没有延时正常情况下该时序是有问题的

    JHAL_I2CSF i2c;

bool isSingleConversion;

}  SHT3x;






bool  sht3xOpen( SHT3x  *config);

//mdianNumbe 中位数个数
//true读取成功 false 读取失败
bool sht3xGet (SHT3x  *config, float *  temperature,float * humidity );



#endif



