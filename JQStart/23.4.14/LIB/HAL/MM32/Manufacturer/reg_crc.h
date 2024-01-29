/***********************************************************************************************************************
    @file     reg_crc.h
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

#ifndef __REG_CRC_H
#define __REG_CRC_H

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
  * @brief CRC Base Address Definition
  */
#define CRC_BASE                        (AHBPERIPH_BASE + 0x3000)               /*!< Base Address: 0x40023000 */

/**
  * @brief CRC Register Structure Definition
  */
typedef struct {
    __IO uint32_t   DR;                                                              /*!< CRC data register,                             offset: 0x00 */
    __IO uint32_t   IDR;                                                             /*!< CRC independent data register,                 offset: 0x04 */
    __IO uint32_t   CR;                                                              /*!< CRC control register,                          offset: 0x08 */
} CRC_TypeDef;

/**
  * @brief CRC type pointer Definition
  */
#define CRC                             ((CRC_TypeDef*) CRC_BASE)

/**
  * @brief CRC_DR Register Bit Definition
  */
#define CRC_DR_DATA_Pos                 (0)
#define CRC_DR_DATA                     (0xFFFFFFFFU << CRC_DR_DATA_Pos)        /*!< Data register bits */
/**
  * @brief CRC_IDR Register Bit Definition
  */
#define CRC_IDR_DATA_Pos                (0)
#define CRC_IDR_DATA                    (0xFFU << CRC_IDR_DATA_Pos)             /*!< General-purpose 8-bit data register bits */

/**
  * @brief CRC_CTRL Register Bit Definition
  */
#define CRC_CR_RESET_Pos                (0)
#define CRC_CR_RESET                    (0x01U << CRC_CR_RESET_Pos)             /*!< RESET bit */

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
