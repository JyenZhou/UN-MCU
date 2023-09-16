#ifndef __FLASH_UTIL_H__
#define __FLASH_UTIL_H__

#include "JUTIL.h"
//用户根据自己的需要设置
#define CodeSize 29000
//1.Flash内存范围是 0x0800 0000 ~ 0x0802 0000  ，
//2.要注意MCU一页的大小，一些MCU一页是1KB，一些MCU一页是2KB
//3.这里Flash写入的地址必须是偶数（ 如果是操作16位总线,那么访问时必须按偶数地址对齐。 否则就不一定受这样的限制。例如对串行Flash进行读操作是可以对奇地址进行单字节读取的。）


//写入的地址（去掉了代码大小和起始地址的），写入类型,写入数据，写入的长度
en_result_t wirteFlashWORD(uint32  writeFlashAddr,uint32 *writeFlashData,u16 NumToWrite,uint8 isSectorErase);

void STMFLASH_Read_HALFWORD(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);
void STMFLASH_Read_WORD(u32 ReadAddr,u32 *pBuffer,u16 NumToRead);
void STMFLASH_Read_DOUBLEWORD(u32 ReadAddr,u64 *pBuffer,u16 NumToRead);
void STMFLASH_Read_WORD_Float(u32 ReadAddr,float *pBuffer,u16 NumToRead);
void flashUtilInit(void);

#ifdef __cplusplus
extern "C" {
#endif
#endif













