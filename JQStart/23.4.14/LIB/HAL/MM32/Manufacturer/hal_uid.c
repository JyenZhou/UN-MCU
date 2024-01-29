/***********************************************************************************************************************
    @file     hal_uid.c
    @author   VV TEAM
    @brief    THIS FILE PROVIDES ALL THE UID FIRMWARE FUNCTIONS.
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
#define _HAL_UID_C_

/* Files includes ------------------------------------------------------------*/
#include "hal_uid.h"

/** @addtogroup MM32_Hardware_Abstract_Layer
  * @{
  */

/** @addtogroup UID_HAL
  * @{
  */

/** @addtogroup UID_Exported_Functions
  * @{
  */

/**
  * @brief  Get device ID.
  * @param  None.
  * @retval None.
  */
	  u8 device_id_data[12];
void GetChipUID(void)
{
    u8 i;
    u8* device_id;
    device_id = (u8*)0x1FFFF7E8;

    for (i = 0; i < 12; i++) {
        device_id_data[i] = *(device_id + i);
    }
}

/**
  * @brief  Returns first word of the unique device identifier (UID based on 96 bits)
  * @param  None.
  * @retval Device identifier
  */
uint32_t Get_ChipsetUIDw0(void)
{
    return *((__IO uint32_t *)UID_BASE);
}

/**
  * @brief  Returns second word of the unique device identifier (UID based on 96 bits)
  * @param  None.
  * @retval Device identifier
  */
uint32_t Get_ChipsetUIDw1(void)
{
    return *((__IO uint32_t *)(UID_BASE + 4U));
}

/**
  * @brief  Returns third word of the unique device identifier (UID based on 96 bits)
  * @param  None.
  * @retval Device identifier
  */
uint32_t Get_ChipsetUIDw2(void)
{
    return *((__IO uint32_t *)(UID_BASE + 8U));
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
