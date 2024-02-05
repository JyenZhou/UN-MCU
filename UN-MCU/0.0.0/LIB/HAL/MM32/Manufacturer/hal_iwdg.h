/***********************************************************************************************************************
    @file     hal_iwdg.h
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
#ifndef __HAL_IWDG_H
#define __HAL_IWDG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Files includes ------------------------------------------------------------*/
#include "mm32_device.h"

/** @addtogroup MM32_Hardware_Abstract_Layer
  * @{
  */

/** @defgroup IWDG_HAL
  * @brief IWDG HAL modules
  * @{
  */

/** @defgroup IWDG_Exported_Constants
  * @{
  */

/**
  * @brief  IWDG prescaler
  */
typedef enum {
    IWDG_Prescaler_4                    = IWDG_PR_PRE_DIV4,
    IWDG_Prescaler_8                    = IWDG_PR_PRE_DIV8,
    IWDG_Prescaler_16                   = IWDG_PR_PRE_DIV16,
    IWDG_Prescaler_32                   = IWDG_PR_PRE_DIV32,
    IWDG_Prescaler_64                   = IWDG_PR_PRE_DIV64,
    IWDG_Prescaler_128                  = IWDG_PR_PRE_DIV128,
    IWDG_Prescaler_256                  = IWDG_PR_PRE_DIV256
} IWDGPrescaler_TypeDef;

/**
  * @brief  IWDG flag
  */
typedef enum {
    IWDG_FLAG_PVU                       = 0x0001,                               /*!< IWDG prescaler value update flag */
    IWDG_FLAG_RVU                       = 0x0002,                                /*!< IWDG counter reload value update flag */
    IWDG_FLAG_IVU                       = 0x0004 
} IWDGFlag_TypeDef;

/**
  * @brief  Write access to IWDG_PR and IWDG_RLR registers
  */
typedef enum {
    IWDG_WriteAccess_Enable             = 0x5555,                               /*!< Enable write */
    IWDG_WriteAccess_Disable            = 0x0000                                /*!< Disable write */
} IWDGWriteAccess_TypeDef;

/**
  * @brief  IWDG Key Reload
  */
typedef enum {
    KR_KEY_Reload                       = 0xAAAA,                               /*!< Reload value */
    KR_KEY_Enable                       = 0xCCCC                                /*!< Start IWDG */
} IWDGKey_TypeDef;

/**
  * @brief  IWDG Overflow Configration
  */
#define IWDG_Overflow_Reset       (0x00U << IWDG_CR_IRQSEL_Pos)
#define IWDG_Overflow_Interrupt   (0x01U << IWDG_CR_IRQSEL_Pos)

/**
  * @}
  */

/** @defgroup IWDG_Exported_Variables
  * @{
  */
#ifdef _HAL_IWDG_C_
#define GLOBAL

#else
#define GLOBAL extern
#endif

#undef GLOBAL
/**
  * @}
  */

/** @defgroup IWDG_Exported_Functions
  * @{
  */
FlagStatus IWDG_GetFlagStatus(uint16_t flag);

void IWDG_WriteAccessCmd(uint16_t write_access);
void IWDG_SetPrescaler(uint8_t prescaler);
void IWDG_SetReload(uint16_t reload);
uint32_t IWDG_GetReload(void);
void IWDG_ReloadCounter(void);
void IWDG_Enable(void);
void PVU_CheckStatus(void);
void RVU_CheckStatus(void);

void IWDG_ClearITPendingBit(void);
void IVU_CheckStatus(void);
void IWDG_SetIGen(uint16_t GenValue);
void IWDG_OverflowConfig(uint32_t overflow_action);

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
#endif /* __HAL_IWDG_H -------------------------------------------------------*/
/** --------------------------------------------------------------------------*/
