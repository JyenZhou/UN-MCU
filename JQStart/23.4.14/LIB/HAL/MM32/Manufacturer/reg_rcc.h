/***********************************************************************************************************************
    @file     reg_rcc.h
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

#ifndef __REG_RCC_H
#define __REG_RCC_H

/* Files includes ------------------------------------------------------------*/
#include  "./CMSIS_Creo/core_cm0.h"


#ifdef __cplusplus
extern "C" {
#endif

#if defined(__CC_ARM)
#pragma anon_unions
#elif defined(__ICCARM__)
#pragma language=extended
#elif defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#pragma clang diagnostic ignored "-Wc11-extensions"
#pragma clang diagnostic ignored "-Wreserved-id-macro"
#elif defined(__GNUC__)
/* anonymous unions are enabled by default */
#else
#warning Not supported compiler type
#endif

/**
  * @brief RCC Base Address Definition
  */
#define RCC_BASE                        (AHBPERIPH_BASE + 0x1000)               /*!< Base Address: 0x40021000 */

/**
  * @brief RCC Register Structure Definition
  */
typedef struct {
    __IO uint32_t CR;                                                                /*!< Control Register                               offset: 0x00 */
    __IO uint32_t CFGR;                                                              /*!< Configuration Register                         offset: 0x04 */
    __IO uint32_t CIR;                                                               /*!< Clock Interrupt Register                       offset: 0x08 */
    __IO uint32_t RESERVED0x0C;                                                      /*!< Reserved                                       offset: 0x0C */
    __IO uint32_t APB1RSTR;                                                          /*!< Advanced Peripheral Bus 1 Reset Register       offset: 0x10 */
    __IO uint32_t AHBENR;                                                            /*!< Advanced High Performance Bus Enable Register  offset: 0x14 */
    __IO uint32_t RESERVED0x18;                                                      /*!< Reserved                                       offset: 0x18 */
    __IO uint32_t APB1ENR;                                                           /*!< Advanced Peripheral Bus 1 Enable Register      offset: 0x1C */
    __IO uint32_t RESERVED0x20;                                                      /*!< Reserved                                       offset: 0x20 */
    __IO uint32_t CSR;                                                               /*!< Control Status Register                        offset: 0x24 */
    __IO uint32_t AHBRSTR;                                                           /*!< Advanced High Performance Bus Reset Register   offset: 0x28 */
    __IO uint32_t RESERVED2;                                                         /*!< Reserved                                       offset: 0x2C */
    __IO uint32_t RESERVED0x30;                                                      /*!< Reserved                                       offset: 0x30 */
    __IO uint32_t RESERVED0x34;                                                      /*!< Reserved                                       offset: 0x34 */
    __IO uint32_t RESERVED0x38;                                                      /*!< Reserved                                       offset: 0x38 */
    __IO uint32_t RESERVED0x3C;                                                      /*!< Reserved                                       offset: 0x3C */
    __IO uint32_t SYSCFGR;                                                           /*!< System Configuration Register                  offset: 0x40 */
    __IO uint32_t HSIDLY;                                                            /*!< HSI delay Register                             offset: 0x44 */
    __IO uint32_t HSEDLY;                                                            /*!< HSE delay Register                             offset: 0x48 */
} RCC_TypeDef;

/**
  * @brief RCC type pointer Definition
  */
#define RCC                             ((RCC_TypeDef*) RCC_BASE)

/**
  * @brief RCC_CR Register Bit Definition
  */
#define RCC_CR_HSION_Pos                (0)
#define RCC_CR_HSION                    (0x01U << RCC_CR_HSION_Pos)             /*!< Internal High Speed clock enable */

#define RCC_CR_HSIRDY_Pos               (1)
#define RCC_CR_HSIRDY                   (0x01U << RCC_CR_HSIRDY_Pos)            /*!< Internal High Speed clock ready flag */

#define RCC_CR_HSITEN_Pos               (2)
#define RCC_CR_HSITEN                   (0x01U << RCC_CR_HSITEN_Pos)            /*!< HSI internal temperature enable */

#define RCC_CR_HSEON_Pos                (16)
#define RCC_CR_HSEON                    (0x01U << RCC_CR_HSEON_Pos)             /*!< External High Speed clock enable */
#define RCC_CR_HSERDY_Pos               (17)
#define RCC_CR_HSERDY                   (0x01U << RCC_CR_HSERDY_Pos)            /*!< External High Speed clock ready flag */
#define RCC_CR_HSEBYP_Pos               (18)
#define RCC_CR_HSEBYP                   (0x01U << RCC_CR_HSEBYP_Pos)            /*!< External High Speed clock Bypass */
#define RCC_CR_CSSON_Pos                (19)
#define RCC_CR_CSSON                    (0x01U << RCC_CR_CSSON_Pos)             /*!< Clock Security System enable */

/**
  * @brief RCC_CFGR Register Bit Definition
  */
#define RCC_CFGR_SW_Pos                 (0)
#define RCC_CFGR_SW                     (0x03U << RCC_CFGR_SW_Pos)              /*!< SW[1:0] bits (System clock Switch) */
#define RCC_CFGR_SW_HSI_DIV6            (0x00U << RCC_CFGR_SW_Pos)              /*!< HSI/6 selected as system clock */
#define RCC_CFGR_SW_HSE                 (0x01U << RCC_CFGR_SW_Pos)              /*!< HSE selected as system clock */
#define RCC_CFGR_SW_HSI                 (0x02U << RCC_CFGR_SW_Pos)              /*!< HSI selected as system clock */
#define RCC_CFGR_SW_LSI                 (0x03U << RCC_CFGR_SW_Pos)              /*!< LSI selected as system clock */

#define RCC_CFGR_SWS_Pos                (2)
#define RCC_CFGR_SWS                    (0x03U << RCC_CFGR_SWS_Pos)             /*!< SWS[1:0] bits (System Clock Switch Status) */
#define RCC_CFGR_SWS_HSI_DIV6           (0x00U << RCC_CFGR_SWS_Pos)             /*!< HSI/6 oscillator used as system clock */
#define RCC_CFGR_SWS_HSE                (0x01U << RCC_CFGR_SWS_Pos)             /*!< HSE oscillator used as system clock */
#define RCC_CFGR_SWS_HSI                (0x02U << RCC_CFGR_SWS_Pos)             /*!< HSI used as system clock */
#define RCC_CFGR_SWS_LSI                (0x03U << RCC_CFGR_SWS_Pos)             /*!< LSI used as system clock */

#define RCC_CFGR_HPRE_Pos               (4)
#define RCC_CFGR_HPRE                   (0x0FU << RCC_CFGR_HPRE_Pos)            /*!< HPRE[3:0] bits (AHB prescaler) */
#define RCC_CFGR_PPRE_0                 (0x01U << RCC_CFGR_HPRE_Pos)            /*!< Bit 0 */
#define RCC_CFGR_PPRE_1                 (0x02U << RCC_CFGR_HPRE_Pos)            /*!< Bit 1 */
#define RCC_CFGR_PPRE_2                 (0x04U << RCC_CFGR_HPRE_Pos)            /*!< Bit 2 */
#define RCC_CFGR_PPRE_3                 (0x08U << RCC_CFGR_HPRE_Pos)            /*!< Bit 3 */

#define RCC_CFGR_HPRE_DIV1              (0x00U << RCC_CFGR_HPRE_Pos)            /*!< AHB = FCLK = SYSCLK not divided */
#define RCC_CFGR_HPRE_DIV2              (0x08U << RCC_CFGR_HPRE_Pos)            /*!< AHB = FCLK = SYSCLK divided by 2 */
#define RCC_CFGR_HPRE_DIV4              (0x09U << RCC_CFGR_HPRE_Pos)            /*!< AHB = FCLK = SYSCLK divided by 4 */
#define RCC_CFGR_HPRE_DIV8              (0x0AU << RCC_CFGR_HPRE_Pos)            /*!< AHB = FCLK = SYSCLK divided by 8 */
#define RCC_CFGR_HPRE_DIV16             (0x0BU << RCC_CFGR_HPRE_Pos)            /*!< AHB = FCLK = SYSCLK divided by 16 */
#define RCC_CFGR_HPRE_DIV64             (0x0CU << RCC_CFGR_HPRE_Pos)            /*!< AHB = FCLK = SYSCLK divided by 64 */
#define RCC_CFGR_HPRE_DIV128            (0x0DU << RCC_CFGR_HPRE_Pos)            /*!< AHB = FCLK = SYSCLK divided by 128 */
#define RCC_CFGR_HPRE_DIV256            (0x0EU << RCC_CFGR_HPRE_Pos)            /*!< AHB = FCLK = SYSCLK divided by 256 */
#define RCC_CFGR_HPRE_DIV512            (0x0FU << RCC_CFGR_HPRE_Pos)            /*!< AHB = FCLK = SYSCLK divided by 512 */

#define RCC_CFGR_PPRE1_Pos              (8)
#define RCC_CFGR_PPRE1                  (0x07U << RCC_CFGR_PPRE1_Pos)           /*!< PRE1[2:0] bits (APB1 prescaler) */
#define RCC_CFGR_PPRE1_0                (0x01U << RCC_CFGR_PPRE1_Pos)           /*!< Bit 0 */
#define RCC_CFGR_PPRE1_1                (0x02U << RCC_CFGR_PPRE1_Pos)           /*!< Bit 1 */
#define RCC_CFGR_PPRE1_2                (0x04U << RCC_CFGR_PPRE1_Pos)           /*!< Bit 2 */

#define RCC_CFGR_PPRE1_DIV1             (0x00U << RCC_CFGR_PPRE1_Pos)           /*!< APB1 = HCLK not divided */
#define RCC_CFGR_PPRE1_DIV2             (0x04U << RCC_CFGR_PPRE1_Pos)           /*!< APB1 = HCLK divided by 2 */
#define RCC_CFGR_PPRE1_DIV4             (0x05U << RCC_CFGR_PPRE1_Pos)           /*!< APB1 = HCLK divided by 4 */
#define RCC_CFGR_PPRE1_DIV8             (0x06U << RCC_CFGR_PPRE1_Pos)           /*!< APB1 = HCLK divided by 8 */
#define RCC_CFGR_PPRE1_DIV16            (0x07U << RCC_CFGR_PPRE1_Pos)           /*!< APB1 = HCLK divided by 16 */

#define RCC_CFGR_MCO_Pos                (24)
#define RCC_CFGR_MCO                    (0x07U << RCC_CFGR_MCO_Pos)             /*!< MCO[2:0] bits (Microcontroller Clock Output) */
#define RCC_CFGR_MCO_NOCLOCK            (0x00U << RCC_CFGR_MCO_Pos)             /*!< No clock */
#define RCC_CFGR_MCO_LSI                (0x02U << RCC_CFGR_MCO_Pos)             /*!< LSI clock */
#define RCC_CFGR_MCO_SYSCLK             (0x04U << RCC_CFGR_MCO_Pos)             /*!< System clock selected */
#define RCC_CFGR_MCO_HSI_DIV4           (0x05U << RCC_CFGR_MCO_Pos)             /*!< HSI clock divided by 4 selected */
#define RCC_CFGR_MCO_HSI                (0x05U << RCC_CFGR_MCO_Pos)             /*!< Internal 8 MHz RC oscillator clock selected */
#define RCC_CFGR_MCO_HSE                (0x06U << RCC_CFGR_MCO_Pos)             /*!< External 1-25 MHz oscillator clock selected */

/**
  * @brief RCC_CIR Register Bit Definition
  */
#define RCC_CIR_LSIRDYF_Pos             (0)
#define RCC_CIR_LSIRDYF                 (0x01U << RCC_CIR_LSIRDYF_Pos)          /*!< LSI Ready Interrupt flag */

#define RCC_CIR_HSIRDYF_Pos             (2)
#define RCC_CIR_HSIRDYF                 (0x01U << RCC_CIR_HSIRDYF_Pos)          /*!< HSI Ready Interrupt flag */
#define RCC_CIR_HSERDYF_Pos             (3)
#define RCC_CIR_HSERDYF                 (0x01U << RCC_CIR_HSERDYF_Pos)          /*!< HSE Ready Interrupt flag */

#define RCC_CIR_CSSF_Pos                (7)
#define RCC_CIR_CSSF                    (0x01U << RCC_CIR_CSSF_Pos)             /*!< Clock Security System Interrupt flag */
#define RCC_CIR_LSIRDYIE_Pos            (8)
#define RCC_CIR_LSIRDYIE                (0x01U << RCC_CIR_LSIRDYIE_Pos)         /*!< LSI Ready Interrupt Enable */

#define RCC_CIR_HSIRDYIE_Pos            (10)
#define RCC_CIR_HSIRDYIE                (0x01U << RCC_CIR_HSIRDYIE_Pos)         /*!< HSI Ready Interrupt Enable */
#define RCC_CIR_HSERDYIE_Pos            (11)
#define RCC_CIR_HSERDYIE                (0x01U << RCC_CIR_HSERDYIE_Pos)         /*!< HSE Ready Interrupt Enable */

#define RCC_CIR_LSIRDYC_Pos             (16)
#define RCC_CIR_LSIRDYC                 (0x01U << RCC_CIR_LSIRDYC_Pos)          /*!< LSI Ready Interrupt Clear */

#define RCC_CIR_HSIRDYC_Pos             (18)
#define RCC_CIR_HSIRDYC                 (0x01U << RCC_CIR_HSIRDYC_Pos)          /*!< HSI Ready Interrupt Clear */
#define RCC_CIR_HSERDYC_Pos             (19)
#define RCC_CIR_HSERDYC                 (0x01U << RCC_CIR_HSERDYC_Pos)          /*!< HSE Ready Interrupt Clear */

#define RCC_CIR_CSSC_Pos                (23)
#define RCC_CIR_CSSC                    (0x01U << RCC_CIR_CSSC_Pos)             /*!< Clock Security System Interrupt Clear */

/**
  * @brief RCC_APB1RSTR Register Bit Definition
  */
#define RCC_APB1RSTR_TIM3_Pos           (1)
#define RCC_APB1RSTR_TIM3               (0x01U << RCC_APB1RSTR_TIM3_Pos)        /*!< Timer 2 reset */
#define RCC_APB1RSTR_TIM1_Pos           (2)
#define RCC_APB1RSTR_TIM1               (0x01U << RCC_APB1RSTR_TIM1_Pos)        /*!< Timer 3 reset */
#define RCC_APB1RSTR_TIM14_Pos          (3)
#define RCC_APB1RSTR_TIM14              (0x01U << RCC_APB1RSTR_TIM3_Pos)        /*!< Timer 14 reset */

#define RCC_APB1RSTR_ADC1_Pos           (9)
#define RCC_APB1RSTR_ADC1               (0x01U << RCC_APB1RSTR_ADC1_Pos)        /*!< ADC Configuration register reset */

#define RCC_APB1RSTR_WWDG_Pos           (11)
#define RCC_APB1RSTR_WWDG               (0x01U << RCC_APB1RSTR_WWDG_Pos)        /*!< Window Watchdog reset */
#define RCC_APB1RSTR_SPI1_Pos           (12)
#define RCC_APB1RSTR_SPI1               (0x01U << RCC_APB1RSTR_SPI1_Pos)        /*!< SPI 1 reset */
#define RCC_APB1RSTR_UART1_Pos          (16)
#define RCC_APB1RSTR_UART1              (0x01U << RCC_APB1RSTR_UART1_Pos)       /*!< UART 1 reset */

#define RCC_APB1RSTR_UART2_Pos          (17)
#define RCC_APB1RSTR_UART2              (0x01U << RCC_APB1RSTR_UART2_Pos)       /*!< UART 2 reset */
#define RCC_APB1RSTR_I2C1_Pos           (21)
#define RCC_APB1RSTR_I2C1               (0x01U << RCC_APB1RSTR_I2C1_Pos)        /*!< I2C 1 reset */

#define RCC_APB1RSTR_PWR_Pos            (28)
#define RCC_APB1RSTR_PWR                (0x01U << RCC_APB1RSTR_PWR_Pos)         /*!< Power interface reset */

#define RCC_APB1RSTR_DBGMCU_Pos         (29)
#define RCC_APB1RSTR_DBGMCU             (0x01U << RCC_APB1RSTR_DBGMCU_Pos)      /*!< DBGMCU register reset */

#define RCC_APB1RSTR_EXTI_Pos           (30)
#define RCC_APB1RSTR_EXTI               (0x01U << RCC_APB1RSTR_EXTI_Pos)        /*!< EXTI register reset */

/**
  * @brief RCC_AHBENR Register Bit Definition
  */
#define RCC_AHBENR_SRAM_Pos             (2)
#define RCC_AHBENR_SRAM                 (0x01U << RCC_AHBENR_SRAM_Pos)          /*!< SRAM interface clock enable */
#define RCC_AHBENR_FLITF_Pos            (4)
#define RCC_AHBENR_FLITF                (0x01U << RCC_AHBENR_FLITF_Pos)         /*!< FLITF clock enable */
#define RCC_AHBENR_CRC_Pos              (6)
#define RCC_AHBENR_CRC                  (0x01U << RCC_AHBENR_CRC_Pos)           /*!< Internal High Speed clock Calibration */

#define RCC_AHBENR_GPIOA_Pos            (17)
#define RCC_AHBENR_GPIOA                (0x01U << RCC_AHBENR_GPIOA_Pos)         /*!< GPIOA clock enable */
#define RCC_AHBENR_GPIOB_Pos            (18)
#define RCC_AHBENR_GPIOB                (0x01U << RCC_AHBENR_GPIOB_Pos)         /*!< GPIOB clock enable */

/**
  * @brief RCC_APB1ENR Register Bit Definition
  */
#define RCC_APB1ENR_TIM3_Pos            (1)
#define RCC_APB1ENR_TIM3                (0x01U << RCC_APB1ENR_TIM3_Pos)         /*!< Timer 3 clock enabled */

#define RCC_APB1ENR_TIM1_Pos            (2)
#define RCC_APB1ENR_TIM1                (0x01U << RCC_APB1ENR_TIM1_Pos)         /*!< TIM1 Timer clock enable */

#define RCC_APB1ENR_TIM14_Pos           (3)
#define RCC_APB1ENR_TIM14               (0x01U << RCC_APB1ENR_TIM14_Pos)        /*!< TIM14 Timer clock enable */
#define RCC_APB1ENR_ADC1_Pos            (9)
#define RCC_APB1ENR_ADC1                (0x01U << RCC_APB1ENR_ADC1_Pos)         /*!< ADC 1 interface clock enable */

#define RCC_APB1ENR_SPI1_Pos            (12)
#define RCC_APB1ENR_SPI1                (0x01U << RCC_APB1ENR_SPI1_Pos)         /*!< SPI 1 clock enable */

#define RCC_APB1ENR_UART1_Pos           (16)
#define RCC_APB1ENR_UART1               (0x01U << RCC_APB1ENR_UART1_Pos)        /*!< UART 1 clock enable */
#define RCC_APB1ENR_USART1_Pos          (16)
#define RCC_APB1ENR_USART1              (0x01U << RCC_APB1ENR_USART1_Pos)        /*!< USART 1 clock enable*/
#define RCC_APB1ENR_UART2_Pos           (17)
#define RCC_APB1ENR_UART2               (0x01U << RCC_APB1ENR_UART2_Pos)        /*!< UART 2 clock enable */
#define RCC_APB1ENR_USART2_Pos          (17)
#define RCC_APB1ENR_USART2  		    (0x01U << RCC_APB1ENR_USART2_Pos)       /*!< USART 2 clock enable  */
#define RCC_APB1ENR_I2C1_Pos            (21)
#define RCC_APB1ENR_I2C1                (0x01U << RCC_APB1ENR_I2C1_Pos)         /*!< I2C 1 clock enable */

#define RCC_APB1ENR_PWR_Pos             (28)
#define RCC_APB1ENR_PWR                 (0x01U << RCC_APB1ENR_PWR_Pos)          /*!< Power interface clock enable */

#define RCC_APB1ENR_DBGMCU_Pos          (29)
#define RCC_APB1ENR_DBGMCU              (0x01U << RCC_APB1ENR_DBGMCU_Pos)       /*!< DBGMCU clock enable */

#define RCC_APB1ENR_EXTI_Pos            (30)
#define RCC_APB1ENR_EXTI                (0x01U << RCC_APB1ENR_EXTI_Pos)         /*!< Exti interface clock enable */
#define RCC_APB1ENR_SYSCFG_Pos           (30)
#define RCC_APB1ENR_SYSCFG               (0x01U << RCC_APB1ENR_EXTI_Pos)        /*!< Exti interface clock enable  */ 
/**
  * @brief RCC_CSR Register Bit Definition
  */
#define RCC_CSR_LSION_Pos               (0)
#define RCC_CSR_LSION                   (0x01U << RCC_CSR_LSION_Pos)            /*!< Internal Low Speed oscillator enable */
#define RCC_CSR_LSIRDY_Pos              (1)
#define RCC_CSR_LSIRDY                  (0x01U << RCC_CSR_LSIRDY_Pos)           /*!< Internal Low Speed oscillator Ready */

#define RCC_CSR_PVDRSTEN_Pos            (6)
#define RCC_CSR_PVDRSTEN                (0x01U << RCC_CSR_PVDRSTEN_Pos)         /*!< Internal Low Speed oscillator Ready */
#define RCC_CSR_LOCKUPEN_Pos            (7)
#define RCC_CSR_LOCKUPEN                (0x01U << RCC_CSR_LOCKUPEN_Pos)         /*!< Internal Low Speed oscillator Ready */
#define RCC_CSR_PVDRSTF_Pos             (22)
#define RCC_CSR_PVDRSTF                 (0x01U << RCC_CSR_PVDRSTF_Pos)          /*!< Internal Low Speed oscillator Ready */
#define RCC_CSR_LOCKUPF_Pos             (23)
#define RCC_CSR_LOCKUPF                 (0x01U << RCC_CSR_LOCKUPF_Pos)          /*!< Internal Low Speed oscillator Ready */
#define RCC_CSR_RMVF_Pos                (24)
#define RCC_CSR_RMVF                    (0x01U << RCC_CSR_RMVF_Pos)             /*!< Remove reset flag */
#define RCC_CSR_PINRSTF_Pos             (26)
#define RCC_CSR_PINRSTF                 (0x01U << RCC_CSR_PINRSTF_Pos)          /*!< PIN reset flag */

#define RCC_CSR_PORRSTF_Pos             (27)
#define RCC_CSR_PORRSTF                 (0x01U << RCC_CSR_PORRSTF_Pos)          /*!< POR/PDR reset flag */

#define RCC_CSR_SFTRSTF_Pos             (28)
#define RCC_CSR_SFTRSTF                 (0x01U << RCC_CSR_SFTRSTF_Pos)          /*!< Software Reset flag */

#define RCC_CSR_IWDGRSTF_Pos            (29)
#define RCC_CSR_IWDGRSTF                (0x01U << RCC_CSR_IWDGRSTF_Pos)         /*!< Independent Watchdog reset flag */


/**
  * @brief RCC_AHBRSTR Register Bit Definition
  */
#define RCC_AHBRSTR_GPIOA_Pos           (17)
#define RCC_AHBRSTR_GPIOA               (0x01U << RCC_AHBRSTR_GPIOA_Pos)        /*!< GPIOA clock reset */
#define RCC_AHBRSTR_GPIOB_Pos           (18)
#define RCC_AHBRSTR_GPIOB               (0x01U << RCC_AHBRSTR_GPIOB_Pos)        /*!< GPIOB clock reset */
/**
  * @brief RCC_SYSCFG Register Bit Definition
  */
#define RCC_SYSCFG_PROGCHECKEN_Pos      (0)
#define RCC_SYSCFG_PROGCHECKEN          (0x01U << RCC_SYSCFG_PROGCHECKEN_Pos)
#define RCC_SYSCFG_SECTOR1KCFG_Pos      (1)
#define RCC_SYSCFG_SECTOR1KCFG          (0x01U << RCC_SYSCFG_SECTOR1KCFG_Pos)
#define RCC_SYSCFG_SFTNRSTRAMP_Pos      (2)
#define RCC_SYSCFG_SFTNRSTRAMP          (0x01U << RCC_SYSCFG_SFTNRSTRAMP_Pos)
#define RCC_SYSCFG_OSCLPFEN_Pos         (14)
#define RCC_SYSCFG_OSCLPFEN             (0x01U << RCC_SYSCFG_OSCLPFEN_Pos)
#define RCC_CONFIG_PAGESIZE_Pos         (1)
#define RCC_CONFIG_PAGESIZE             (0x01U << RCC_CONFIG_PAGESIZE_Pos)      /*!< Flash Page size */
#define RCC_CONFIG_OSC_RTRIM_Pos        (8)
#define RCC_CONFIG_OSC_RTRIM            (0x07U << RCC_CONFIG_OSC_RTRIM_Pos)     /*!< Oscillator feedback resistance trimming */
#define RCC_CONFIG_OSC_ITRIM_Pos        (11)
#define RCC_CONFIG_OSC_ITRIM            (0x03U << RCC_CONFIG_OSC_ITRIM_Pos)     /*!< Oscillator drive current trimming */
#define RCC_CONFIG_OSC_LPFEN_Pos        (14)
#define RCC_CONFIG_OSC_LPFEN            (0x01U << RCC_CONFIG_OSC_LPFEN_Pos)     /*!< Oscillator low pass filtering enable */

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
#endif
/** --------------------------------------------------------------------------*/
