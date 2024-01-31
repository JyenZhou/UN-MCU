/*
 * @Description:
 * @Author: wangshi@tinychip.com.cn
 * @Date: 2023-02-06 19:11:11
 * @Disclaimer:
 *  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 *  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 *  * TIME. AS A RESULT, TINY CHIP SHALL NOT BE HELD LIABLE FOR ANY
 *  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 *  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 *  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 *  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 *  * Copyright (c) 2020 Tinychip Microelectronics Co.,Ltd, All Rights Reserved.
 */
#ifndef __TC04XX_INTERRUPT_H__
#define __TC04XX_INTERRUPT_H__
#include "tc04xx.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif


#define TC04XX_IRQ_LEVEL_DEFAULT                   3u

#define FLASH_ERROR_IRQHandler(void)              IRQ000_Handler(void)
#define FLASH_PROG_IRQHandler(void)               IRQ001_Handler(void)
#define FLASH_ERASE_IRQHandler(void)              IRQ002_Handler(void)
#define I2C0_IRQHandler(void)                     IRQ003_Handler(void)
#define UART0_IRQHandler(void)                    IRQ004_Handler(void)
#define GPT0_IRQHandler(void)                     IRQ005_Handler(void)
#define GPT1_IRQHandler(void)                     IRQ006_Handler(void)
#define GPT2_IRQHandler(void)                     IRQ007_Handler(void)
#define GPT3_IRQHandler(void)                     IRQ008_Handler(void)
#define GPT4_IRQHandler(void)                     IRQ009_Handler(void)
#define WDG_OVF_IRQHandler(void)                  IRQ010_Handler(void)
#define CAPTOUCH_IRQHandler(void)                 IRQ011_Handler(void)
#define LPDMA_IRQHandler(void)                    IRQ012_Handler(void)
#define SARADC_CMP_IRQHandler(void)               IRQ013_Handler(void)
#define SARADC_DONE_IRQHandler(void)              IRQ014_Handler(void)
#define SARADC_OVF_IRQHandler(void)               IRQ015_Handler(void)
#define SARADC_RX_IRQHandler(void)                IRQ016_Handler(void)
#define SARADC_UNF_IRQHandler(void)               IRQ017_Handler(void)
#define GPIOA_IRQHandler(void)                    IRQ018_Handler(void)
#define RTC_IRQHandler(void)                      IRQ019_Handler(void)
#define LVD_IRQHandler(void)                      IRQ020_Handler(void)
#define LVD15_IRQHandler(void)                    IRQ021_Handler(void)
#define SPI_IRQHandler(void)                      IRQ022_Handler(void)
#define GPIOB_IRQHandler(void)                    IRQ023_Handler(void)



void EnableNvic(uint32_t u32Irq, uint8_t u8Level, boolean_t bEn);




#ifdef __cplusplus
}
#endif
#endif


