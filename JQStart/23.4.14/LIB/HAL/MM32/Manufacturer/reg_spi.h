/***********************************************************************************************************************
    @file     reg_spi.h
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

#ifndef __REG_SPI_H
#define __REG_SPI_H

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
  * @brief SPI Base Address Definition
  */
#define SPI1_BASE                       (APB1PERIPH_BASE + 0x13000)             /*!< Base Address: 0x40013000 */

/**
  * @brief SPI Register Structure Definition
  */
typedef struct {
    union {
        __IO uint32_t TDR;                                                           /*!< SPI transmit data register,                    offset: 0x00 */
        __IO uint32_t TXREG;
    };
    union {
        __IO uint32_t RDR;                                                           /*!< SPI receive data register,                     offset: 0x04 */
        __IO uint32_t RXREG;
    };
    union {
        __IO uint32_t SR;                                                            /*!< SPI current state register,                    offset: 0x08 */
        __IO uint32_t CSTAT;
    };
    union {
        __IO uint32_t ISR;                                                           /*!< SPI interruput state register,                 offset: 0x0C */
        __IO uint32_t INTSTAT;
    };
    union {
        __IO uint32_t IER;                                                           /*!< SPI interruput enable register,                offset: 0x10 */
        __IO uint32_t INTEN;
    };
    union {
        __IO uint32_t ICR;                                                           /*!< SPI interruput control register,               offset: 0x14 */
        __IO uint32_t INTCLR;
    };
    union {
        __IO uint32_t GCR;                                                           /*!< SPI global control register,                   offset: 0x18 */
        __IO uint32_t GCTL;
    };
    union {
        __IO uint32_t CCR;                                                           /*!< SPI common control register,                   offset: 0x1C */
        __IO uint32_t CCTL;
    };
    union {
        __IO uint32_t BRR;                                                           /*!< SPI baud rate control register,                offset: 0x20 */
        __IO uint32_t SPBRG;
    };
    union {
        __IO uint32_t RDNR;                                                          /*!< SPI receive data number register,              offset: 0x24 */
        __IO uint32_t RXDNR;
    };
    union {
        __IO uint32_t NSSR;                                                          /*!< SPI chip select register,                      offset: 0x28 */
        __IO uint32_t SCSR;
    };
    union {
        __IO uint32_t ECR;                                                           /*!< SPI extand control register,                   offset: 0x2C */
        __IO uint32_t EXTCTL;
    };
} SPI_TypeDef;

/**
  * @brief SPI type pointer Definition
  */
#define SPI1                            ((SPI_TypeDef*) SPI1_BASE)
/**
  * @brief SPI_TDR Register Bit Definition
  */
#define SPI_TDR_TXREG_Pos               (0)
#define SPI_TDR_TXREG                   (0xFFFFFFFFU << SPI_TDR_TXREG_Pos)      /*!< Transmit data register */

/**
  * @brief SPI_RDR Register Bit Definition
  */
#define SPI_RDR_RXREG_Pos               (0)
#define SPI_RDR_RXREG                   (0xFFFFFFFFU << SPI_RDR_RXREG_Pos)      /*!< Receive data register */

/**
  * @brief SPI_SR Register Bit Definition
  */
#define SPI_SR_TXEPT_Pos                (0)
#define SPI_SR_TXEPT                    (0x01U << SPI_SR_TXEPT_Pos)             /*!< Transmitter empty bit */
#define SPI_SR_RXAVL_Pos                (1)
#define SPI_SR_RXAVL                    (0x01U << SPI_SR_RXAVL_Pos)             /*!< Receive available byte data message */
#define SPI_SR_TXFULL_Pos               (2)
#define SPI_SR_TXFULL                   (0x01U << SPI_SR_TXFULL_Pos)            /*!< Transmitter FIFO full status bit */
#define SPI_SR_RXAVL_4BYTE_Pos          (3)
#define SPI_SR_RXAVL_4BYTE              (0x01U << SPI_SR_RXAVL_4BYTE_Pos)       /*!< Receive available 4 byte data message */
#define SPI_SR_TXFADDR_Pos              (4)
#define SPI_SR_TXFADDR                  (0x0FU << SPI_SR_TXFADDR_Pos)           /*!< Transmit FIFO address */
#define SPI_SR_RXFADDR_Pos              (8)
#define SPI_SR_RXFADDR                  (0x0FU << SPI_SR_RXFADDR_Pos)           /*!< Receive FIFO address */

/**
  * @brief SPI_ISR Register Bit Definition
  */
#define SPI_ISR_TX_INTF_Pos             (0)
#define SPI_ISR_TX_INTF                 (0x01U << SPI_ISR_TX_INTF_Pos)          /*!< Transmit FIFO available interrupt flag bit */
#define SPI_ISR_RX_INTF_Pos             (1)
#define SPI_ISR_RX_INTF                 (0x01U << SPI_ISR_RX_INTF_Pos)          /*!< Receive data available interrupt flag bit */
#define SPI_ISR_UNDERRUN_INTF_Pos       (2)
#define SPI_ISR_UNDERRUN_INTF           (0x01U << SPI_ISR_UNDERRUN_INTF_Pos)    /*!< SPI underrun interrupt flag bit */
#define SPI_ISR_RXOERR_INTF_Pos         (3)
#define SPI_ISR_RXOERR_INTF             (0x01U << SPI_ISR_RXOERR_INTF_Pos)      /*!< Receive overrun error interrupt flag bit */
#define SPI_ISR_RXMATCH_INTF_Pos        (4)
#define SPI_ISR_RXMATCH_INTF            (0x01U << SPI_ISR_RXMATCH_INTF_Pos)     /*!< Receive data match the RXDNR number, the receive process will be completed and generate the interrupt */
#define SPI_ISR_RXFULL_INTF_Pos         (5)
#define SPI_ISR_RXFULL_INTF             (0x01U << SPI_ISR_RXFULL_INTF_Pos)      /*!< RX FIFO full interrupt flag bit */
#define SPI_ISR_TXEPT_INTF_Pos          (6)
#define SPI_ISR_TXEPT_INTF              (0x01U << SPI_ISR_TXEPT_INTF_Pos)       /*!< Transmitter empty interrupt flag bit */

/**
  * @brief SPI_IER Register Bit Definition
  */
#define SPI_IER_TX_IEN_Pos              (0)
#define SPI_IER_TX_IEN                  (0x01U << SPI_IER_TX_IEN_Pos)           /*!< Transmit FIFO empty interrupt enable bit */
#define SPI_IER_RX_IEN_Pos              (1)
#define SPI_IER_RX_IEN                  (0x01U << SPI_IER_RX_IEN_Pos)           /*!< Receive FIFO interrupt enable bit */
#define SPI_IER_UNDERRUN_IEN_Pos        (2)
#define SPI_IER_UNDERRUN_IEN            (0x01U << SPI_IER_UNDERRUN_IEN_Pos)     /*!< Transmitter underrun interrupt enable bit */
#define SPI_IER_RXOERR_IEN_Pos          (3)
#define SPI_IER_RXOERR_IEN              (0x01U << SPI_IER_RXOERR_IEN_Pos)       /*!< Overrun error interrupt enable bit */
#define SPI_IER_RXMATCH_IEN_Pos         (4)
#define SPI_IER_RXMATCH_IEN             (0x01U << SPI_IER_RXMATCH_IEN_Pos)      /*!< Receive data complete interrupt enable bit */
#define SPI_IER_RXFULL_IEN_Pos          (5)
#define SPI_IER_RXFULL_IEN              (0x01U << SPI_IER_RXFULL_IEN_Pos)       /*!< Receive FIFO full interrupt enable bit */
#define SPI_IER_TXEPT_IEN_Pos           (6)
#define SPI_IER_TXEPT_IEN               (0x01U << SPI_IER_TXEPT_IEN_Pos)        /*!< Transmit empty interrupt enable bit */

/**
  * @brief SPI_ICR Register Bit Definition
  */
#define SPI_ICR_TX_ICLR_Pos             (0)
#define SPI_ICR_TX_ICLR                 (0x01U << SPI_ICR_TX_ICLR_Pos)          /*!< Transmitter FIFO empty interrupt clear bit */
#define SPI_ICR_RX_ICLR_Pos             (1)
#define SPI_ICR_RX_ICLR                 (0x01U << SPI_ICR_RX_ICLR_Pos)          /*!< Receive interrupt clear bit */
#define SPI_ICR_UNDERRUN_ICLR_Pos       (2)
#define SPI_ICR_UNDERRUN_ICLR           (0x01U << SPI_ICR_UNDERRUN_ICLR_Pos)    /*!< Transmitter underrun interrupt clear bit */
#define SPI_ICR_RXOERR_ICLR_Pos         (3)
#define SPI_ICR_RXOERR_ICLR             (0x01U << SPI_ICR_RXOERR_ICLR_Pos)      /*!< Overrun error interrupt clear bit */
#define SPI_ICR_RXMATCH_ICLR_Pos        (4)
#define SPI_ICR_RXMATCH_ICLR            (0x01U << SPI_ICR_RXMATCH_ICLR_Pos)     /*!< Receive completed interrupt clear bit */
#define SPI_ICR_RXFULL_ICLR_Pos         (5)
#define SPI_ICR_RXFULL_ICLR             (0x01U << SPI_ICR_RXFULL_ICLR_Pos)      /*!< Receiver buffer full interrupt clear bit */
#define SPI_ICR_TXEPT_ICLR_Pos          (6)
#define SPI_ICR_TXEPT_ICLR              (0x01U << SPI_ICR_TXEPT_ICLR_Pos)       /*!< Transmitter empty interrupt clear bit */

/**
  * @brief SPI_GCR Register Bit Definition
  */
#define SPI_GCR_SPIEN_Pos               (0)
#define SPI_GCR_SPIEN                   (0x01U << SPI_GCR_SPIEN_Pos)            /*!< SPI select bit */
#define SPI_GCR_IEN_Pos                 (1)
#define SPI_GCR_IEN                     (0x01U << SPI_GCR_IEN_Pos)              /*!< SPI interrupt enable bit */
#define SPI_GCR_MODE_Pos                (2)
#define SPI_GCR_MODE                    (0x01U << SPI_GCR_MODE_Pos)             /*!< Master mode bit */
#define SPI_GCR_TXEN_Pos                (3)
#define SPI_GCR_TXEN                    (0x01U << SPI_GCR_TXEN_Pos)             /*!< Transmit enable bit */
#define SPI_GCR_RXEN_Pos                (4)
#define SPI_GCR_RXEN                    (0x01U << SPI_GCR_RXEN_Pos)             /*!< Receive enable bit */

#define SPI_GCR_RXTLF_Pos               (5)
#define SPI_GCR_RXTLF                   (0x03U << SPI_GCR_RXTLF_Pos)            /*!< RX FIFO trigger level bit */
#define SPI_GCR_RXTLF_One               (0x00U << SPI_GCR_RXTLF_Pos)            /*!< */
#define SPI_GCR_RXTLF_Half              (0x01U << SPI_GCR_RXTLF_Pos)            /*!< */

#define SPI_GCR_TXTLF_Pos               (7)
#define SPI_GCR_TXTLF                   (0x03U << SPI_GCR_TXTLF_Pos)            /*!< TX FIFO trigger level bit */
#define SPI_GCR_TXTLF_One               (0x00U << SPI_GCR_TXTLF_Pos)            /*!< */
#define SPI_GCR_TXTLF_Half              (0x01U << SPI_GCR_TXTLF_Pos)            /*!< */
#define SPI_GCR_NSS_Pos                 (10)
#define SPI_GCR_NSS                     (0x01U << SPI_GCR_NSS_Pos)              /*!< NSS select signal that from software or hardware */
#define SPI_GCR_DWSEL_Pos               (11)
#define SPI_GCR_DWSEL                   (0x01U << SPI_GCR_DWSEL_Pos)            /*!< Valid byte or double-word data select signal */

#define SPI_GCR_NSSTOG_Pos              (12)
#define SPI_GCR_NSSTOG                  (0x01U << SPI_GCR_NSSTOG_Pos)           /*!< Slave select toggle */

/**
  * @brief SPI_CCR Register Bit Definition
  */
#define SPI_CCR_CPHA_Pos                (0)
#define SPI_CCR_CPHA                    (0x01U << SPI_CCR_CPHA_Pos)             /*!< Clock phase select bit */
#define SPI_CCR_CPOL_Pos                (1)
#define SPI_CCR_CPOL                    (0x01U << SPI_CCR_CPOL_Pos)             /*!< Clock polarity select bit */
#define SPI_CCR_LSBFE_Pos               (2)
#define SPI_CCR_LSBFE                   (0x01U << SPI_CCR_LSBFE_Pos)            /*!< LSI first enable bit */
#define SPI_CCR_SPILEN_Pos              (3)
#define SPI_CCR_SPILEN                  (0x01U << SPI_CCR_SPILEN_Pos)           /*!< SPI character length bit */
#define SPI_CCR_RXEDGE_Pos              (4)
#define SPI_CCR_RXEDGE                  (0x01U << SPI_CCR_RXEDGE_Pos)           /*!< Receive data edge select */
#define SPI_CCR_TXEDGE_Pos              (5)
#define SPI_CCR_TXEDGE                  (0x01U << SPI_CCR_TXEDGE_Pos)           /*!< Transmit data edge select */

#define SPI_CCR_CPHASEL_Pos             (6)
#define SPI_CCR_CPHASEL                 (0x01U << SPI_CCR_CPHASEL)              /*!< CPHA polarity select */

#define SPI_CCR_HISPD_Pos               (7)
#define SPI_CCR_HISPD                   (0x01U << SPI_CCR_HISPD)                /*!< High speed slave mode */

/**
  * @brief SPI_BRR Register Bit Definition
  */
#define SPI_BRR_DIVF_Pos                (0)
#define SPI_BRR_DIVF                    (0xFFFFU << SPI_BRR_DIVF_Pos)           /*!< SPI baud rate control register for baud rate */

/**
  * @brief SPI_RDNR Register Bit Definition
  */
#define SPI_RDNR_RDN_Pos                (0)
#define SPI_RDNR_RDN                    (0xFFFFU << SPI_RDNR_RDN_Pos)           /*!< The register is used to hold a count of to be received bytes in next receive process */

/**
  * @brief SPI_NSSR Register Bit Definition
  */
#define SPI_NSSR_NSS_Pos                (0)
#define SPI_NSSR_NSS                    (0x01U << SPI_NSSR_NSS_Pos)             /*!< Chip select output signal in Master mode */

/**
  * @brief SPI_ECR Register Bit Definition
  */
#define SPI_ECR_EXTLEN_Pos              (0)
#define SPI_ECR_EXTLEN                  (0x1FU << SPI_ECR_EXTLEN_Pos)           /*!< control SPI data length */

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
