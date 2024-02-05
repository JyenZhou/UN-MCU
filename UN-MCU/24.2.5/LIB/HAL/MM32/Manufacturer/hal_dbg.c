/*
 *******************************************************************************
    @file     hal_dbg.c
    @author   VV TEAM
    @brief    THIS FILE PROVIDES ALL THE DBG FIRMWARE FUNCTIONS.
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
#define _HAL_DBG_C

// Files includes
#include "hal_dbg.h"

/** @addtogroup MM32_Hardware_Abstract_Layer
  * @{
  */

/** @addtogroup DBG_HAL
  * @{
  */

/** @addtogroup DBG_Exported_Functions
  * @{
  */

/**
  * @brief  Enables or disables the specified DBG peripheral.
  * @param  periph: DBG peripheral.
  * @param  state: new state of the specified DBG peripheral.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None.
  */
void DBGMCU_Configure(uint32_t periph, FunctionalState state)
{
    (state) ? (DBGMCU->CR |= periph) : (DBGMCU->CR &= ~periph);
}

/**
  * @brief  Returns the device identifier..
  * @param  None.
  * @retval return the device Device identifier.
  */
uint32_t DBGMCU_GetDeviceIDCode(void)
{
    return (uint32_t)DBGMCU->IDCODE;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
