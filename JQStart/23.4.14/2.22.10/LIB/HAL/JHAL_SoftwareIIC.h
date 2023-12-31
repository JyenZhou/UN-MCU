#ifndef _JHAL_IICX_H
#define _JHAL_IICX_H

#include "HAL\JHAL_GPIO.h"


/*模拟IIC 使用需要定义全局的 JHAL_IIC_Number
 使用的id要小于定义的JHAL_IIC_Number  这个id是数组下标 从0开始

 */


typedef enum
{
//标准的
    JHAL_IIC_Device_Standard=0,
//对于TEA5767
    JHAL_IIC_Device_TEA5767

} JHAL_IIC_Device_Type;



typedef struct
{
//这个是延时系数最小值应该为1 要是降低速率可以修改这里值  注意IO有翻转时间的间隔太小可能导致时序不准 所以0则全程没有延时正常情况下该时序是有问题的
    u8 delayMultiple;
    JHAL_IO_Port sclPort;
    u8  sclPin;
    JHAL_IO_Port sdaPort;
    u8  sdaPin;
    u16 slaveAddress;
    JHAL_IIC_Device_Type deviceType;
//IO是否推挽模式 否则开漏
    bool isIO_Mode_PP;

} JHAL_IICConfig;



//初始化模拟的IIC
void JHAL_iicInit(u8 id,JHAL_IICConfig init );
/*devAddress 是器件地址  写的时候*/

//只有器件地址的操作
//发送/读取  一个寄存器中N个数据
bool  JHAL_iicTransmit(u8 id, uint8_t *pData, u8 dataSize);
bool  JHAL_iicReceice(u8 id, uint8_t *pData,  u8 dataSize);



//器件地址+寄存器地址的操作
//从往指定寄存器写一些数据
bool JHAL_iicMemWrite(u8 id, u16 memAddress,bool memSizeIs16,  u8 *pData,  u8 dataSize);
//从指定寄存器中读出一些数据
bool JHAL_iicMemRead(u8 id, u16 memAddress,bool memSizeIs16, u8 *pData, u8 dataSize);



#endif

