/***********************************************************************************************************************
    @file     hal_crc.c
    @author   VV TEAM
    @brief    THIS FILE PROVIDES ALL THE CRC FIRMWARE FUNCTIONS.
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
#define _HAL_CRC_C_

/* Files includes ------------------------------------------------------------*/
#include "hal_crc.h"

/** @addtogroup MM32_Hardware_Abstract_Layer
  * @{
  */

/** @addtogroup CRC_HAL
  * @{
  */

/** @addtogroup CRC_Exported_Functions
  * @{
  */

/**
  * @brief  Resets the CRC Data register (DR).
  * @param  None.
  * @retval None.
  */
void CRC_ResetDR(void)
{
    CRC->CR = CRC_CR_RESET;
}
//#pragma optimize(0)
/**
  * @brief  Computes the 32-bit CRC of a given data word(32-bit).
  * @param  Data: data word(32-bit) to compute its CRC
  * @retval 32-bit CRC
  */
uint32_t CRC_CalcCRC(uint32_t data)
{
    CRC->DR = data;
    return (CRC->DR);
}

/**
  * @brief  Computes the 32-bit CRC of a given buffer of data word(32-bit).
  * @param  buffer: pointer to the buffer containing the data to be computed
  * @param  length: length of the buffer to be computed
  * @retval 32-bit CRC
  */
uint32_t CRC_CalcBlockCRC(uint32_t* buffer, uint32_t length)
{
    uint32_t i;
    for (i = 0; i < length; i++) {
        CRC->DR = buffer[i];
    }
    return (CRC->DR);
}

/**
  * @brief  Returns the current CRC value.
  * @param  None.
  * @retval 32-bit CRC
  */
uint32_t CRC_GetCRC(void)
{
    return (CRC->DR);
}

/**
  * @brief  Stores a 8-bit data in the Independent Data(ID) register.
  * @param  id_value: 8-bit value to be stored in the ID register
  * @retval None.
  */
void CRC_SetIDRegister(uint8_t id_value)
{
    CRC->IDR = id_value;
}

/**
  * @brief  Returns the 8-bit data stored in the Independent Data(ID) register
  * @param  None.
  * @retval 8-bit value of the ID register
  */
uint8_t CRC_GetIDRegister(void)
{
    return (CRC->IDR);
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

