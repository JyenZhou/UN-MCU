/*
 * @FilePath     : tc04xx_debug.c
 * @Description  : 
 * @Author       : Liu_zhen
 * @Date         : 2023-09-11
 * @LastEditTime: 2023-09-28 11:22:25
 * Copyright 2023, All Rights Reserved. 
 */

#include <stdio.h>
#include "tc04xx_uart.h"
#include "tc04xx_gpio.h"
#include "tc04xx_debug.h"


/**
 * @description:
 * @param {uint8_t} clk_src
 * @param {uint16_t} clk_div
 * @return {*}
 */
void UART1_SetFclkSource(uint8_t clk_src, uint16_t clk_div)
{
    SYSCFG->UART1_CLKRST_CTRL_F.SW_RST_UART1 = 1;
    SYSCFG->UART1_CLKRST_CTRL_F.SW_FCLK_EN_UART1 = 1;
    SYSCFG->UART1_CLKRST_CTRL_F.SW_FCLK_SEL_UART1 = clk_src;
    SYSCFG->UART1_CLKRST_CTRL_F.SW_FCLK_DIV_UART1 = (uint8_t)clk_div;
    SYSCFG->UART1_CLKRST_CTRL_F.SW_RST_UART1 = 0;
}

/**
 * @description:
 * @return {*}
 */
static uint32_t UART1_GetClk(void)
{
    uint32_t clk_base = 0;

    switch (CLKRST->HCLK_SEL_F.HCLK_SEL)
    {
    case CLOCK_SEL_16M:
        clk_base = 16000000;
        break;
    case CLOCK_SEL_32M:
        clk_base = 32000000;
        break;
    case CLOCK_SEL_32K:
        clk_base = 32000;
        break;
    default:
        //error
        break;
    }
    return (clk_base / (SYSCFG->UART1_CLKRST_CTRL_F.SW_FCLK_DIV_UART1 + (uint32_t)1));
}

/**
 * @description:
 * @param {uint32_t} baud
 * @return {*}
 */
void Debug_Init(uint32_t baud)
{
    uint32_t precale = 0;
    uint32_t clk = UART1_GetClk();

    precale = clk / baud - 1;
    UART1->PRESCALE_F.PRESCALE = (uint16_t)(precale & 0xFFFU);
}

/**
 * @description:
 * @return {*}
 */
int __attribute__((weak)) fputc(int c, FILE *stream)
{
    (void)stream;

    UART1->TX_DATA = (uint8_t)c;
    while (UART1->STATUS_F.TX_BUSY == 1)
    {

    }

    return (c);
}

