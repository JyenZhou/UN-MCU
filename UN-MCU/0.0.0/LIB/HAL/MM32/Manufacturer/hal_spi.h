/***********************************************************************************************************************
    @file     hal_spi.h
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
#ifndef __HAL_SPI_H
#define __HAL_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

/* Files includes ------------------------------------------------------------*/
#include "mm32_device.h"

/** @addtogroup MM32_Hardware_Abstract_Layer
  * @{
  */

/** @defgroup SPI_HAL
  * @brief SPI HAL modules
  * @{
  */

/** @defgroup SPI_Exported_Types
  * @{
  */

/**
  * @brief  SPI mode enum definition
  */
typedef enum {
    SPI_Mode_Slave                      = 0x0000,                               /*!< SPI slave mode */
    SPI_Mode_Master                     = SPI_GCR_MODE                          /*!< SPI master mode */
} SPI_Mode_TypeDef;

/**
  * @brief  SPI data size enum definition
  */
typedef enum {
    SPI_DataSize_8b                     = 0x0000,                               /*!< 8 bits valid data */
    SPI_DataSize_32b                    = SPI_GCR_DWSEL                         /*!< 32 bits valid data */
} SPI_DataSize_TypeDef;

/**
  * @brief  SPI clock polarity enum definition
  */
typedef enum {
    SPI_CPOL_Low                        = 0x0000,                               /*!< The clock is low in idle state. */
    SPI_CPOL_High                       = SPI_CCR_CPOL                          /*!< The clock is high in idle state. */
} SPI_CPOL_TypeDef;

/**
  * @brief  SPI clock phase enum definition
  */
typedef enum {
    SPI_CPHA_2Edge                      = 0x0000,                               /*!< Data sampling starts from the second clock edge. */
    SPI_CPHA_1Edge                      = SPI_CCR_CPHA                          /*!< Data sampling starts from the first clock edge. */
} SPI_CPHA_TypeDef;

/**
  * @brief  SPI nss control mode enum definition
  */
typedef enum {
    SPI_NSS_Soft                        = 0x0000,
    SPI_NSS_Hard                        = SPI_GCR_NSS
} SPI_NSS_TypeDef;

/**
  * @brief  SPI baud rate prescaler enum definition
  */
typedef enum {
    SPI_BaudRatePrescaler_2             = 0x0002,                               /*!< SCK clock devide by 2 */
    SPI_BaudRatePrescaler_4             = 0x0004,                               /*!< SCK clock devide by 4 */
    SPI_BaudRatePrescaler_8             = 0x0008,                               /*!< SCK clock devide by 7 */
    SPI_BaudRatePrescaler_16            = 0x0010,                               /*!< SCK clock devide by 16 */
    SPI_BaudRatePrescaler_32            = 0x0020,                               /*!< SCK clock devide by 32 */
    SPI_BaudRatePrescaler_64            = 0x0040,                               /*!< SCK clock devide by 64 */
    SPI_BaudRatePrescaler_128           = 0x0080,                               /*!< SCK clock devide by 128 */
    SPI_BaudRatePrescaler_256           = 0x0100                                /*!< SCK clock devide by 256 */
} SPI_BaudRatePrescaler_TypeDef;

/**
  * @brief  SPI first bit enum definition
  */
typedef enum {
    SPI_FirstBit_MSB                    = 0x0000,                               /*!< Data transfers start from MSB */
    SPI_FirstBit_LSB                    = SPI_CCR_LSBFE                         /*!< Data transfers start from LSB */
} SPI_FirstBit_TypeDef;

/**
  * @brief  SPI FIFO trigger level enum definition
  */
typedef enum {
    SPI_RXTLF                           = SPI_GCR_RXTLF_Half,                   /*!< RX FIFO trigger level */
    SPI_TXTLF                           = SPI_GCR_TXTLF_Half                    /*!< TX FIFO trigger level */
} SPI_TLF_TypeDef;

/**
  * @brief  SPI bit derection enum definition
  */
typedef enum {
    SPI_Direction_Rx,                                                           /*!< Receive enable */
    SPI_Direction_Tx,                                                           /*!< Transmit enable */
    SPI_Disable_Rx,                                                             /*!< Receive disable */
    SPI_Disable_Tx                                                              /*!< Transmit disable */
} SPI_Direction_TypeDef;

/**
  * @brief  SPI flag enum definition
  */
typedef enum {
    SPI_FLAG_RXAVL                      = SPI_SR_RXAVL,                         /*!< Receive 1 byte available data flag */
    SPI_FLAG_TXEPT                      = SPI_SR_TXEPT,                         /*!< Transmitter empty flag */
    SPI_FLAG_TXFULL                     = SPI_SR_TXFULL,                        /*!< Transmitter FIFO full status flag */
    SPI_FLAG_RXAVL_4BYTE                = SPI_SR_RXAVL_4BYTE                    /*!< Receive 4 bytes available data flag */
} SPI_FLAG_TypeDef;

/**
  * @brief  SPI slave mode data edge adjust enum definition
  */
typedef enum {
    SPI_SlaveAdjust_LOW,                                                        /*!< SPI slave mode data edge adjust in low speed mode */
    SPI_SlaveAdjust_FAST                                                        /*!< SPI slave mode data edge adjust in fast speed mode */
} SPI_SlaveAdjust_TypeDef;

/**
  * @brief  SPI data edge adjust enum definition
  */
typedef enum {
    SPI_DataEdgeAdjust_LOW,                                                     /*!< SPI data edge adjust in low speed mode */
    SPI_DataEdgeAdjust_FAST                                                     /*!< SPI data edge adjust in fast speed mode */
} SPI_DataEdgeAdjust_TypeDef;

/**
  * @brief  SPI interruput enum definition
  */
typedef enum {
    SPI_IT_TXEPT                        = 0x40,                                 /*!< Transmitter empty interrupt */
    SPI_IT_RXFULL                       = 0x20,                                 /*!< RX FIFO full interrupt */
    SPI_IT_RXMATCH                      = 0x10,                                 /*!< Receive data match the RXDNR number interrut */
    SPI_IT_RXOERR                       = 0x08,                                 /*!< Receive overrun error interrupt */
    SPI_IT_UNDERRUN                     = 0x04,                                 /*!< Underrun interrupt */
    SPI_IT_RX                           = 0x02,                                 /*!< Receive available data interrupt */
    SPI_IT_TX                           = 0x01                                  /*!< Transmit FIFO available interrupt */
} SPI_IT_TypeDef;

/**
  * @brief  SPI Init structure definition
  */
typedef struct {
    SPI_Mode_TypeDef                    SPI_Mode;                               /*!< Specifies the SPI operating mode */
    SPI_DataSize_TypeDef                SPI_DataSize;                           /*!< Specifies the SPI available data size */
    uint8_t                                  SPI_DataWidth;                          /*!< SPI data length */
    SPI_CPOL_TypeDef                    SPI_CPOL;                               /*!< Specifies the serial clock steady state */
    SPI_CPHA_TypeDef                    SPI_CPHA;                               /*!< Specifies the clock active edge for the bit capture */
    SPI_NSS_TypeDef                     SPI_NSS;                                /*!< Specifies whether the NSS signal is managed by hardware or by software */
    SPI_BaudRatePrescaler_TypeDef       SPI_BaudRatePrescaler;                  /*!< Specifies the Baud Rate prescaler value which will be */
    /*!< used to configure the transmit and receive SCK clock */
    SPI_FirstBit_TypeDef                SPI_FirstBit;                           /*!< Specifies whether data transfers start from MSB or LSB bit */
    /*!< uint16_t     SPI_length; */
} SPI_InitTypeDef;
/**
  * @}
  */

/** @defgroup SPI_Exported_Constants
  * @{
  */

/** @defgroup SPI_Register_Mask
  * @{
  */
#define GCR_Mask                        ((uint32_t)0x0FFF)
#define CCR_Mask                        ((uint32_t)0x003F)
#define BRR_Mask                        ((uint32_t)0xFFFF)
#define ECR_Mask                        ((uint32_t)0x001F)

/**
  * @}
  */

/* SPI_7bit_8bit data width --------------------------------------------------*/
#define SPI_DataWidth_1b                ((uint16_t)0x0001)
#define SPI_DataWidth_2b                ((uint16_t)0x0002)
#define SPI_DataWidth_3b                ((uint16_t)0x0003)
#define SPI_DataWidth_4b                ((uint16_t)0x0004)
#define SPI_DataWidth_5b                ((uint16_t)0x0005)
#define SPI_DataWidth_6b                ((uint16_t)0x0006)
#define SPI_DataWidth_7b                ((uint16_t)0x0007)
#define SPI_DataWidth_8b                ((uint16_t)0x0008)
#define SPI_DataWidth_9b                ((uint16_t)0x0009)
#define SPI_DataWidth_10b               ((uint16_t)0x000a)
#define SPI_DataWidth_11b               ((uint16_t)0x000b)
#define SPI_DataWidth_12b               ((uint16_t)0x000c)
#define SPI_DataWidth_13b               ((uint16_t)0x000d)
#define SPI_DataWidth_14b               ((uint16_t)0x000e)
#define SPI_DataWidth_15b               ((uint16_t)0x000f)
#define SPI_DataWidth_16b               ((uint16_t)0x0010)
#define SPI_DataWidth_17b               ((uint16_t)0x0011)
#define SPI_DataWidth_18b               ((uint16_t)0x0012)
#define SPI_DataWidth_19b               ((uint16_t)0x0013)
#define SPI_DataWidth_20b               ((uint16_t)0x0014)
#define SPI_DataWidth_21b               ((uint16_t)0x0015)
#define SPI_DataWidth_22b               ((uint16_t)0x0016)
#define SPI_DataWidth_23b               ((uint16_t)0x0017)
#define SPI_DataWidth_24b               ((uint16_t)0x0018)
#define SPI_DataWidth_25b               ((uint16_t)0x0019)
#define SPI_DataWidth_26b               ((uint16_t)0x001a)
#define SPI_DataWidth_27b               ((uint16_t)0x001b)
#define SPI_DataWidth_28b               ((uint16_t)0x001c)
#define SPI_DataWidth_29b               ((uint16_t)0x001d)
#define SPI_DataWidth_30b               ((uint16_t)0x001e)
#define SPI_DataWidth_31b               ((uint16_t)0x001f)
#define SPI_DataWidth_32b               ((uint16_t)0x0000)

/**
  * @}
  */

/** @defgroup SPI_Exported_Variables
  * @{
  */

#ifdef _HAL_SPI_C_
#define GLOBAL
#else
#define GLOBAL extern
#endif

#undef GLOBAL

/**
  * @}
  */

/** @defgroup SPI_Exported_Functions
  * @{
  */

void SPI_DeInit(SPI_TypeDef* spi);
void SPI_Init(SPI_TypeDef* spi, SPI_InitTypeDef* init_struct);
void SPI_StructInit(SPI_InitTypeDef* init_struct);
void SPI_Cmd(SPI_TypeDef* spi, FunctionalState state);
void SPI_ITConfig(SPI_TypeDef* spi, uint8_t interrupt, FunctionalState state);
void SPI_FifoTrigger(SPI_TypeDef* spi, SPI_TLF_TypeDef fifo_trigger_value, FunctionalState state);
void SPI_SendByte(SPI_TypeDef* spi, uint8_t data);
void SPI_SendData(SPI_TypeDef* spi, uint32_t data);
void SPI_CSInternalSelected(SPI_TypeDef* spi, FunctionalState state);
void SPI_NSSInternalSoftwareConfig(SPI_TypeDef* spi, SPI_NSS_TypeDef nss);

void SPI_BiDirectionalLineConfig(SPI_TypeDef* spi, SPI_Direction_TypeDef direction);
void SPI_ClearITPendingBit(SPI_TypeDef* spi, SPI_IT_TypeDef interrupt);
void SPI_RxBytes(SPI_TypeDef* spi, uint16_t number);
void SPI_SlaveAdjust(SPI_TypeDef* spi, SPI_SlaveAdjust_TypeDef adjust_value);

bool SPI_DataSizeConfig(SPI_TypeDef* spi, uint8_t data_size);
void SPI_DataSizeTypeConfig(SPI_TypeDef* spi, SPI_DataSize_TypeDef SPI_DataSize);
uint8_t SPI_ReceiveByte(SPI_TypeDef* spi);
uint32_t SPI_ReceiveData(SPI_TypeDef* spi);

FlagStatus SPI_GetFlagStatus(SPI_TypeDef* spi, SPI_FLAG_TypeDef flag);

ITStatus SPI_GetITStatus(SPI_TypeDef* spi, SPI_IT_TypeDef interrupt);

/* Extended function interface -----------------------------------------------*/
void exSPI_ITCmd(SPI_TypeDef* spi, FunctionalState state);
void exSPI_ITConfig(SPI_TypeDef* spi, SPI_IT_TypeDef interrput, FunctionalState state);
void exSPI_CSInternalSelected(SPI_TypeDef* spi, FunctionalState state);
void exSPI_DataEdgeAdjust(SPI_TypeDef* spi, SPI_DataEdgeAdjust_TypeDef adjust_value);
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
#endif /* __HAL_SPI_H --------------------------------------------------------*/
/** --------------------------------------------------------------------------*/
