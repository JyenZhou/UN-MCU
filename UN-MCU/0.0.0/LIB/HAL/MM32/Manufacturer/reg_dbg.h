/*
 *******************************************************************************
    @file     reg_dbg.h
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

#ifndef __REG_DBG_H
#define __REG_DBG_H

/* Files includes ------------------------------------------------------------*/
#include  "core_cm0.h"


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
  * @brief DBG Base Address Definition
  */
#define DBG_BASE                        (APB1PERIPH_BASE + 0x13400)             /*!< Base Address: 0x40013400 */

/**
  * @brief DEBUG Registers Structure Definition
  */
typedef struct {
    __IO uint32_t IDCODE;                                                            /*!< Code ID                                        offset: 0x00 */
    __IO uint32_t CR;                                                                /*!< Control Register                               offset: 0x04 */
} DBGMCU_TypeDef;

/**
  * @brief DBGMCU type pointer Definition
  */
#define DBGMCU                          ((DBGMCU_TypeDef*) DBG_BASE)

/**
  * @brief DBGMCU_IDCODE Register Bit Definition
  */
#define DBGMCU_IDCODE_DEV_ID_Pos        (0)
#define DBGMCU_IDCODE_DEV_ID            (0xFFFFFFFFU << DBGMCU_IDCODE_DEV_ID_Pos)   /*!< Device identifier */

/**
  * @brief DBGMCU_CR Register Bit Definition
  */
#define DBGMCU_CR_SLEEP_Pos             (0)
#define DBGMCU_CR_SLEEP                 (0x01U << DBGMCU_CR_SLEEP_Pos)          /*!< Debug Sleep mode */
#define DBGMCU_CR_STOP_Pos              (1)
#define DBGMCU_CR_STOP                  (0x01U << DBGMCU_CR_STOP_Pos)           /*!< Debug Stop mode */
#define DBGMCU_CR_STANDBY_Pos           (2)
#define DBGMCU_CR_STANDBY               (0x01U << DBGMCU_CR_STANDBY_Pos)        /*!< Debug Standby mode */

#define DBGMCU_CR_IWDG_STOP_Pos         (8)
#define DBGMCU_CR_IWDG_STOP             (0x01U << DBGMCU_CR_IWDG_STOP_Pos)      /*!< Debug independent watchdog stopped when core is halted */
#define DBGMCU_CR_WWDG_STOP_Pos         (9)
#define DBGMCU_CR_WWDG_STOP             (0x01U << DBGMCU_CR_WWDG_STOP_Pos)      /*!< Debug window watchdog stopped when core is halted */
#define DBGMCU_CR_TIM_STOP_Pos          (10)
#define DBGMCU_CR_TIM1_STOP             (0x01U << DBGMCU_CR_TIM_STOP_Pos)       /*!< TIM1 counter stopped when core is halted */
#define DBGMCU_CR_TIM3_STOP             (0x04U << DBGMCU_CR_TIM_STOP_Pos)       /*!< TIM3 counter stopped when core is halted */
#define DBGMCU_CR_TIM14_STOP            (0x08U << DBGMCU_CR_TIM_STOP_Pos)       /*!< TIM14 counter stopped when core is halted */

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
