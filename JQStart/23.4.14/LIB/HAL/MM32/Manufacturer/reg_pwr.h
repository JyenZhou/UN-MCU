/***********************************************************************************************************************
    @file     reg_pwr.h
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

#ifndef __REG_PWR_H
#define __REG_PWR_H

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
  * @brief PWR Base Address Definition
  */
#define PWR_BASE                        (APB1PERIPH_BASE + 0x7000)              /*!< Base Address: 0x40007000 */

/**
  * @brief PWR Register Structure Definition
  */
typedef struct {
    union {
        __IO uint32_t CR;                                                            /*!< Control register,                              offset: 0x00 */
        __IO uint32_t CR1;
    };
    union {
        __IO uint32_t CSR;                                                           /*!< Control Status register                        offset: 0x04 */
        __IO uint32_t CSR1;
    };
} PWR_TypeDef;

/**
  * @brief PWR type pointer Definition
  */
#define PWR                             ((PWR_TypeDef*) PWR_BASE)

/**
  * @brief PWR_CR register Bit definition
  */
#define PWR_CR_LPDS_Pos                 (0)
#define PWR_CR_LPDS                     (0x01U << PWR_CR_LPDS_Pos)              /*!< Low power deepsleep */

/* legacy define */
#define PWR_CR_LDPS_Pos                 PWR_CR_LPDS_Pos
#define PWR_CR_LDPS                     PWR_CR_LPDS

#define PWR_CR_PDDS_Pos                 (1)
#define PWR_CR_PDDS                     (0x01U  << PWR_CR_PDDS_Pos)             /*!< Power Down Deepsleep */

#define PWR_CR_CWUF_Pos                 (2)
#define PWR_CR_CWUF                     (0x01U  << PWR_CR_CWUF_Pos)             /*!< Clear Standby Flag */
#define PWR_CR_CSBF_Pos                 (3)
#define PWR_CR_CSBF                     (0x01U  << PWR_CR_CSBF_Pos)             /*!< Clear Standby Flag */

#define PWR_CR_PVDE_Pos                 (4)
#define PWR_CR_PVDE                     (0x01U  << PWR_CR_PVDE_Pos)             /*!< Power Voltage Detector Enable */

#define PWR_CR_PLS_Pos                  (9)
#define PWR_CR_PLS                      (0x0FU  << PWR_CR_PLS_Pos)
#define PWR_CR_PLS_LEVEL0               (0x00U  << PWR_CR_PLS_Pos)              /*!< PVD level 1.8V */
#define PWR_CR_PLS_LEVEL1               (0x01U  << PWR_CR_PLS_Pos)              /*!< PVD level 2.1V */
#define PWR_CR_PLS_LEVEL2               (0x02U  << PWR_CR_PLS_Pos)              /*!< PVD level 2.4V */
#define PWR_CR_PLS_LEVEL3               (0x03U  << PWR_CR_PLS_Pos)              /*!< PVD level 2.7V */
#define PWR_CR_PLS_LEVEL4               (0x04U  << PWR_CR_PLS_Pos)              /*!< PVD level 3.0V */
#define PWR_CR_PLS_LEVEL5               (0x05U  << PWR_CR_PLS_Pos)              /*!< PVD level 3.3V */
#define PWR_CR_PLS_LEVEL6               (0x06U  << PWR_CR_PLS_Pos)              /*!< PVD level 3.6V */
#define PWR_CR_PLS_LEVEL7               (0x07U  << PWR_CR_PLS_Pos)              /*!< PVD level 3.9V */
#define PWR_CR_PLS_LEVEL8               (0x08U  << PWR_CR_PLS_Pos)              /*!< PVD level 4.2V */
#define PWR_CR_PLS_LEVEL9               (0x09U  << PWR_CR_PLS_Pos)              /*!< PVD level 4.5V */
#define PWR_CR_PLS_LEVEL10              (0x0AU  << PWR_CR_PLS_Pos)              /*!< PVD level 4.8V */

/* legacy define */
#define PWR_CR_PLS_0                    PWR_CR_PLS_LEVEL0
#define PWR_CR_PLS_1                    PWR_CR_PLS_LEVEL1
#define PWR_CR_PLS_2                    PWR_CR_PLS_LEVEL2
#define PWR_CR_PLS_3                    PWR_CR_PLS_LEVEL3
#define PWR_CR_PLS_4                    PWR_CR_PLS_LEVEL4
#define PWR_CR_PLS_5                    PWR_CR_PLS_LEVEL5
#define PWR_CR_PLS_6                    PWR_CR_PLS_LEVEL6
#define PWR_CR_PLS_7                    PWR_CR_PLS_LEVEL7
#define PWR_CR_PLS_8                    PWR_CR_PLS_LEVEL8
#define PWR_CR_PLS_9                    PWR_CR_PLS_LEVEL9
#define PWR_CR_PLS_10                   PWR_CR_PLS_LEVEL10

#define PWR_CR_PLS_1V8                  PWR_CR_PLS_LEVEL0
#define PWR_CR_PLS_2V1                  PWR_CR_PLS_LEVEL1
#define PWR_CR_PLS_2V4                  PWR_CR_PLS_LEVEL2
#define PWR_CR_PLS_2V7                  PWR_CR_PLS_LEVEL3
#define PWR_CR_PLS_3V0                  PWR_CR_PLS_LEVEL4
#define PWR_CR_PLS_3V3                  PWR_CR_PLS_LEVEL5
#define PWR_CR_PLS_3V6                  PWR_CR_PLS_LEVEL6
#define PWR_CR_PLS_3V9                  PWR_CR_PLS_LEVEL7
#define PWR_CR_PLS_4V2                  PWR_CR_PLS_LEVEL8
#define PWR_CR_PLS_4V5                  PWR_CR_PLS_LEVEL9
#define PWR_CR_PLS_4V8                  PWR_CR_PLS_LEVEL10

#define PWR_CR_STDBY_FS_WK_Pos          (14)
#define PWR_CR_STDBY_FS_WK              (0x03U  << PWR_CR_STDBY_FS_WK_Pos)      /*!< Rapid awakening standby mode selection */
#define PWR_CR_STDBY_FS_WK_MODE0        (0x00U  << PWR_CR_STDBY_FS_WK_Pos)      /*!< 9 LSI40K cycles to wake up the standby */
#define PWR_CR_STDBY_FS_WK_MODE1        (0x01U  << PWR_CR_STDBY_FS_WK_Pos)      /*!< 7 LSI40K cycles to wake up the standby */
#define PWR_CR_STDBY_FS_WK_MODE2        (0x02U  << PWR_CR_STDBY_FS_WK_Pos)      /*!< 5 LSI40K cycles to wake up the standby */
#define PWR_CR_STDBY_FS_WK_MODE3        (0x03U  << PWR_CR_STDBY_FS_WK_Pos)      /*!< 3 LSI40K cycles to wake up the standby */

/**
  * @brief PWR_CSR register Bit definition
  */
#define PWR_CSR_WUF_Pos                 (0)
#define PWR_CSR_WUF                     (0x01U  << PWR_CSR_WUF_Pos)             /*!< Wakeup Flag */
#define PWR_CSR_SBF_Pos                 (1)
#define PWR_CSR_SBF                     (0x01U  << PWR_CSR_SBF_Pos)             /*!< Standby Flag */
#define PWR_CSR_PVDO_Pos                (2)
#define PWR_CSR_PVDO                    (0x01U  << PWR_CSR_PVDO_Pos)            /*!< PVD Output */
#define PWR_CSR_EWUP_Pos                (8)
#define PWR_CSR_EWUP                    (0x01U  << PWR_CSR_EWUP_Pos)            /*!< Enable WKUP pin */

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
