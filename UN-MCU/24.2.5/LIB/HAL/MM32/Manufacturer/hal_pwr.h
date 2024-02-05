/***********************************************************************************************************************
    @file     hal_pwr.h
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
#ifndef __HAL_PWR_H
#define __HAL_PWR_H

#ifdef __cplusplus
extern "C" {
#endif

/* Files includes ------------------------------------------------------------*/
#include "mm32_device.h"

/** @addtogroup MM32_Hardware_Abstract_Layer
  * @{
  */

/** @defgroup PWR_HAL
  * @brief PWR HAL modules
  * @{
  */

/** @defgroup PWR_Exported_Types
  * @{
  */

/**
  * @brief PVD_detection_level
  */
typedef enum {
    emPVD_1V8 = PWR_CR_PLS_1V8,
    emPVD_2V1 = PWR_CR_PLS_2V1,
    emPVD_2V4 = PWR_CR_PLS_2V4,
    emPVD_2V7 = PWR_CR_PLS_2V7,
    emPVD_3V0 = PWR_CR_PLS_3V0,
    emPVD_3V3 = PWR_CR_PLS_3V3,
    emPVD_3V6 = PWR_CR_PLS_3V6,
    emPVD_3V9 = PWR_CR_PLS_3V9,
    emPVD_4V2 = PWR_CR_PLS_4V2,
    emPVD_4V5 = PWR_CR_PLS_4V5,
    emPVD_4V8 = PWR_CR_PLS_4V8
} EM_PVD_LEVEL;
#define PWR_PVDLevel_1V8          PWR_CR_PLS_1V8
#define PWR_PVDLevel_2V1          PWR_CR_PLS_2V1
#define PWR_PVDLevel_2V4          PWR_CR_PLS_2V4
#define PWR_PVDLevel_2V7          PWR_CR_PLS_2V7
#define PWR_PVDLevel_3V0          PWR_CR_PLS_3V0
#define PWR_PVDLevel_3V3          PWR_CR_PLS_3V3
#define PWR_PVDLevel_3V6          PWR_CR_PLS_3V6
#define PWR_PVDLevel_3V9          PWR_CR_PLS_3V9
#define PWR_PVDLevel_4V2          PWR_CR_PLS_4V2
#define PWR_PVDLevel_4V5          PWR_CR_PLS_4V5
#define PWR_PVDLevel_4V8          PWR_CR_PLS_4V8

/**
  * @brief Regulator_state_is_STOP_mode
  */
typedef enum {
    PWR_Regulator_ON                    = 0x00000000,
    PWR_Regulator_LowPower              = 0x00000001
} emPWR_Reg_Stop_mode_Typedef;

/**
  * @brief STOP_mode_entry
  */
typedef enum {
    PWR_STOPEntry_WFI                   = 0x00000001,
    PWR_STOPEntry_WFE                   = 0x00000002
} emPWR_STOP_ModeEn_Typedef;

/**
  * @brief Low Power Mode
  */
typedef enum {
    LP_STOP_MODE                        = 0,
    LP_SLEEP_MODE                       = 1
} emPWR_LP_Mode_Typedef;

/**
  * @brief Wait_for_mode
  */
typedef enum {
    LP_WFI,
    LP_WFE
} emPWR_Wait_Mode_Typedef;
/**
  * @brief PWR_Flag
  */
typedef enum {
    PWR_FLAG_WU                         = PWR_CSR_WUF,
    PWR_FLAG_SB                         = PWR_CSR_SBF,

    PWR_FLAG_PVDO                       = PWR_CSR_PVDO
} emPWR_PWR_Flag_Typedef;


/**
  * @brief SLEEP_mode_entry
  */
#define PWR_SLEEPNOW_WFI   0x00000001
#define PWR_SLEEPNOW_WFE   0x00000002
#define PWR_SLEEPONEXIT    0x00000003

/**
  * @}
  */

/** @defgroup PWR_Exported_Variables
  * @{
  */

#ifdef _HAL_PWR_C_

#define GLOBAL
#else
#define GLOBAL extern
#endif

#undef GLOBAL

/**
  * @}
  */

/** @defgroup PWR_Exported_Functions
  * @{
  */

void PWR_DeInit(void);

void PWR_PVDCmd(FunctionalState state);
void PWR_PVDLevelConfig(uint32_t pvd_level);
void PWR_WakeUpPinCmd(FunctionalState state);
void PWR_EnterSTOPMode(emPWR_Reg_Stop_mode_Typedef regulator, uint8_t stop_entry);


void PWR_ClearFlag(uint32_t flag);
FlagStatus PWR_GetFlagStatus(uint32_t flag);
void PWR_EnterSLEEPMode(uint32_t sleep_entry);

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
#endif /* __HAL_PWR_H --------------------------------------------------------*/
/** --------------------------------------------------------------------------*/
