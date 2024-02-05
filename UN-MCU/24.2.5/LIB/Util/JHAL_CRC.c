
/*crc校验*/

/*
参数说明
pdata 数据 包含要行校验后的数据预留位
 length  数据长度 含校验位的长度

校验模式下返回值 ture成功/false失败
AutoWirte 下自动修改包尾校验值
*/





#include "JHAL_CRC.h"


u16  __JHAL_crc16_Modbus ( u8  *pdata, u16 length)
{

    u16 crc=0xFFFF;
    int i, j;
    for ( j=0; j<length; j++)
    {
        crc=crc^pdata[j];
        for ( i=0; i<8; i++)
        {
            if( ( crc&0x0001) >0)
            {
                crc=crc>>1;
                crc=crc^ 0xa001;
            }
            else
                crc=crc>>1;
        }
    }

    return crc;
}
/*
*Modbus总线crc16效验算法C语言  （自动修改校验位值）
pdata 数据
length  数据长度 包含校验位的长度
（）

返回校验后crc (自动修改校验位值)

*/
void __JHAL_crc16_Modbus_AutoWirte ( u8  *pdata, u16 length)
{
    u16 crc=__JHAL_crc16_Modbus(pdata,length-2);
    pdata[length-2]=(u8 )(crc>>8);
    pdata[length-1]=(u8 )crc;
}


bool __JHAL_crc16_Modbus_Cheak (u8  *pdata, u16 length)
{
    u16  crc_Result=__JHAL_crc16_Modbus(pdata,length-2);
    if(pdata[length-2]==(u8 )(crc_Result>>8)&&pdata[length-1]==(u8 )crc_Result)
    {
        return true;
    } else {
        return false;
    }

}


/*异或校验
参数 数组
参数 长度 包含校验位的长度*/
u8  __JHAL_crc8_XOR( u8 *data, u16 length)
{
    u8  xordata;
    xordata = data[0];
	
    for( u8  i=1; i<length-1; i++)
    {
        xordata ^=data[i];
    }
    return xordata;
}
/*异或后并自动修改校验位值
参数 数组
参数 长度 包含校验位的长度*/
void __JHAL_crc8_XOR_AutoWirte( u8 *data, u16 length)
{
    data[length-1]= __JHAL_crc8_XOR(data,length);
}

bool __JHAL_crc8_XOR_Cheak( u8 *data, u16 length)
{
    if(data[length-1]== __JHAL_crc8_XOR(data,length))
    {
        return true;
    } else {
        return false;
    }

}








//修改最后一个字段
void  JHAL_crcAutoWirte(JHAL_CRC_Mode mode,  u8  *pdata, u16 length)
{
    if(mode==JHAL_CRC_Mode_8_XOR)
    {
        __JHAL_crc8_XOR_AutoWirte(  pdata,  length);
    } else if(mode==JHAL_CRC_Mode_16_Modbus)
    {
        __JHAL_crc16_Modbus_AutoWirte(  pdata,  length);
    }

}
//返回是否匹配
bool  JHAL_crcCheak(JHAL_CRC_Mode mode,  u8  *pdata, u16 length)
{
    if(mode==JHAL_CRC_Mode_8_XOR)
    {
        return   __JHAL_crc8_XOR_Cheak(  pdata,  length);
    } else if(mode==JHAL_CRC_Mode_16_Modbus)
    {
        return   __JHAL_crc16_Modbus_Cheak(  pdata,  length);
    }
    return true;
}
//返回的是值
u16  JHAL_crc(JHAL_CRC_Mode mode,  u8  *pdata, u16 length)
{
    if(mode==JHAL_CRC_Mode_8_XOR)
    {
        return   __JHAL_crc8_XOR(  pdata,  length);
    } else if(mode==JHAL_CRC_Mode_16_Modbus)
    {
        return   __JHAL_crc16_Modbus(  pdata,  length);
    }
    return 0;
}






