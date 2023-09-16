#ifndef __JHAL_CRC__H__
#define __JHAL_CRC__H__
#include "JSYS.h"

typedef enum
{   //不使用 主要是给有些地方做兼容用
    JHAL_CRC_Mode_NONE =0,
    JHAL_CRC_Mode_16_Modbus,
    JHAL_CRC_Mode_8_XOR
} JHAL_CRC_Mode;



//传进来包含将要修改的最后X位
void  JHAL_crcAutoWirte(JHAL_CRC_Mode mode,  u8  *pdata, u16 length);
bool  JHAL_crcCheak(JHAL_CRC_Mode mode,  u8  *pdata, u16 length);
u16  JHAL_crc(JHAL_CRC_Mode mode,  u8  *pdata, u16 length);

#endif

