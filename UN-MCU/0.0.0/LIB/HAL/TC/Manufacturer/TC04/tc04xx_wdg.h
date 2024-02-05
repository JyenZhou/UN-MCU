/*
 * @Description: 
 * @Author: wangshi@tinychip.com.cn
 * @Date: 2023-01-28 17:21:58
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
#ifndef __TC04XX_WDG_H__
#define __TC04XX_WDG_H__
#include "tc04xx.h"



 /* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

#define                      WDG_INT_MODE                                 0x00U
#define                      WDG_RST_MODE                                 0x01U


#define     WDG_DEFAULT_LOCK_KEY    (0x12345678)

#define     WDG_DEFAULT_UNLOCK_KEY  (0xaaaa5555)

#define     WDG_ENABLE()        (WDG->CTRL_F.WE = ENABLE)

#define     WDG_DISABLE()       (WDG->CTRL_F.WE = DISABLE)

#define     WDG_RST_CNT()       (WDG->CCR_F.CCR = 0x01)

#define     WDG_OVF_FLAG_CLR()  (WDG->ICR_F.COI = 0x01)

#define     WDG_OVF_FLAG_GET()  (WDG->STATUS_F.IOVF & 0x1)

#define     WDG_CNT_GET()       (WDG->STATUS_F.WCNT & 0xFFF)

#define     WDG_LOCK()          (WDG->LOCK_F.PASSWORD = WDG_DEFAULT_LOCK_KEY)

#define     WDG_UNLOCK()        (WDG->LOCK_F.PASSWORD = WDG_DEFAULT_UNLOCK_KEY)

void WDG_SetFclkSource(uint8_t clk_src,uint8_t PrescalerValue);
void WDG_Init(uint16_t loadVal,uint8_t mode);
#ifdef __cplusplus
}
#endif
#endif

