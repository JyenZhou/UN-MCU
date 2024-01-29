/*
 *******************************************************************************
    @file     reg_syscfg.h
    @author   VV TEAM
    @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE SERIES OF
              MM32 FIRMWARE LIBRARY.
 *******************************************************************************
    @attention

    <h2><center>&copy; Copyright(c) <2023> <MindMotion></center></h2>

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the
    following conditions are met:
    1. Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its
       contributors may be used to endorse or promote products derived from this
       software without specific prior written permission.

    THE EXISTING FIRMWARE IS ONLY FOR REFERENCE, WHICH IS DESIGNED TO PROVIDE
    CUSTOMERS WITH CODING INFORMATION ABOUT THEIR PRODUCTS SO THEY CAN SAVE
    TIME. THEREFORE, MINDMOTION SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
    CONSEQUENTIAL DAMAGES ABOUT ANY CLAIMS ARISING OUT OF THE CONTENT OF SUCH
    HARDWARE AND/OR THE USE OF THE CODING INFORMATION CONTAINED HEREIN IN
    CONNECTION WITH PRODUCTS MADE BY CUSTOMERS.

    <H2><CENTER>&COPY; COPYRIGHT MINDMOTION </CENTER></H2>
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __REG_SYSCFG_H
#define __REG_SYSCFG_H

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

#define SYSCFG_BASE                     (APB1PERIPH_BASE + 0x010000)            /*!< Base Address: 0x40010000 */

/**
  * @brief SysTem Configuration Register Structure Definition
  */
typedef struct {
    union {
        __IO uint32_t CFGR;                                                          /*!< SYSCFG configuration register                  offset: 0x00 */
        __IO uint32_t CFGR1;
    };
    __IO uint32_t RESERVED0x04;                                                      /*!< RESERVED register                              offset: 0x04 */
    __IO uint32_t EXTICR[4];                                                         /*!< EXTI Control register                          offset: 0x08-0x14 */
    __IO uint32_t PADHYS;                                                            /*!< PADHYS register                                offset: 0x18 */
} SYSCFG_TypeDef;

#define SYSCFG                          ((SYSCFG_TypeDef *) SYSCFG_BASE)

/**
  * @brief System Configuration (SYSCFG)
  */

/**
  * @brief SYSCFG_CFGR Register Bit definition
  */
#define SYSCFG_CFGR_MEM_MODE_Pos        (0)
#define SYSCFG_CFGR_MEM_MODE            ((uint32_t)0x00000003)                       /*!< SYSCFG_Memory Remap Config */
#define SYSCFG_CFGR_MEM_MODE_0          ((uint32_t)0x00000001)                       /*!< SYSCFG_Memory Remap Config Bit 0 */
#define SYSCFG_CFGR_MEM_MODE_1          ((uint32_t)0x00000002)                       /*!< SYSCFG_Memory Remap Config Bit 1 */

/**
  * @brief SYSCFG_EXTICR1 Register Bit definition
  */
#define SYSCFG_EXTICR1_EXTI0            ((uint16_t)0x000F)                           /*!< EXTI 0 configuration */
#define SYSCFG_EXTICR1_EXTI1            ((uint16_t)0x00F0)                           /*!< EXTI 1 configuration */
#define SYSCFG_EXTICR1_EXTI2            ((uint16_t)0x0F00)                           /*!< EXTI 2 configuration */
#define SYSCFG_EXTICR1_EXTI3            ((uint16_t)0xF000)                           /*!< EXTI 3 configuration */

/**
  *  @brief  EXTI0 configuration
  */
#define SYSCFG_EXTICR1_EXTI0_PA         ((uint16_t)0x0000)                           /*!< PA[0] pin */
#define SYSCFG_EXTICR1_EXTI0_PB         ((uint16_t)0x0001)                           /*!< PB[0] pin */

/**
  *  @brief  EXTI1 configuration
  */
#define SYSCFG_EXTICR1_EXTI1_PA         ((uint16_t)0x0000)                           /*!< PA[1] pin */
#define SYSCFG_EXTICR1_EXTI1_PB         ((uint16_t)0x0010)                           /*!< PB[1] pin */

/**
  *  @brief  EXTI2 configuration
  */
#define SYSCFG_EXTICR1_EXTI2_PA         ((uint16_t)0x0000)                           /*!< PA[2] pin */

/**
  *  @brief  EXTI3 configuration
  */
#define SYSCFG_EXTICR1_EXTI3_PA         ((uint16_t)0x0000)                           /*!< PA[3] pin */

/**
  * @brief SYSCFG_EXTICR2 Register Bit definition
  */
#define SYSCFG_EXTICR2_EXTI4            ((uint16_t)0x000F)                           /*!< EXTI 4 configuration */
#define SYSCFG_EXTICR2_EXTI5            ((uint16_t)0x00F0)                           /*!< EXTI 5 configuration */
#define SYSCFG_EXTICR2_EXTI6            ((uint16_t)0x0F00)                           /*!< EXTI 6 configuration */
#define SYSCFG_EXTICR2_EXTI7            ((uint16_t)0xF000)                           /*!< EXTI 7 configuration */

/**
  * @brief  EXTI4 configuration
  */
#define SYSCFG_EXTICR2_EXTI4_PA         ((uint16_t)0x0000)                           /*!< PA[4] pin */

/**
  * @brief  EXTI5 configuration
  */
#define SYSCFG_EXTICR2_EXTI5_PA         ((uint16_t)0x0000)                           /*!< PA[5] pin */

/**
  * @brief  EXTI6 configuration
  */
#define SYSCFG_EXTICR2_EXTI6_PA         ((uint16_t)0x0000)                           /*!< PA[6] pin */

/**
  * @brief  EXTI7 configuration
  */
#define SYSCFG_EXTICR2_EXTI7_PA         ((uint16_t)0x0000)                           /*!< PA[7] pin */

/**
  * @brief SYSCFG_EXTICR3 Register Bit definition
  */
#define SYSCFG_EXTICR3_EXTI8            ((uint16_t)0x000F)                           /*!< EXTI 8 configuration */
#define SYSCFG_EXTICR3_EXTI9            ((uint16_t)0x00F0)                           /*!< EXTI 9 configuration */
#define SYSCFG_EXTICR3_EXTI10           ((uint16_t)0x0F00)                           /*!< EXTI 10 configuration */
#define SYSCFG_EXTICR3_EXTI11           ((uint16_t)0xF000)                           /*!< EXTI 11 configuration */

/**
  * @brief  EXTI8 configuration
  */
#define SYSCFG_EXTICR3_EXTI8_PA         ((uint16_t)0x0000)                           /*!< PA[8] pin */

/**
  * @brief  EXTI9 configuration
  */
#define SYSCFG_EXTICR3_EXTI9_PA         ((uint16_t)0x0000)                           /*!< PA[9] pin */

/**
  * @brief  EXTI10 configuration
  */
#define SYSCFG_EXTICR3_EXTI10_PA        ((uint16_t)0x0000)                           /*!< PA[10] pin */

/**
  * @brief  EXTI11 configuration
  */
#define SYSCFG_EXTICR3_EXTI11_PA        ((uint16_t)0x0000)                           /*!< PA[11] pin */

/**
  * @brief SYSCFG_EXTICR4 Register Bit definition
  */
#define SYSCFG_EXTICR4_EXTI12           ((uint16_t)0x000F)                           /*!< EXTI 12 configuration */
#define SYSCFG_EXTICR4_EXTI13           ((uint16_t)0x00F0)                           /*!< EXTI 13 configuration */
#define SYSCFG_EXTICR4_EXTI14           ((uint16_t)0x0F00)                           /*!< EXTI 14 configuration */
#define SYSCFG_EXTICR4_EXTI15           ((uint16_t)0xF000)                           /*!< EXTI 15 configuration */

#define SYSCFG_EXTICR4_EXTI12_PA        ((uint16_t)0x0000)                           /*!< PA[12] pin for EXTI12 */

#define SYSCFG_EXTICR4_EXTI13_PA        ((uint16_t)0x0000)                           /*!< PA[13] pin for EXTI13 */

#define SYSCFG_EXTICR4_EXTI14_PA        ((uint16_t)0x0000)                           /*!< PA[14] pin for EXTI14 */

#define SYSCFG_EXTICR4_EXTI15_PA        ((uint16_t)0x0000)                           /*!< PA[15] pin for EXTI15 */

/**
  * @brief SYSCFG_PADHYS Register Bit definition
  */
#define SYSCFG_PADHYS_I2C1_MODE_SEL_Pos (16)
#define SYSCFG_PADHYS_I2C1_MODE_SEL     (0x01U  << SYSCFG_PADHYS_I2C1_MODE_SEL_Pos)                   /*!< I2C1 Enable PushPull mode */

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
