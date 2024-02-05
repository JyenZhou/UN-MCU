/*
 * @Description: 
 * @Author: wangshi@tinychip.com.cn
 * @Date: 2023-01-28 17:21:47
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
#ifndef __TC04XX_UART_H__
#define __TC04XX_UART_H__
#include "tc04xx.h"



 /* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

//UART Parity
#define                      UART_PARITY_NONE                             0x00U
#define                      UART_PARITY_ODD                              0x01U
#define                      UART_PARITY_EVEN                             0x02U


//UART Stopbit
#define                      UART_STOPBIT_1                               0x00U
#define                      UART_STOPBIT_1_5                             0x01U

//UART Datalen
#define                      UART_DATALEN_5                               0x00U
#define                      UART_DATALEN_6                               0x01U
#define                      UART_DATALEN_7                               0x02U
#define                      UART_DATALEN_8                               0x03U

//UART RX FIFOTH
#define                      UART_FIFO_RXTH_1BYTE                         0x00U
#define                      UART_FIFO_RXTH_1_4_DEPTH                     0x01U
#define                      UART_FIFO_RXTH_1_2_DEPTH                     0x02U
#define                      UART_FIFO_RXTH_FULL_DEPTH                    0x03U
//UART TX FIFOTH
#define                      UART_FIFO_TXTH_EMPTY                         0x00U
#define                      UART_FIFO_TXTH_2BYTES                        0x01U
#define                      UART_FIFO_TXTH_1_4_DEPTH                     0x02U
#define                      UART_FIFO_TXTH_1_2_DEPTH                     0x03U

//UART Breakbit Set
#define                      UART_BREAK_10_BIT                            0x00U
#define                      UART_BREAK_11_BIT                            0x01U

//UART  Interrupt bit
#define                     UART_LINE_STATUS                              0x01U
#define                     UART_RX_DATA_AVAILABLE                        0x02U
#define                     UART_RX_DATA_TIMEOUT                          0x04U
#define                     UART_TX_DATA_REQUEST                          0x08U
#define                     UART_MODEM_STATUS                             0x10U
#define                     UART_CONFIG_LCR_DURING_BUSY                   0x20U
#define                     UART_AUTO_BAUDRATE_TIMEOUT                    0x40U
#define                     UART_RECV_CHAR_MATCH                          0x80U
#define                     UART_AUTO_BAUDRATE_DONE                       0x100U


typedef struct
{
    uint32_t buadtare;
    uint8_t datalen;
    uint8_t stopbits;
    uint8_t parity;
}UART_INIT_T;

#define     UART0_RX_ENABLE()       (UART0->MFCR_F.RX_EN = ENABLE)
#define     UART0_RX_DISABLE()      (UART0->MFCR_F.RX_EN = DISABLE)
#define     UART0_TX_ENABLE()       (UART0->MFCR_F.TX_EN = ENABLE)
#define     UART0_TX_DISABLE()      (UART0->MFCR_F.TX_EN = DISABLE)
#define     UART0_ENABLE()          (UART0->MFCR_F.UE = ENABLE)
#define     UART0_DISABLE()         (UART0->MFCR_F.UE = DISABLE)
#define     UART0_DMA_ENABLE()      (UART0->MFCR_F.DE = ENABLE)
#define     UART0_DMA_DISABLE()     (UART0->MFCR_F.DE = DISABLE)
#define     UART0_INT_ENABLE(mask)  (UART0->IMR &= ~(mask))
#define     UART0_INT_DISABLE(mask) (UART0->IMR |= (mask))
#define     UART0_INT_CLR(mask)     (UART0->ICR = (mask))
#define     UART0_INT_STATUS(mask)  ((UART0->ISR &(mask))?1:0)
#define     UART0_RAW_STATUS(mask)  ((UART0->IRSR &(mask))?1:0)


#define     UART0_FIFO_ENABLE()      (UART0->FCR_F.FIFOEN = ENABLE)
#define     UART0_FIFO_DISABLE()     (UART0->FCR_F.FIFOEN = DISABLE)

#define     UART0_RX_FIFO_TH_SET(th) (UART0->FCR_F.RFATL = (th))
#define     UART0_TX_FIFO_TH_SET(th) (UART0->FCR_F.TFETL = (th))
#define     UART0_RX_FIFO_CLR()      (UART0->FCR_F.RSTRF = 0x01)
#define     UART0_TX_FIFO_CLR()      (UART0->FCR_F.RSTTF = 0x01)

#define     UART0_BREAK_CFG(cfg)     (UART0->LCR_F.BLS = (cfg))
#define     UART0_FORCE_BREAK()      (UART0->LCR_F.SETB = 0x01)   

#define     UART0_TX_DATA(data)      (UART0->RTBR_F.RBTHR = (data&0xFF))
#define     UART0_RX_DATA()          (UART0->RTBR_F.RBTHR&0xFF)

#define     UART0_AUTOBAUDRATE_ENABLE() do{UART0->ADCR_F.ABE = ENABLE;UART0->ADCR_F.ABPS = 1;}while(0)
#define     UART0_AUTOBAUDRATE_DISABLE() do{UART0->ADCR_F.ABE = DISABLE;UART0->ADCR_F.ABPS = 0;}while(0)

#define     UART0_CHAR_MATCH_ENABLE() (UART0->RCMR_F.RCME = ENABLE)
#define     UART0_CHAR_MATCH_DISABLE() (UART0->RCMR_F.RCME = DISABLE)

#define     UART0_CHAR_MATCH_STATUS() (UART0->RCMR_F.RCMS)
#define     UART0_CHAR_MATCH_CLR() (UART0->RCMR_F.RCMSC = 0x01)

#define     UART0_CHAR_MATCH_SET(ch) (UART0->RCMR_F.RCM = (ch&0xFF))

void UART0_SetFclkSource(uint8_t clk_src, uint8_t clk_div);
void UART0_Init(UART_INIT_T * p_init_t);

#ifdef __cplusplus
}
#endif
#endif
