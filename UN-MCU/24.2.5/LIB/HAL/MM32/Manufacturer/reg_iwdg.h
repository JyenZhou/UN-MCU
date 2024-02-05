/***********************************************************************************************************************
    @file     reg_iwdg.h
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

#ifndef __REG_IWDG_H
#define __REG_IWDG_H

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
  * @brief IWDG Base Address Definition
  */
#define IWDG_BASE                       (APB1PERIPH_BASE + 0x3000)              /*!< Base Address: 0x40003000 */

/**
  * @brief IWDG Register Structure Definition
  */
typedef struct {
    __IO uint32_t KR;                                                                /*!< Key Register                                   offset: 0x00 */
    __IO uint32_t PR;                                                                /*!< Prescaler Register                             offset: 0x04 */
    __IO uint32_t RLR;                                                               /*!< Reload Register                                offset: 0x08 */
    __IO uint32_t SR;                                                                /*!< Status Register                                offset: 0x0C */
    __IO uint32_t CR;                                                                /*!< Control Register                               offset: 0x10 */
    __IO uint32_t IGEN;                                                              /*!< Interrupt Generator Register                   offset: 0x14 */
    __IO uint32_t CNT;                                                               /*!< Interrupt Generator count Register             offset: 0x18 */
} IWDG_TypeDef;

/**
  * @brief IWDG type pointer Definition
  */
#define IWDG                            ((IWDG_TypeDef*) IWDG_BASE)

/**
  * @brief IWDG_KR Register Bit Definition
  */
#define IWDG_KEYR_KEY_Pos               (0)
#define IWDG_KEYR_KEY                   (0xFFFFU << IWDG_KEYR_KEY_Pos)          /*!< Key Value */

/**
  * @brief IWDG_PR Register Bit Definition
  */
#define IWDG_PR_PRE_Pos                 (0)
#define IWDG_PR_PRE                     (0x07U << IWDG_PR_PRE_Pos)              /*!< Prescaler divided mask */
#define IWDG_PR_PRE_DIV4                (0x00U << IWDG_PR_PRE_Pos)              /*!< Prescaler divided by 4 */
#define IWDG_PR_PRE_DIV8                (0x01U << IWDG_PR_PRE_Pos)              /*!< Prescaler divided by 8 */
#define IWDG_PR_PRE_DIV16               (0x02U << IWDG_PR_PRE_Pos)              /*!< Prescaler divided by 16 */
#define IWDG_PR_PRE_DIV32               (0x03U << IWDG_PR_PRE_Pos)              /*!< Prescaler divided by 32 */
#define IWDG_PR_PRE_DIV64               (0x04U << IWDG_PR_PRE_Pos)              /*!< Prescaler divided by 64 */
#define IWDG_PR_PRE_DIV128              (0x05U << IWDG_PR_PRE_Pos)              /*!< Prescaler divided by 128 */
#define IWDG_PR_PRE_DIV256              (0x06U << IWDG_PR_PRE_Pos)              /*!< Prescaler divided by 256 */

/**
  * @brief IWDG_RLR Register Bit Definition
  */
#define IWDG_RLR_RL_Pos                 (0)
#define IWDG_RLR_RL                     (0x0FFFU << IWDG_RLR_RL_Pos)            /*!< Watchdog counter reload value */

/**
  * @brief IWDG_SR Register Bit Definition
  */
#define IWDG_SR_PVU_Pos                 (0)
#define IWDG_SR_PVU                     (0x01U << IWDG_SR_PVU_Pos)              /*!< Watchdog prescaler value update */
#define IWDG_SR_RVU_Pos                 (1)
#define IWDG_SR_RVU                     (0x01U << IWDG_SR_RVU_Pos)              /*!< Watchdog counter reload value update */
#define IWDG_SR_IVU_Pos                 (2)
#define IWDG_SR_IVU                     (0x01U << IWDG_SR_IVU_Pos)              /*!< Watchdog Interrupt Generate value update */
#define IWDG_SR_UPDATE_Pos              (3)
#define IWDG_SR_UPDATE                  (0x01U << IWDG_SR_UPDATE_Pos)           /*!< Watchdog reload value update flag */

/**
  * @brief IWDG_CR Register Bit Definition
  */
#define IWDG_CR_IRQSEL_Pos              (0)
#define IWDG_CR_IRQSEL_Msk              (0x01U << IWDG_CR_IRQSEL_Pos)           /*!< IWDG overflow operation selection */
#define IWDG_CR_IRQCLR_Pos              (1)
#define IWDG_CR_IRQCLR                  (0x01U << IWDG_CR_IRQCLR_Pos)           /*!< IWDG interrupt clear */

/**
  * @brief IWDG_IGRN Register Bit Definition
  */
#define IWDG_IGEN_IGEN_Pos              (0)
#define IWDG_IGEN_IGEN                  (0xFFFU << IWDG_IGEN_IGEN_Pos)          /*!< IWDG Interrupt Generate value */

/**
  * @brief IWDG_CNT Register Bit Definition
  */
#define IWDG_CNT_Pos                    (0)
#define IWDG_CNT                        (0xFFFU << IWDG_CNT_Pos)                /*!< IWDG counter value */
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
