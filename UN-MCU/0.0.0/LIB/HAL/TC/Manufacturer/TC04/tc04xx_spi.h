/*
 * @Description: 
 * @Author: wangshi@tinychip.com.cn
 * @Date: 2023-02-28 10:04:45
 * @Disclaimer: 
 * 	* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * 	* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * 	* TIME. AS A RESULT, TINY CHIP SHALL NOT BE HELD LIABLE FOR ANY
 * 	* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * 	* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * 	* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * 	* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 * 
 * 	* Copyright (c) 2020 Tinychip Microelectronics Co.,Ltd, All Rights Reserved. 
 */
#ifndef __TC04XX_SPI_H__
#define __TC04XX_SPI_H__
#include "tc04xx.h"


 /* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

//SPI_Clock_Polarity
#define                      SPI_CPOL_LOW                                 0x00U
#define                      SPI_CPOL_HIGH                                0x01U 

//SPI_Clock_Phase 
#define                      SPI_CPHA_1Edge                               0x00U
#define                      SPI_CPHA_2Edge                               0x01U


//SPI Datasize
#define                      SPI_DATASIZE_8B                              0x07U
#define                      SPI_DATASIZE_16B                             0x0FU
#define                      SPI_DATASIZE_32B                             0x00U

//SPI Mode 
#define                      SPI_MODE_MASTER                              0x00U
#define                      SPI_MODE_SLAVE                               0x01U

//SPI Format 
#define                      SPI_FORMAT_MOTOROLA                          0x00U
#define                      SPI_FORMAT_TI                                0x01U
#define                      SPI_FORMAT_NM                                0x02U

//SPI Baudrate Prescale
#define                      SPI_BAUDRATE_PRESCALER_2                     0x02U
#define                      SPI_BAUDRATE_PRESCALER_4                     0x04U
#define                      SPI_BAUDRATE_PRESCALER_8                     0x08U
#define                      SPI_BAUDRATE_PRESCALER_16                    0x10U
#define                      SPI_BAUDRATE_PRESCALER_32                    0x20U
#define                      SPI_BAUDRATE_PRESCALER_64                    0x40U
#define                      SPI_BAUDRATE_PRESCALER_128                   0x80U
#define                      SPI_BAUDRATE_PRESCALER_254                   0xFEU


//SPI Interrupt Bit
#define                      SPI_IT_OVERRUN                               0x01U
#define                      SPI_IT_TIMEOUT                               0x02U
#define                      SPI_IT_RX                                    0x04U
#define                      SPI_IT_TX                                    0x08U
#define                      SPI_IT_ALL                                   0x0FU


//SPI Status Bit
#define                      SPI_TXFIFO_EMPTY                             0x01U
#define                      SPI_TXFIFO_NOT_FULL                          0x02U
#define                      SPI_RXFIFO_NOT_EMPTY                         0x04U
#define                      SPI_RXFIFO_FULL                              0x08U
#define                      SPI_BUSY_FLAG                                0x10U

typedef struct
{
    uint8_t cpol;
    uint8_t cpha;
    uint8_t datasize;
    uint8_t mode;
    uint8_t prescaler;
}SPI_INIT_T;



#define     SPI_SCR_CFG(cfg)    (SPI->CR0_F.SCR = (cfg))
#define     SPI_CPSR_CFG(cfg)   (SPI->CPSR_F.CPSDIVSR = (cfg))
#define     SPI_DATASIZE_CFG(size)   (SPI->CR0_F.DSS = (size&0xF))


#define     SPI_FRAME_CFG(cfg)   (SPI->CR0_F.FRF = (cfg))
#define     SPI_LONGFRAME_ENABLE()  (SPI->CR1_F.LFRM = ENABLE)
#define     SPI_LONGFRAME_DISABLE()  (SPI->CR1_F.LFRM = DISABLE)

#define     SPI_ENABLE()  (SPI->CR1_F.SSE = ENABLE)
#define     SPI_DISABLE()  (SPI->CR1_F.SSE = DISABLE)

#define     SPI_RXDMA_ENABLE()  (SPI->DMACR_F.RXDMAE = ENABLE)
#define     SPI_RXDMA_DISABLE()  (SPI->DMACR_F.RXDMAE = DISABLE)

#define     SPI_TXDMA_ENABLE()  (SPI->DMACR_F.TXDMAE = ENABLE)
#define     SPI_TXDMA_DISABLE()  (SPI->DMACR_F.TXDMAE = DISABLE)

#define     SPI_IT_ENABLE(u8mask)     (SPI->IMSCR = (u8mask))
#define     SPI_IT_DISABLE(u8mask)     (SPI->IMSCR = ~(u8mask))

#define     SPI_RAW_STATUS(u8mask)  ((SPI->RIS & (u8mask))?TRUE:FALSE)
#define     SPI_IT_STATUS(u8mask)  ((SPI->MIS & (u8mask))?TRUE:FALSE)
#define     SPI_IT_CLR(u8mask)     (SPI->ICR = (u8mask))

#define     SPI_TX_DATA(data)       (SPI->DR = (data&0xFFFFFFFF))
#define     SPI_RX_DATA()           (SPI->DR&0xFFFFFFFF)

#define     SPI_STATUS(status)      ((SPI->SR&(status))?TRUE:FALSE)

void SPI_SetFclkSource(uint8_t clk_src,uint8_t clk_div);
void SPI_Init(SPI_INIT_T *spi_init_t);
uint8_t SPI_SendRecvByte(uint8_t data);
#ifdef __cplusplus
}
#endif
#endif

