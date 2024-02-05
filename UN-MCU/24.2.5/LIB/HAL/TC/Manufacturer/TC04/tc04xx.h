/*
 * @Description: 
 * @Author: wangshi@tinychip.com.cn
 * @Date: 2023-01-13 14:53:44
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
#ifndef __tc04xx_H__
#define __tc04xx_H__

#include "base_types.h"

#if defined(__cplusplus)
extern "C" {
#endif /*_cplusplus*/

typedef enum IRQn
{
 /******  Cortex-M# Processor Exceptions Numbers ***************************************************/

 /* ToDo: use this Cortex interrupt numbers if your device is a CORTEX-M4 device                   */
    NonMaskableInt_IRQn           = -14,      /*!<  2 Non Maskable Interrupt                        */
    HardFault_IRQn                = -13,      /*!<  3 Hard Fault Interrupt                          */
    SVCall_IRQn                   = -5,       /*!< 11 SV Call Interrupt                             */
    PendSV_IRQn                   = -2,       /*!< 14 Pend SV Interrupt                             */
    SysTick_IRQn                  = -1,       /*!< 15 System Tick Interrupt                         */

 /******  Device Specific Interrupt Numbers ********************************************************/
 /* ToDo: add here your device specific external interrupt numbers
          according the interrupt handlers defined in startup_Device.s
          eg.: Interrupt for Timer#1       TIM1_IRQHandler   ->   TIM1_IRQn                        */

    Flash_Error_IRQn               = 0,    //error    hclk
    Flash_Prog_IRQn                = 1,    //Program  hclk
    Flash_Erase_IRQn               = 2,    //Erase    hclk
    I2c_IRQn                       = 3,    //rx/tx..   pclk
    Uart0_IRQn                     = 4,    //overflow/error/timeout/busy   pclk
    GPT0_IRQn                      = 5,    //    pclk
    GPT1_IRQn                      = 6,    //    pclk
    GPT2_IRQn                      = 7,    //    pclk
    GPT3_IRQn                      = 8,    //    pclk
    GPT4_IRQn                      = 9,     
    Wdg_Ovf_IRQn                   = 10,   //WDG counter overflow   fclk
    Captouch_IRQn                  = 11,   //CAPTOUCH detect/trig done/fifo_af/max_cnt   fclk
    Lpdma_IRQn                     = 12,   //DMA sart/stop/end/error hclk
    Saradc_Cmp_IRQn                = 13,   //ADC ouput middle/lower/higher   fclk
    Saradc_Done_IRQn               = 14,   //ADC trigger done   fclk
    Saradc_Ovf_IRQn                = 15,   //ADC fifo full   fclk
    Saradc_Rx_IRQn                 = 16,   //ADC fifo rx rdy when dma/trig not enabled   pclk
    Saradc_Unf_IRQn                = 17,   //ADC fifo empty    pclk
    Gpioa_IRQn                     = 18,   //GPIOA event   fclk
    Rtc_IRQn                       = 19,   //rtc cnt/trigger overflow   fclk
    Lvd_IRQn                       = 20,   //VDD low voltage    clk32k
    Lvd15_IRQn                     = 21,   //LDO15 low voltage    clk32k
    SPI_IRQn                       = 22,   //SPI   fclk
    Gpiob_IRQn                     = 23,   //GPIOB event   fclk
} IRQn_Type;

/************************************************************************/
/*  Processor and Core Peripheral Section                               */
/************************************************************************/
/* -------  Start of section using anonymous unions and disabling warnings  ------- */
#if   defined (__CC_ARM)
#pragma push
#pragma anon_unions
#elif defined (__ICCARM__)
#pragma language=extended
#elif defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc11-extensions"
#pragma clang diagnostic ignored "-Wreserved-id-macro"
#elif defined (__GNUC__)
/* anonymous unions are enabled by default */
#elif defined (__TMS470__)
/* anonymous unions are enabled by default */
#elif defined (__TASKING__)
#pragma warning 586
#elif defined (__CSMC__)
/* anonymous unions are enabled by default */
#else
#warning Not supported compiler type
#endif
/* --------  Configuration of Core Peripherals  ----------------------------------- */
#define __CM0_REV                 0x0000U   /* Core revision r0p0 */
#define __MPU_PRESENT             0U        /* no MPU present */
#define __VTOR_PRESENT            0U        /* no VTOR present */
#define __NVIC_PRIO_BITS          2U        /* Number of Bits used for Priority Levels */
#define __Vendor_SysTickConfig    0U        /* Set to 1 if different SysTick Config is used */

#include <core_cm0.h>


#define CLOCK_SEL_16M            0x00U
#define CLOCK_SEL_32M            0x01U
#define CLOCK_SEL_32K            0x02U      
#define CLOCK_SEL_32K_2          0x03U        
#define DEFAULT_SYSTEM_CLOCK     (16000000UL)



#include "aanareg_reg_def.h"
#include "aclkrst_reg_def.h"
#include "adc_reg_def.h"
#include "apinmux_reg_def.h"
#include "captouch_reg_def.h"
#include "eflash_reg_def.h"
#include "gpioa_reg_def.h"
#include "gpiob_reg_def.h"
#include "gpt_reg_def.h"
#include "i2c_reg_def.h"
#include "pdma_reg_def.h"
#include "pmu_reg_def.h"
#include "print_uart_reg_def.h"
#include "rtc_reg_def.h"
#include "spi_reg_def.h"
#include "pdma_reg_def.h"
#include "syscfg_reg_def.h"
#include "uart_reg_def.h"
#include "wdg_reg_def.h"


/************************************************************************/
/*  APB address config                                                  */
/************************************************************************/

#define PERIPH_BASE                 ((unsigned int)0x40000000U)
#define PSRAM_AHB_BASE              ((unsigned int)0x60000000U)
#define APB0_PERIPH_BASE            (PERIPH_BASE)
#define APB1_PERIPH_BASE            (PERIPH_BASE+0x100000U)

/************************************************************************/
/*  APB0                                                                */
/************************************************************************/
#define PINMUX_BASE_ADDR            (APB0_PERIPH_BASE + 0x1000U)
#define CLKRST_BASE_ADDR            (APB0_PERIPH_BASE + 0x2000U)
#define PMU_BASE_ADDR               (APB0_PERIPH_BASE + 0x3000U)
#define ANAREG_BASE_ADDR            (APB0_PERIPH_BASE + 0x4000U)
#define RTC_BASE_ADDR               (APB0_PERIPH_BASE + 0x5000U)
#define RESEREVED1                  (APB0_PERIPH_BASE + 0x6000U)
#define REMAP_EN_BASE_ADDR          (APB0_PERIPH_BASE + 0xFFF8U)
#define TEST_MODE_BASE_ADDR         (APB0_PERIPH_BASE + 0xFFFCU)
#define IIC_BASE_ADDR               (APB0_PERIPH_BASE + 0x10000U)
#define UART0_BASE_ADDR             (APB0_PERIPH_BASE + 0x20000U)
#define UART1_BASE_ADDR             (APB0_PERIPH_BASE + 0x30000U)
#define WDG_BASE_ADDR               (APB0_PERIPH_BASE + 0x40000U)
#define SYSCFG_BASE_ADDR            (APB0_PERIPH_BASE + 0x50000U)
#define SARADC_BASE_ADDR            (APB0_PERIPH_BASE + 0x60000U)
#define CAPTOUCH_BASE_ADDR          (APB0_PERIPH_BASE + 0x70000U)
#define SPI_BASE_ADDR               (APB0_PERIPH_BASE + 0x80000U)
#define PDMA_BASE_ADDR              (APB0_PERIPH_BASE + 0x90000U)

/************************************************************************/
/*  APB1                                                                */
/************************************************************************/
#define GPIOA_BASE_ADDR             (APB1_PERIPH_BASE)
#define GPIOB_BASE_ADDR             (APB1_PERIPH_BASE + 0x10000U)
#define GPT0_BASE_ADDR              (APB1_PERIPH_BASE + 0x20000U)
#define GPT1_BASE_ADDR              (APB1_PERIPH_BASE + 0x30000U)
#define GPT2_BASE_ADDR              (APB1_PERIPH_BASE + 0x40000U)
#define GPT3_BASE_ADDR              (APB1_PERIPH_BASE + 0x50000U)
#define GPT4_BASE_ADDR              (APB1_PERIPH_BASE + 0x60000U)


#define PINMUX                      ((APINMUX_REG_TypeDef *)       PINMUX_BASE_ADDR) 
#define CLKRST                      ((ACLKRST_REG_TypeDef *)       CLKRST_BASE_ADDR) 
#define PMU                         ((PMU_REG_TypeDef *)           PMU_BASE_ADDR) 
#define ANAREG                      ((AANAREG_REG_TypeDef *)       ANAREG_BASE_ADDR) 
#define RTC                         ((RTC_REG_TypeDef *)           RTC_BASE_ADDR) 
//remap en 
//test mode
#define IIC                         ((I2C_REG_TypeDef *)           IIC_BASE_ADDR) 
#define UART0                       ((UART_REG_TypeDef *)          UART0_BASE_ADDR) 
#define UART1                       ((PRINT_UART_REG_TypeDef *)    UART1_BASE_ADDR) 
#define WDG                         ((WDG_REG_TypeDef *)           WDG_BASE_ADDR) 
#define SYSCFG                      ((SYSCFG_REG_TypeDef *)        SYSCFG_BASE_ADDR) 
#define SARADC                      ((ADC_REG_TypeDef *)           SARADC_BASE_ADDR) 
#define TOUCH                       ((CAPTOUCH_REG_TypeDef *)      CAPTOUCH_BASE_ADDR)
#define SPI                         ((SPI_REG_TypeDef *)           SPI_BASE_ADDR)
#define DMA                         ((PDMA_REG_TypeDef *)          PDMA_BASE_ADDR)

#define GPIOA                       ((GPIOA_REG_TypeDef *)         GPIOA_BASE_ADDR)
#define GPIOB                       ((GPIOB_REG_TypeDef *)         GPIOB_BASE_ADDR)
#define TIMER0                      ((GPT_REG_TypeDef *)           GPT0_BASE_ADDR)
#define TIMER1                      ((GPT_REG_TypeDef *)           GPT1_BASE_ADDR)
#define TIMER2                      ((GPT_REG_TypeDef *)           GPT2_BASE_ADDR)
#define TIMER3                      ((GPT_REG_TypeDef *)           GPT3_BASE_ADDR)
#define TIMER4                      ((GPT_REG_TypeDef *)           GPT4_BASE_ADDR)

#define FLASH_CFG_ADDR              (0x10FF0000U)
#define FLASH_NVR_ADDR              (0x10800000U)
#define FLASH_ADDR                  (0x10000000U)

#define FLASH_SIZE                  (0x10000U)
#define FLASH_NVR_SIZE              (0x1000U)
#define FLASH_SECTOR_SIZE           (0x200U)

#define FLASH                       ((EFLASH_REG_TypeDef *)         FLASH_CFG_ADDR)


/* --------  End of section using anonymous unions and disabling warnings  -------- */
#if   defined (__CC_ARM)
#pragma pop
#elif defined (__ICCARM__)
/* leave anonymous unions enabled */
#elif (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
#pragma clang diagnostic pop
#elif defined (__GNUC__)
/* anonymous unions are enabled by default */
#elif defined (__TMS470__)
/* anonymous unions are enabled by default */
#elif defined (__TASKING__)
#pragma warning restore
#elif defined (__CSMC__)
/* anonymous unions are enabled by default */
#else
#warning Not supported compiler type
#endif

#if defined(__cplusplus)
}

#endif


#endif






