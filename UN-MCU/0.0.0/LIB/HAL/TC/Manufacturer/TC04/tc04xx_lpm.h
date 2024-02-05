/*
 * @Description: 
 * @Author: wangshi@tinychip.com.cn
 * @Date: 2023-02-02 15:13:11
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
#ifndef __TC04XX_LPM_H__
#define __TC04XX_LPM_H__
#include "tc04xx.h"


 /* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

#define                      ACTIVE_MODE                                  0x00U
#define                      SLEEPWALK_MODE                               0x01U
#define                      SLEEP_MODE                                   0x02U
#define                      HIBERBNATE_MODE                              0x03U

#define                      PMU_LVD_MASK                                 0x01U
#define                      PMU_LVD15_MASK                               0x02U

#define                      RTC_TRIG0_SARADC_MASK                        0x1U
#define                      RTC_TRIG1_CAPTOUCH_MASK                      0x2U
#define                      RTC_TRIG2_CAPTOUCH_MASK                      0x4U


#define                     PMU_WAKE_ENABLE(u8Mask)                       (PMU->AWKUP_CTRL_F.AON_IMR &= ~(u8Mask))
#define                     PMU_WAKE_DISABLE(u8Mask)                      (PMU->AWKUP_CTRL_F.AON_IMR |= (u8Mask))

#define                     PMU_TINYWORK_TRIG_ENABLE(u8Mask)              (PMU->TW_CTRL_F.SW_TW_EN |= (u8Mask))
#define                     PMU_TINYWORK_TRIG_DISABLE(u8Mask)             (PMU->TW_CTRL_F.SW_TW_EN &= ~(u8Mask))


#define                     PMU_INT_ENABLE(u8Mask)                        (PMU->AWKUP_CTRL_F.AON_IMR &= ~(u8Mask))
#define                     PMU_INT_DISABLE(u8Mask)                       (PMU->AWKUP_CTRL_F.AON_IMR |= (u8Mask))

#define                     PMU_INT_FLAG_CLR(u8Mask)                      (PMU->AWKUP_CTRL_F.AON_ICR = (u8Mask))
#define                     PMU_INT_FLAG_GET(u8Mask)                      ((PMU->AWKUP_CTRL_F.AON_ISR&(u8Mask))?1:0)
#define                     PMU_RAW_FLAG_GET(u8Mask)                      ((PMU->AWKUP_CTRL_F.AON_IRSR&(u8Mask))?1:0)




void lpmPmuEnterState(uint8_t state, uint8_t bOnExit);



#ifdef __cplusplus
}
#endif
#endif

