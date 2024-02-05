/*
 * @Description: 
 * @Author: wangshi@tinychip.com.cn
 * @Date: 2023-02-28 10:04:36
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
#include "tc04xx_spi.h"

#define     DEFAULT_SCR_CFG     0x0

/**
 * @description: 
 * @param {uint8_t} clk_src
 * @param {uint16_t} clk_div
 * @return {*}
 */
void SPI_SetFclkSource(uint8_t clk_src,uint8_t clk_div)
{
    SYSCFG->SPI_CLKRST_CTRL_F.SW_RST_SPI =1;
    SYSCFG->SPI_CLKRST_CTRL_F.SW_PCLK_EN_SPI = 1;
    SYSCFG->SPI_CLKRST_CTRL_F.SW_FCLK_EN_SPI = 1;
    SYSCFG->SPI_CLKRST_CTRL_F.SW_FCLK_SEL_SPI = clk_src;
    SYSCFG->SPI_CLKRST_CTRL_F.SW_FCLK_DIV_SPI = clk_div;
    SYSCFG->SPI_CLKRST_CTRL_F.SW_RST_SPI = 0;
}
/**
 * @description: 
 * @param {SPI_INIT_T} *spi_init_t
 * @return {*}
 */
void SPI_Init(SPI_INIT_T *spi_init_t)
{
    if(spi_init_t != NULL)
    {
        SPI->CR0_F.DSS = spi_init_t->datasize;
        SPI->CR0_F.SPO = spi_init_t->cpol;
        SPI->CR0_F.SPH = spi_init_t->cpha;
        SPI->CR0_F.SCR = DEFAULT_SCR_CFG;
        SPI->CR1_F.MS = spi_init_t->mode;
        SPI->CPSR_F.CPSDIVSR = spi_init_t->prescaler;
    }
}
/**
 * @description: 
 * @param {uint8_t} data
 * @return {*}
 */
uint8_t SPI_SendRecvByte(uint8_t data)
{
    while(SPI_STATUS(SPI_TXFIFO_EMPTY) == 0)
    {

    }

    SPI_TX_DATA(data);
    
    while(SPI_STATUS(SPI_RXFIFO_NOT_EMPTY) == 0)
    {
        
    }
    return SPI_RX_DATA();
}
