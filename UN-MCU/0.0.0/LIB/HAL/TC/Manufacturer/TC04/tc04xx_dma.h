/*
 * @Description: 
 * @Author: wangshi@tinychip.com.cn
 * @Date: 2023-02-23 14:12:50
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
#ifndef __TC04XX_DMA_H__
#define __TC04XX_DMA_H__
#include "tc04xx.h"


 /* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

//DMA CHANNEL
#define                      DMA_CHANNEL_0                                0x00U
#define                      DMA_CHANNEL_1                                0x01U
#define                      DMA_CHANNEL_2                                0x02U

//DMA Request
// #define                      DMA_REQ_MTM                                  (-1)
#define                      DMA_REQ_1                                    (0x00U)
#define                      DMA_REQ_2                                    (0x01U)
#define                      DMA_REQ_3                                    (0x02U)
#define                      DMA_REQ_IIC_TX                               (0x03U)
#define                      DMA_REQ_IIC_RX                               (0x04U)
#define                      DMA_REQ_UART0_TX                             (0x05U)
#define                      DMA_REQ_UART0_RX                             (0x06U)
#define                      DMA_REQ_SPI_TX                               (0x07U)
#define                      DMA_REQ_SPI_RX                               (0x08U)
#define                      DMA_REQ_TOUCH                                (0x09U)
#define                      DMA_REQ_SARADC                               (0x0AU)

//DMA Align Byte
#define                      DMA_ALIGN_DEFAULT                            (0x00U)
#define                      DMA_ALIGN_1_BYTE                             (0x01U)

//DMA Burst Size
#define                      DMA_BURST_4_BYTES                            (0x00U)
#define                      DMA_BURST_8_BYTES                            (0x01U)
#define                      DMA_BURST_16_BYTES                           (0x02U)

//DMA Direction 
#define                      DMA_MEM_TO_MEM                               (0x00U)
#define                      DMA_PERIPH_TO_MEM                            (0x01U)
#define                      DMA_MEM_TO_PERIPH                            (0x02U)

//DMA Peripheral Width
#define                      DMA_PERIPH_WIDTH_NONE                        (0x00U)
#define                      DMA_PERIPH_WIDTH_BYTE                        (0x01U)
#define                      DMA_PERIPH_WIDTH_HALF_WORD                   (0x02U)
#define                      DMA_PERIPH_WIDTH_WORD                        (0x03U)

//DMA Interupt Bit
#define                      DMA_IT_BUSERR                                (0x01U)
#define                      DMA_IT_END                                   (0x04U)
#define                      DMA_IT_STOP                                  (0x08U)
#define                      DMA_IT_REQ_STOP                              (0x10U)
#define                      DMA_IT_ALL                                   (0x1DU)


typedef struct
{
    uint8_t channel;
    uint8_t request;
    uint8_t align;
    uint8_t burst;
    uint8_t dma_dir;
    uint8_t periph_width;
    uint32_t src_addr;
    uint32_t tgt_addr;
    uint16_t len;
}DMA_INIT_T;



#define DMA_ENABLE(channel)                         (DMA->DCSR_F[(channel)].DCSR_ENABLE = ENABLE)
#define DMA_DISABLE(channel)                        (DMA->DCSR_F[(channel)].DCSR_ENABLE = DISABLE)

#define DMA_START(channel)                          (DMA->DCSR_F[(channel)].DCSR_RUN = TRUE)
#define DMA_STOP(channel)                           (DMA->DCSR_F[(channel)].DCSR_RUN = FALSE)

#define DMA_IT_ENABLE(channel)                      (DMA->DESC_F[(channel)].DCMD_F.DESC_ENDIRQEN = ENABLE)
#define DMA_IT_DISABLE(channel)                     (DMA->DESC_F[(channel)].DCMD_F.DESC_ENDIRQEN = DISABLE)

#define DMA_IT_CHANNEL(channel)                     ((DMA->DINT_F.CHLINTR & (1<<(channel)))?TRUE:FALSE)
#define DMA_IT_STATUS(channel,u8mask)               ((DMA->DCSR[(channel)] & (u8mask))?TRUE:FALSE)
#define DMA_IT_CLR(channel,u8mask)                  (DMA->DCSR[(channel)] = (u8mask))


#define DMA_SRCADDR_INC_CFG(channel,cfg)            (DMA->DESC_F[(channel)].DCMD_F.DESC_INCSRCADDR = (cfg))
#define DMA_TGTADDR_INC_CFG(channel,cfg)            (DMA->DESC_F[(channel)].DCMD_F.DESC_INCTRGADDR = (cfg))
#define DMA_SRC_FLOWCTRL_CFG(channel,cfg)           (DMA->DESC_F[(channel)].DCMD_F.DESC_FLOWSRC = (cfg))
#define DMA_TGT_FLOWCTRL_CFG(channel,cfg)           (DMA->DESC_F[(channel)].DCMD_F.DESC_FLOWTRG = (cfg))

#define DMA_SRCADDR_CFG(channel,src_addr)           (DMA->DESC_F[(channel)].DSADR = (src_addr))
#define DMA_TGTADDR_CFG(channel,tgt_addr)           (DMA->DESC_F[(channel)].DTADR = (tgt_addr))
#define DMA_TRANS_LEN_CFG(channel,len)              (DMA->DESC_F[(channel)].DCMD_F.DESC_LEN = (len))
#define DMA_WIDTH_CFG(channel,width)                (DMA->DESC_F[(channel)].DCMD_F.DESC_WIDTH = (width))
#define DMA_BURST_CFG(channel,burst)                (DMA->DESC_F[(channel)].DCMD_F.DESC_BURST = (burst))



void DMA_DeInit(uint8_t channel,uint8_t request);
void DMA_Init(DMA_INIT_T *dma_init_t);



#ifdef __cplusplus
}
#endif
#endif
