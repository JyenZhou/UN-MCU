/*
 *******************************************************************************
    @file     reg_usart.h
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

#ifndef __REG_USART_H
#define __REG_USART_H

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
  * @brief USART Base Address Definition
  */
#define USART1_BASE                      (APB1PERIPH_BASE + 0x13800)             /*!< Base Address: 0x40013800 */
#define USART2_BASE                      (APB1PERIPH_BASE + 0x4400)              /*!< Base Address: 0x40004400 */

/**
  * @brief USART Register Structure Definition
  */
typedef struct {
    __IO uint32_t SR;                                                           /*!< Status  Register,                       offset: 0x00 */
    __IO uint32_t DR;                                                           /*!< Data sending and receiving Register,    offset: 0x04 */
    __IO uint32_t BRR;                                                          /*!< Baud rate control Register,             offset: 0x08 */
    __IO uint32_t CR1;                                                          /*!< Control register 1,                     offset: 0x0C */
    __IO uint32_t CR2;                                                          /*!< Control register 2,                     offset: 0x10 */
    __IO uint32_t CR3;                                                          /*!< Control register 3,                     offset: 0x14 */
} USART_TypeDef;

/**
  * @brief USART type pointer Definition
  */
#define USART1                           ((USART_TypeDef*) USART1_BASE)
#define USART2                           ((USART_TypeDef*) USART2_BASE)

/**
  * @brief USART_SR Register Bit Definition
  */
#define USART_SR_PE_Pos                 (0)
#define USART_SR_PE_Msk                 (0x01U << USART_SR_PE_Pos)              /*!< Parity error */
#define USART_SR_FE_Pos                 (1)
#define USART_SR_FE_Msk                 (0x01U << USART_SR_FE_Pos)              /*!< Framing error */
#define USART_SR_NF_Pos                 (2)
#define USART_SR_NF_Msk                 (0x01U << USART_SR_NF_Pos)              /*!< Noise detected flag */
#define USART_SR_ORE_Pos                (3)
#define USART_SR_ORE_Msk                (0x01U << USART_SR_ORE_Pos)             /*!< Overrun error */
#define USART_SR_IDLE_Pos               (4)
#define USART_SR_IDLE_Msk               (0x01U << USART_SR_IDLE_Pos)            /*!< IDLE frame detected */
#define USART_SR_RXNE_Pos               (5)
#define USART_SR_RXNE_Msk               (0x01U << USART_SR_RXNE_Pos)            /*!< Read data register not empty */
#define USART_SR_TC_Pos                 (6)
#define USART_SR_TC_Msk                 (0x01U << USART_SR_TC_Pos)              /*!< Transmission complete */
#define USART_SR_TXE_Pos                (7)
#define USART_SR_TXE_Msk                (0x01U << USART_SR_TXE_Pos)             /*!< Transmit data register Empty */

#define USART_SR_CTS_Pos                (9)
#define USART_SR_CTS_Msk                (0x01U << USART_SR_CTS_Pos)             /*!< CTS MARK */


/**
  * @brief USART_DR Register Bit Definition
  */
#define USART_DR_DR_Pos                 (0)
#define USART_DR_DR_Msk                 (0x1FFU << USART_DR_DR_Pos)             /*!< Send/receive data register */

/**
  * @brief USART_BRR Register Bit Definition
  */
#define USART_BRR_FFD_Pos               (0)
#define USART_BRR_FFD_Msk               (0x0FU << USART_BRR_FFD_Pos)            /*!< Fraction frequency division of baudrate */
#define USART_BRR_MFD_Pos               (4)
#define USART_BRR_MFD_Msk               (0xFFFFU << USART_BRR_MFD_Pos)          /*!< Mantissa frequency division of baudrate */

/**
  * @brief USART_CR1 Register Bit Definition
  */
#define USART_CR1_SBK_Pos               (0)
#define USART_CR1_SBK_Msk               (0x01U << USART_CR1_SBK_Pos)            /*!< Send break */

#define USART_CR1_RE_Pos                (2)
#define USART_CR1_RE_Msk                (0x01U << USART_CR1_RE_Pos)             /*!< Receiver enable */
#define USART_CR1_TE_Pos                (3)
#define USART_CR1_TE_Msk                (0x01U << USART_CR1_TE_Pos)             /*!< Transmitter enable */
#define USART_CR1_IDLEIEN_Pos           (4)
#define USART_CR1_IDLEIEN_Msk           (0x01U << USART_CR1_IDLEIEN_Pos)        /*!< IDLE interrupt enable */
#define USART_CR1_RXNEIEN_Pos           (5)
#define USART_CR1_RXNEIEN_Msk           (0x01U << USART_CR1_RXNEIEN_Pos)        /*!< RXNE interrupt enable */
#define USART_CR1_TCIEN_Pos             (6)
#define USART_CR1_TCIEN_Msk             (0x01U << USART_CR1_TCIEN_Pos)          /*!< Transmission complete Interrupt enable */
#define USART_CR1_TXEIEN_Pos            (7)
#define USART_CR1_TXEIEN_Msk            (0x01U << USART_CR1_TXEIEN_Pos)         /*!< TXE interrupt enable */
#define USART_CR1_PEIEN_Pos             (8)
#define USART_CR1_PEIEN_Msk             (0x01U << USART_CR1_PEIEN_Pos)          /*!< PE interrupt enable */
#define USART_CR1_PS_Pos                (9)
#define USART_CR1_PS_Msk                (0x01U << USART_CR1_PS_Pos)             /*!< Check mode */
#define USART_CR1_PCE_Pos               (10)
#define USART_CR1_PCE_Msk               (0x01U << USART_CR1_PCE_Pos)            /*!< Parity control enable */

#define USART_CR1_DL_Pos                (12)
#define USART_CR1_DL_Msk                (0x01U << USART_CR1_DL_Pos)             /*!< Data length */
#define USART_CR1_UE_Pos                (13)
#define USART_CR1_UE_Msk                (0x01U << USART_CR1_UE_Pos)             /*!< USART enable */

#define USART_CR1_OVER8_Pos             (15)
#define USART_CR1_OVER8_MSK             (0x01U << USART_CR1_OVER8_Pos)          /*! Oversampling mode */
#define USART_CR1_MLS_Pos               (16)
#define USART_CR1_MLS_Msk               (0x01U << USART_CR1_MLS_Pos)            /*!< MSB\LSB Mode selection bit */
#define USART_CR1_SAS_Pos               (17)
#define USART_CR1_SAS_Msk               (0x01U << USART_CR1_SAS_Pos)            /*!< synchronous/asynchronous mode selection */

/**
  * @brief USART_CR2 Register Bit Definition
  */
#define USART_CR2_CPHA_Pos              (9)
#define USART_CR2_CPHA_Msk              (0x01U << USART_CR2_CPHA_Pos)           /*!< Clock PHAse */
#define USART_CR2_CPOL_Pos              (10)
#define USART_CR2_CPOL_Msk              (0x01U << USART_CR2_CPOL_Pos)           /*!< Clock POLarity */

#define USART_CR2_STOP_Pos              (12)
#define USART_CR2_STOP_Msk              (0x03U << USART_CR2_STOP_Pos)           /*!< STOP bit */

#define USART_CR2_SWAP_Pos              (15)
#define USART_CR2_SWAP_MsK              (0x01U << USART_CR2_SWAP_Pos)           /*!< The input and output pins are swapped */

/**
  * @brief USART_CR3 Register Bit Definition
  */
#define USART_CR3_ERRIEN_Pos            (0)
#define USART_CR3_ERRIEN_Msk            (0x01U << USART_CR3_ERRIEN_Pos)         /*!< Error interrupt enable */

#define USART_CR3_HDSEL_Pos             (3)
#define USART_CR3_HDSEL_Msk             (0x01U << USART_CR3_HDSEL_Pos)          /*!< Half-duplex selection */

#define USART_CR3_RTSE_Pos              (8)
#define USART_CR3_RTSE_Msk              (0x01U << USART_CR3_RTSE_Pos)           /*!< RTS enable */
#define USART_CR3_CTSE_Pos              (9)
#define USART_CR3_CTSE_Msk              (0x01U << USART_CR3_CTSE_Pos)           /*!< CTS enable */
#define USART_CR3_CTSIEN_Pos            (10)
#define USART_CR3_CTSIEN_Msk            (0x01U << USART_CR3_CTSIEN_Pos)         /*!< CTS interrupt enable */
#define USART_CR3_ONEBIT_Pos            (11)
#define USART_CR3_ONEBIT_Msk            (0x01U << USART_CR3_ONEBIT_Pos)         /*!< One sample bit method enable */

#define USART_CR3_CKINE_Pos             (16)
#define USART_CR3_CKINE_Msk             (0x01U << USART_CR3_CKINE_Pos)          /*!< Clock input permission in synchronous mode */

#define USART_CR3_RXTOG_Pos             (28)
#define USART_CR3_RXTOG_Msk             (0x01U << USART_CR3_RXTOG_Pos)          /*!< Receiving reverse bit */
#define USART_CR3_TXTOG_Pos             (29)
#define USART_CR3_TXTOG_Msk             (0x01U << USART_CR3_TXTOG_Pos)          /*!< Send fetch reverse bit */


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
