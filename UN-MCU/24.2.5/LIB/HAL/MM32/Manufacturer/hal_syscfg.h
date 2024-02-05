/*
 *******************************************************************************
    @file     hal_syscfg.h
    @author   VV TEAM
    @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE EXTI
              FIRMWARE LIBRARY.
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
#ifndef __HAL_SYSCFG_H
#define __HAL_SYSCFG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Files includes ------------------------------------------------------------*/
#include "mm32_device.h"

/** @addtogroup MM32_Hardware_Abstract_Layer
  * @{
  */


/** @defgroup SYSCFG_HAL
  * @brief SYSCFG HAL modules
  * @{
  */

/** @defgroup SYSCFG_Exported_Types
  * @{
  */

/**
  * @brief  SYSCFG mode enumeration
  */
/*!< @defgroup SYSCFG_Memory_Remap_Config */
#define SYSCFG_MemoryRemap_Flash        ((uint8_t)0x00)
#define SYSCFG_MemoryRemap_SystemMemory ((uint8_t)0x01)
#define SYSCFG_MemoryRemap_SRAM         ((uint8_t)0x03)

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/* Function used to set the SYSCFG configuration to the default reset state --*/
#define SYSCFG_DeInit                   EXTI_DeInit
#define SYSCFG_MemoryRemapConfig        EXTI_MemoryRemapConfig
#define SYSCFG_EXTILineConfig           EXTI_LineConfig

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
#endif /* __HAL_SYSCFG_H -----------------------------------------------------*/
/** --------------------------------------------------------------------------*/

