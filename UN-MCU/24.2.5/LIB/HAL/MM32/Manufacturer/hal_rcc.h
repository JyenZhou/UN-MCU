/***********************************************************************************************************************
    @file     hal_rcc.h
    @author   VV TEAM
    @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE SERIES OF
              MM32 FIRMWARE LIBRARY.
  **********************************************************************************************************************
    @attention

    <h2><center>&copy; Copyright(c) <2023> <MindMotion></center></h2>

      Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
    following conditions are met:
    1. Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
       the following disclaimer in the documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or
       promote products derived from this software without specific prior written permission.

      THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
    INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
    WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *********************************************************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HAL_RCC_H
#define __HAL_RCC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Files includes ------------------------------------------------------------*/
#include "mm32_device.h"

/** @addtogroup MM32_Hardware_Abstract_Layer
  * @{
  */

/** @defgroup RCC_HAL
  * @brief RCC HAL modules
  * @{
  */

/** @defgroup RCC_Exported_Types
  * @{
  */

/** @defgroup RCC_Exported_Constants
  * @{
  */

/**
  * @}
  */


/** @defgroup RCC_Exported_Enumeration
  * @{
  */

/**
  * @brief HSE configuration
  */
typedef enum {
    RCC_HSE_OFF                         = 0,                                    /*!< HSE OFF */
    RCC_HSE_ON                          = RCC_CR_HSEON,                         /*!< HSE ON */
    RCC_HSE_Bypass                      = RCC_CR_HSEBYP                         /*!< HSE Bypass */
} RCCHSE_TypeDef;

/**
  * @brief Used for flags
  */
typedef enum {
    CR_REG_INDEX                        = 1,     //
    BDCR_REG_INDEX                      = 2,     //
    CSR_REG_INDEX                       = 3,     //
    RCC_FLAG_MASK                       = 0x1FU  //
} RCC_RegisterFlag_TypeDef;

/**
  * @brief RCC Flag
  */
typedef enum {
    // Flags in the CR register
    RCC_FLAG_HSIRDY = ((uint8_t)((CR_REG_INDEX << 5U) | RCC_CR_HSIRDY_Pos)),         /*!< Internal High Speed clock ready flag */
    RCC_FLAG_HSERDY = ((uint8_t)((CR_REG_INDEX << 5U) | RCC_CR_HSERDY_Pos)),         /*!< External High Speed clock ready flag */

    RCC_FLAG_PLLRDY     = 0,

    // Flags in the CSR register
    RCC_FLAG_LSIRDY  = ((uint8_t)((CSR_REG_INDEX << 5U) | RCC_CSR_LSIRDY_Pos)),      /*!< Internal Low Speed oscillator Ready */
    RCC_FLAG_PINRST  = ((uint8_t)((CSR_REG_INDEX << 5U) | RCC_CSR_PINRSTF_Pos)),     /*!< PIN reset flag */
    RCC_FLAG_PORRST  = ((uint8_t)((CSR_REG_INDEX << 5U) | RCC_CSR_PORRSTF_Pos)),     /*!< POR/PDR reset flag */
    RCC_FLAG_SFTRST  = ((uint8_t)((CSR_REG_INDEX << 5U) | RCC_CSR_SFTRSTF_Pos)),     /*!< Software Reset flag */
    RCC_FLAG_IWDGRST = ((uint8_t)((CSR_REG_INDEX << 5U) | RCC_CSR_IWDGRSTF_Pos)),    /*!< Independent Watchdog reset flag */

} RCC_FLAG_TypeDef;

/**
  * @brief System clock source
  */
typedef enum {
    RCC_HSI                             = 0,                                    /*!< Set HSI as systemCLOCK */
    RCC_HSE                             = 1,                                    /*!< Set HSE as systemCLOCK */
    RCC_HSI48MHZ                        = 2,                                    /*!< Set HSI48MHZ as systemCLOCK */
    RCC_LSI                             = 3                                     /*!< Set LSI as systemCLOCK */
} SYSCLK_TypeDef;

/**
  * @brief PLL entry clock source
  */
typedef enum {
    RCC_HSI_Div4 = 0,
    RCC_HSE_Div1        = 0,
    RCC_HSE_Div2        = 0
} RCC_PLLSource_TypeDef;
/**
  * @brief PLL multiplication factor
  */

/**
  * @brief AHB clock source
  */
typedef enum {
    RCC_SYSCLK_Div1                     = RCC_CFGR_HPRE_DIV1,
    RCC_SYSCLK_Div2                     = RCC_CFGR_HPRE_DIV2,
    RCC_SYSCLK_Div4                     = RCC_CFGR_HPRE_DIV4,
    RCC_SYSCLK_Div8                     = RCC_CFGR_HPRE_DIV8,
    RCC_SYSCLK_Div16                    = RCC_CFGR_HPRE_DIV16,
    RCC_SYSCLK_Div64                    = RCC_CFGR_HPRE_DIV64,
    RCC_SYSCLK_Div128                   = RCC_CFGR_HPRE_DIV128,
    RCC_SYSCLK_Div256                   = RCC_CFGR_HPRE_DIV256,
    RCC_SYSCLK_Div512                   = RCC_CFGR_HPRE_DIV512
} RCC_AHB_CLK_TypeDef;

/**
  * @brief APB1  source
  */
typedef enum {
    RCC_HCLK_Div1                       = RCC_CFGR_PPRE1_DIV1,
    RCC_HCLK_Div2                       = RCC_CFGR_PPRE1_DIV2,
    RCC_HCLK_Div4                       = RCC_CFGR_PPRE1_DIV4,
    RCC_HCLK_Div8                       = RCC_CFGR_PPRE1_DIV8,
    RCC_HCLK_Div16                      = RCC_CFGR_PPRE1_DIV16
} RCC_APB1_CLK_TypeDef;

/**
  * @brief USB Device clock source
  */

/**
  * @brief ADC clock source
  */
typedef enum {
    RCC_PCLK2_Div2                      = (0x00000000),
    RCC_PCLK2_Div4                      = (0x00004000),
    RCC_PCLK2_Div6                      = (0x00008000),
    RCC_PCLK2_Div8                      = (0x0000C000)
} RCC_ADCCLKSOURCE_TypeDef;

/**
  * @brief LSE configuration
  */

/**
  * @brief Clock source to output on MCO pin
  */
typedef enum {
    RCC_MCO_NoClock                     = RCC_CFGR_MCO_NOCLOCK,
    RCC_MCO_LSI                         = RCC_CFGR_MCO_LSI,
    RCC_MCO_SYSCLK                      = RCC_CFGR_MCO_SYSCLK,
    RCC_MCO_HSI_DIV4                    = RCC_CFGR_MCO_HSI_DIV4,
    RCC_MCO_HSE                         = RCC_CFGR_MCO_HSE,
} RCC_MCO_TypeDef;

/**
  * @brief RCC Interrupt source
  */
typedef enum {
    RCC_IT_LSIRDY                       = RCC_CIR_LSIRDYF,
    RCC_IT_LSERDY                       = 0,
    RCC_IT_HSIRDY                       = RCC_CIR_HSIRDYF,
    RCC_IT_HSERDY                       = RCC_CIR_HSERDYF,
    RCC_IT_PLLRDY                       = 0,
    RCC_IT_CSS                          = RCC_CIR_CSSF
} RCC_IT_TypeDef;

/**
  * @brief RCC clock frequency type definition
  */
typedef struct {
    uint32_t SYSCLK_Frequency;                                                       /*!< returns SYSCLK clock frequency. */
    uint32_t HCLK_Frequency;                                                         /*!< returns hclk clock frequency. */
    uint32_t PCLK1_Frequency;                                                        /*!< returns PCLK1 clock frequency. */
    uint32_t PCLK2_Frequency;                                                        /*!< returns PCLK2 clock frequency. */
    uint32_t ADCCLK_Frequency;                                                       /*!< returns ADCCLK clock frequency. */
} RCC_ClocksTypeDef;
/**
  * @}
  */

/**
  * @}
  */

/** @defgroup RCC_Exported_Variables
  * @{
  */
#ifdef _HAL_RCC_C_

#define GLOBAL
#else
#define GLOBAL extern
#endif

#undef GLOBAL
/**
  * @}
  */

/** @defgroup RCC_Exported_Functions
  * @{
  */
void RCC_DeInit(void);
void RCC_HSEConfig(RCCHSE_TypeDef state);
void RCC_HSICmd(FunctionalState state);
void RCC_SYSCLKConfig(SYSCLK_TypeDef sys_clk_src);
void RCC_HCLKConfig(RCC_AHB_CLK_TypeDef sys_clk);
void RCC_PCLK1Config(RCC_APB1_CLK_TypeDef hclk);
void RCC_ADCCLKConfig(RCC_ADCCLKSOURCE_TypeDef pclk2);
void RCC_LSICmd(FunctionalState state);

void RCC_GetClocksFreq(RCC_ClocksTypeDef* clk);
void RCC_AHBPeriphClockCmd(uint32_t ahb_periph, FunctionalState state);
void RCC_APB1PeriphClockCmd(uint32_t apb1_periph, FunctionalState state);
void RCC_APB1PeriphResetCmd(uint32_t apb1_periph, FunctionalState state);

void RCC_ClockSecuritySystemCmd(FunctionalState state);
void RCC_MCOConfig(RCC_MCO_TypeDef mco_src);
void RCC_ClearFlag(void);
void RCC_ITConfig(RCC_IT_TypeDef it, FunctionalState state);
void RCC_ClearITPendingBit(uint8_t it);

uint8_t  RCC_GetSYSCLKSource(void);
uint32_t RCC_GetSysClockFreq(void);
uint32_t RCC_GetHCLKFreq(void);

uint32_t RCC_GetPCLK1Freq(void);
FlagStatus  RCC_GetFlagStatus(RCC_FLAG_TypeDef flag);
ErrorStatus RCC_WaitForHSEStartUp(void);
ErrorStatus RCC_WaitForFlagStartUp(RCC_FLAG_TypeDef flag);
ITStatus    RCC_GetITStatus(RCC_IT_TypeDef it);


/* Extended function interface -----------------------------------------------*/
//ErrorStatus exRCC_Init(RCCInitStruct_TypeDef* para);
void exRCC_SystickDisable(void);
void exRCC_SystickEnable(uint32_t sys_tick_period);
void exRCC_APB1PeriphReset(uint32_t apb1_periph);
void RCC_ADC_ClockCmd(ADC_TypeDef* peripheral, FunctionalState state);

void RCC_APB1PeriphReset(uint32_t apb1_periph);
void RCC_AHBPeriphReset(uint32_t ahb1_periph);
void exRCC_AHBPeriphReset(uint32_t ahb1_periph);
void RCC_AHBPeriphResetCmd(uint32_t ahb_periph, FunctionalState state);
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

/** --------------------------------------------------------------------------*/
#endif /* __HAL_RCC_H --------------------------------------------------------*/
/** --------------------------------------------------------------------------*/

