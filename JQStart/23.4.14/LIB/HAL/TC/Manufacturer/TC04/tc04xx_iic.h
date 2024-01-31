/*
 * @Description: 
 * @Author: wangshi@tinychip.com.cn
 * @Date: 2023-03-08 13:55:36
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
#ifndef  __TC04XX_IIC_H__
#define  __TC04XX_IIC_H__

#include "tc04xx.h"



/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

//IIC Mode
#define	                     IIC_MODE_MASTER                              0x00U
#define	                     IIC_MODE_SLAVE                               0x01U

//IIC Address Mode
#define	                     IIC_ADDR_MODE_7BIT                           0x00U
#define	                     IIC_ADDR_MODE_10BIT                          0x01U

//IIC Rate
#define                     IIC_RATE_20K                                  0x00U
#define                     IIC_RATE_100K                                 0x01U
#define                     IIC_RATE_400K                                 0x02U

//IIC Interrupt Bit
#define                     IIC_IT_TXFIFO_EMPTY                           0x01U
#define                     IIC_IT_TXFIFO_OVERFLOW                        0x04U
#define                     IIC_IT_RXFIFO_FULL                            0x08U
#define                     IIC_IT_TX_BYTE                                0x20U
#define                     IIC_IT_RX_BYTE                                0x40U
#define                     IIC_IT_NACK                                   0x80U
#define                     IIC_IT_ADDR_MATCH                             0x100U
#define                     IIC_IT_TRANS_DONE                             0x200U
#define                     IIC_IT_DETECT_STOP                            0x400U
#define                     IIC_IT_BUS_ERR                                0x800U
#define                     IIC_IT_ARB_LOST                               0x1000U
#define                     IIC_IT_SCL_STRETCH_TIMEOUT                    0x10000U
#define                     IIC_IT_DEDIATE_POINT                          0x20000U
#define                     IIC_IT_DETECT_START                           0x40000U
#define                     IIC_IT_DETECT_RESTART                         0x80000U


//IIC Status Bit
#define                     IIC_STATUS_BUSY                               0x01U
#define                     IIC_STATUS_ADDR_PHASE                         0x02U
#define                     IIC_STATUS_DATA_PHASE                         0x04U
#define                     IIC_STATUS_SLAVE_ADDRED                       0x100U
#define                     IIC_STATUS_SLAVE_ADDRED_IND                   0x200U
#define                     IIC_STATUS_SLAVE_RWN                          0x400U

typedef struct
{
    uint8_t mode;
    uint8_t rate;
    uint8_t addr_mode;
    uint8_t auto_mode;
    uint16_t addr;
}IIC_INIT_T;



#define     IIC_ENABLE()                    (IIC->MCR_F.I2C_EN = ENABLE)
#define     IIC_DISABLE()                   (IIC->MCR_F.I2C_EN = DISABLE)

#define     IIC_TXDMA_ENABLE()              (IIC->MCR_F.TX_DMA_EN = ENABLE)
#define     IIC_TXDMA_DISABLE()             (IIC->MCR_F.TX_DMA_EN = DISABLE)

#define     IIC_RXDMA_ENABLE()              (IIC->MCR_F.RX_DMA_EN = ENABLE)
#define     IIC_RXDMA_DISABLE()             (IIC->MCR_F.RX_DMA_EN = DISABLE)

#define     IIC_AUTOAMODE_ENABLE()          (IIC->MCR_F.CONTROL_MODE = ENABLE)
#define     IIC_AUTOAMODE_DISABLE()         (IIC->MCR_F.CONTROL_MODE = DISABLE)

#define     IIC_FLUSH_TX_FIFO()             (IIC->SCR_F.FLUSH_TX_FIFO = ENABLE)
#define     IIC_FLUSH_RX_FIFO()             (IIC->SCR_F.FLUSH_RX_FIFO = ENABLE)
#define     IIC_TX_FIFO_TH_CFG(cfg)         (IIC->MCR_F.TX_FIFO_THRESHOLD =(cfg) &0xF)
#define     IIC_RX_FIFO_TH_CFG(cfg)         (IIC->MCR_F.RX_FIFO_THRESHOLD =(cfg) &0xF)


#define     IIC_TPR_SCLL_CFG(cfg)           (IIC->TPR_F.SCLL = (cfg)&0xFF)
#define     IIC_TPR_SCLH_CFG(cfg)           (IIC->TPR_F.SCLH = (cfg)&0xFF)

#define     IIC_SCL_STRETCH_CFG(cfg)        (IIC->MCR_F.DISABLE_SCL_STRETCH = (cfg))
#define     IIC_SCL_STRETCH_TIMEOUT(time)   (IIC->TOR_F.SCL_STRETCH_TIMEOUT_TIME = (time) &0x3FF)
#define     IIC_WAIT_IDLE_TIMEOUT(time)     (IIC->TOR_F.WAIT_IDLE_TIMEOUT_TIME = (time) &0x3FF)

#define     IIC_IT_ENABLE(u32mask)          (IIC->IMR &= ~(u32mask))
#define     IIC_IT_DISABLE(u32mask)         (IIC->IMR |= (u32mask))

#define     IIC_IT_CLR(u32mask)             (IIC->ICR = (u32mask))
#define     IIC_IT_STATUS(u32mask)          (((IIC->ISR)&(u32mask))?TRUE:FALSE)
#define     IIC_IT_RAW_STATUS(u32mask)      (((IIC->IRSR)&(u32mask))?TRUE:FALSE)

#define     IIC_TX_DATA(data)               (IIC->TRDR_F.DATA = (data)&0xFF)
#define     IIC_RX_DATA()                   (IIC->TRDR_F.DATA&0xFF)

#define     IIC_STATUS(u32mask)             (((IIC->STR&(u32mask))?TRUE:FALSE))
#define     IIC_TRANS_DATA_NUM()            (IIC->STR_F.TXRX_DATA_NUM & 0x1FF)





void IIC_SetFclkSource(uint8_t clk_src, uint8_t clk_div);
void IIC_Init(IIC_INIT_T * iic_init_t);
void IIC_Master_Send(uint16_t slave_addr,uint8_t addr_mode,uint8_t *data,uint16_t size);
void IIC_Master_Recv(uint16_t slave_addr,uint8_t addr_mode,uint8_t *data,uint16_t size);
void IIC_Slave_Send(uint8_t *data,uint16_t size);
void IIC_Slave_Recv(uint8_t *data,uint16_t size);
void IIC_DmaTxSetup(uint16_t slave_addr,uint8_t addr_mode,uint16_t size);
void IIC_DmaRxSetup(uint16_t slave_addr,uint8_t addr_mode,uint16_t size);
#ifdef __cplusplus
}
#endif
#endif
