/*
 * @Description: 
 * @Author: wangshi@tinychip.com.cn
 * @Date: 2023-01-30 15:35:07
 * @Disclaimer: 
 * 	* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * 	* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * 	* TIME. AS A RESULT, TINY CHIP SHALL NOT BE HELD LIABLE FOR ANY
 * 	* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * 	* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * 	* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * 	* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 * 
 * 	* Copyright (c) 2020 Tinychip Microelectronics Co.,Ltd, All Rights Reserved. 
 */
#ifndef __TC04XX_FLASH_H__
#define __TC04XX_FLASH_H__
#include "tc04xx.h"


 /* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

#define   FLASH_READ_TIME_CFG(cyc)        (FLASH->RCTC_F.CONFIG_CYCLE_OF_RC = (cyc))
#define   FLASH_SETUP_TIME_CFG(cyc)       (FLASH->RCTC_F.CONFIG_CYCLE_OF_CFS = (cyc))
#define   FLASH_HOLD_TIME_CFG(cyc)        (FLASH->RCTC_F.CONFIG_CYCLE_OF_CFH = (cyc))


void Flash_Init(void);
void Flash_Deinit(void);
void Flash_Erase(uint8_t wIndex);
void Flash_NVR_Erase(uint8_t wIndex);

uint32_t Flash_Read(uint32_t u32Addr);
uint16_t Flash_Read16(uint32_t u32Addr);
void Flash_Write(uint32_t u32Addr, uint32_t u32Data);
void Flash_Write16(uint32_t u32Addr, uint16_t u16Data);

uint32_t Flash_NVR_Read(uint32_t u32Addr);
uint16_t Flash_NVR_Read16(uint32_t u32Addr);
void Flash_NVR_Write(uint32_t u32Addr, uint32_t u32Data);
void Flash_NVR_Write16(uint32_t u32Addr, uint16_t u16Data);

void Flash_NVR_WriteByte(uint32_t addr,uint8_t *pBuf,uint16_t Size);
void Flash_NVR_ReadByte(uint32_t addr,uint8_t *pBuf,uint16_t Size);

void Flash_ReadByte(uint32_t addr,uint8_t *pBuf,uint16_t Size);
void Flash_WriteByte(uint32_t addr,uint8_t *pBuf,uint16_t Size);

#ifdef __cplusplus
}
#endif
#endif

