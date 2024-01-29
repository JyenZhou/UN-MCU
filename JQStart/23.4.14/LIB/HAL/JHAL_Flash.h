/*************Jyen*************************Jyen********************Jyen***********************Jyen********************
   *                                                                *
 *                                                                   *
*
 		Flash默认通用HAL驱动  注意封装内部已经做了地址偏移   传进去的00地址就是falsh所在首地址
 		另外要注意大小不要超出

*                                                                     *
  *                                                                 *
   *                                                              *
*********Jyen******************Jyen************************Jyen*****************************************/

#ifndef __JHAL_FLASH__H__
#define __JHAL_FLASH__H__
#include "JUTIL.h"
#ifdef __CplusPlus
extern "C" {
#endif




    /*
    写falsh  自动擦除和加CRC与JHAL_flashRead一起用
    page 请从2开始 第一页预留给bootloader了不使用bootloader时候可以用   实际上从最后一页开始向前写
    data 可以是结构体
    length  数据大小多少个字节（u8) 注意长度要小于扇区-2（crc）
    	注意：不支持跨页写入每次只支持单页，已经若定义实现该函数
     */
    bool JHAL_flashWirte(u32 page,void *data,u16 length);

    /*读flash 返回值true crc校验成功 faslh校验失败 校验失败是不会修改传进来的值
    data 要读数据缓冲区
    length  数据大小多少个字节（u8)
      注意：已经弱定义通过直接读地址方式实现该函数*/


    bool JHAL_flashRead(u32 page,void *data,u16 length);

    //擦除地址段数据数据 注意起始要扇区对齐，结束地址内部自动对齐
    bool JHAL_flashErasePage(uint32_t startPageAddr, uint32_t endAddr);
    //在指定地址写入一定数据
    // 注意要记得先擦除，部分单片机内部采用的是8个字节对齐这里建议每次8字节对齐写 如果非要紧挨着写上一次写， 上次的尾部用1填充对齐8字节就不影响本次头部覆盖上次尾部
    bool JHAL_flashWriteNByte(uint32_t address,uint8_t *p_FlashBuffer,uint16 leng);





#ifdef CplusPlus
}
#endif

#endif








