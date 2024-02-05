/*
 * @Description: 
 * @Author: wangshi@tinychip.com.cn
 * @Date: 2023-01-29 13:26:40
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
#ifndef __TC04XX_RTC_H__
#define __TC04XX_RTC_H__
#include "tc04xx.h"

#if defined(__cplusplus)
extern "C" {
#endif /*_cplusplus*/

//RTC TRIG TYPE
#define                      RTC_TRIG_WAVE                                0x01U
#define                      RTC_TRIG_0                                   0x02U
#define                      RTC_TRIG_1                                   0x04U
#define                      RTC_TRIG_2                                   0x08U

//RTC CYCLE SET
#define                      RTC_CYCLE_8_MS                               0x80U
#define                      RTC_CYCLE_16_MS                              0x100U
#define                      RTC_CYCLE_32_MS                              0x200U
#define                      RTC_CYCLE_64_MS                              0x400U
#define                      RTC_CYCLE_128_MS                             0x800U
#define                      RTC_CYCLE_256_MS                             0x1000U

//RTC LOCK/PASSWORD
#define     RTC_LOCK_VALUE            (0x12345678)
#define     RTC_UNLOCK_VALUE          (0xAAAA5555)

#define     RTC_LOCK()                (RTC->RTC_LOCK = RTC_LOCK_VALUE)

#define     RTC_UNLOCK()              (RTC->RTC_LOCK = RTC_UNLOCK_VALUE)

#define     RTC_RESUME()              (RTC->RTC_CTRL_0_F.RTC_HOLD = 0x00)

#define     RTC_PAUSE()               (RTC->RTC_CTRL_0_F.RTC_HOLD = 0x01)

#define     RTC_WAVE_ENABLE()         (RTC->RTC_CTRL_0_F.SW_WAVE_EN = 0x01)

#define     RTC_WAVE_DISABLE()        (RTC->RTC_CTRL_0_F.SW_WAVE_EN = 0x00)

#define     RTC_IT_ENABLE(mask)       (RTC->RTC_IMR_F.RTC_IMR &= ~(mask))

#define     RTC_IT_DISABLE(mask)      (RTC->RTC_IMR_F.RTC_IMR |= (mask))

#define     RTC_IT_FLAG_GET(mask)     ((RTC->RTC_ISR_F.RTC_ISR & (mask))?1:0)

#define     RTC_IT_FLAG_CLR(mask)     (RTC->RTC_ICR_F.RTC_ICR = (mask))

void RTC_SetFclkSource(uint8_t clk_src,uint8_t clk_div);
void RTC_Flush_Setting(void);
void RTC_WaveConfig(uint8_t Wave_Cnt,uint8_t Output_En);
void RTC_Trigger_Config(uint8_t Trigger_ID,uint16_t Trigger_Cnt);
void RTC_Trigger_Enable(uint8_t Trigger_ID);
void RTC_Trigger_Disable(uint8_t Trigger_ID);
void RTC_Config(uint8_t Trigger_ID,uint8_t rtc_cycle);

#ifdef __cplusplus
}
#endif
#endif
