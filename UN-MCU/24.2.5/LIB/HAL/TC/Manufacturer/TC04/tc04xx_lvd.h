/*
 * @Description: 
 * @Author: wangshi@tinychip.com.cn
 * @Date: 2023-01-28 17:21:47
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
#ifndef __TC04XX_LVD_H__
#define __TC04XX_LVD_H__
#include "tc04xx.h"

 /* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

#define                      LVD_TRIP_LEVEL0                              0x01U//rise 2.8V fall 2.9V
#define                      LVD_TRIP_LEVEL1                              0x02U//rise 3.1V fall 3.2V
#define                      LVD_TRIP_LEVEL2                              0x04U//rise 3.7V fall 3.9V
#define                      LVD_TRIP_LEVEL3                              0x08U//rise 4.4V fall 4.6V

#define                      LVD_IT_RISING                                0x01U
#define                      LVD_IT_FALLING                               0x02U
#define                      LVD_IT_LOW_LEVEL                             0x04U


#define     LVD_IT_CFG(u8mask)                  (ANAREG->LVD_CFG_F.LVD_IRQ_MODE = (u8mask))
#define     LVD15_IT_CFG(u8mask)                (ANAREG->LVD_CFG_F.LVD15_IRQ_MODE = (u8mask))

#define     LVD_ENABLE()                        (ANAREG->LVD_TOP_F.CTRL_LVD5_0_ENABLE = ENABLE)
#define     LVD_DISABLE()                       (ANAREG->LVD_TOP_F.CTRL_LVD5_0_ENABLE = DISABLE)

#define     LVD_FILTER_CFG(u16mask)             (ANAREG->LVD_CFG_F.LVD_FILTER = (u16mask&0xFFFFU))
#define     LVD_FILTER_ENABLE()                 (ANAREG->LVD_CFG_F.LVD_FILTER_EN = ENABLE)
#define     LVD_FILTER_DISABLE()                (ANAREG->LVD_CFG_F.LVD_FILTER_EN = DISABLE)

void LVD_SetFclkSource(uint8_t clk_src,uint16_t clk_div);
void LVD_Config(uint8_t volt_level,uint8_t high_power_en,uint8_t rst_en);



#ifdef __cplusplus
}
#endif
#endif
