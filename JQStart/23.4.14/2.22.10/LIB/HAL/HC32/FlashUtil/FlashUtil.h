#ifndef __FLASH_UTIL_H__
#define __FLASH_UTIL_H__

#include "JUTIL.h"
//�û������Լ�����Ҫ����
#define CodeSize 29000
//1.Flash�ڴ淶Χ�� 0x0800 0000 ~ 0x0802 0000  ��
//2.Ҫע��MCUһҳ�Ĵ�С��һЩMCUһҳ��1KB��һЩMCUһҳ��2KB
//3.����Flashд��ĵ�ַ������ż���� ����ǲ���16λ����,��ô����ʱ���밴ż����ַ���롣 ����Ͳ�һ�������������ơ�����Դ���Flash���ж������ǿ��Զ����ַ���е��ֽڶ�ȡ�ġ���


//д��ĵ�ַ��ȥ���˴����С����ʼ��ַ�ģ���д������,д�����ݣ�д��ĳ���
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













