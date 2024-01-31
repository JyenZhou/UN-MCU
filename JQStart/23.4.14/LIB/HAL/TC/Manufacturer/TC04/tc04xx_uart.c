/*
 * @Description:
 * @Author: wangshi@tinychip.com.cn
 * @Date: 2023-02-06 17:09:30
 * @Disclaimer:
 *  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 *  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 *  * TIME. AS A RESULT, TINY CHIP SHALL NOT BE HELD LIABLE FOR ANY
 *  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 *  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 *  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 *  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 *  * Copyright (c) 2020 Tinychip Microelectronics Co.,Ltd, All Rights Reserved.
 */
#include "tc04xx_uart.h"

#define     UART0_DEFAULT_PRESCALE_CFG  0x00
#define     UART0_DEFAULT_PRESCALE      16

/**
 * @description:
 * @param {uint8_t} clk_src
 * @param {uint16_t} clk_div
 * @return {*}
 */
void UART0_SetFclkSource(uint8_t clk_src, uint8_t clk_div)
{
    SYSCFG->UART0_CLKRST_CTRL_F.SW_RST_UART0 = 1;
    SYSCFG->UART0_CLKRST_CTRL_F.SW_FCLK_EN_UART0 = 1;
    SYSCFG->UART0_CLKRST_CTRL_F.SW_FCLK_SEL_UART0 = clk_src;
    SYSCFG->UART0_CLKRST_CTRL_F.SW_FCLK_DIV_UART0 = clk_div;
    SYSCFG->UART0_CLKRST_CTRL_F.SW_RST_UART0 = 0;
}
/**
 * @description:
 * @return {*}
 */
static uint32_t UART0_GetFclk(void)
{
    uint32_t clk_base = 0;
    uint8_t clk_div = SYSCFG->UART0_CLKRST_CTRL_F.SW_FCLK_DIV_UART0 + 0x01U;

    switch (SYSCFG->UART0_CLKRST_CTRL_F.SW_FCLK_SEL_UART0)
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
    return (uint32_t)(clk_base/clk_div);
}
/**
 * @description:
 * @param {uint32_t} baudrate
 * @return {*}
 */
static void Uart0_BaudrateSet(uint32_t baudrate)
{
    uint32_t fclk = UART0_GetFclk();
    uint64_t divisor = (((uint64_t)fclk * 100) / baudrate) / UART0_DEFAULT_PRESCALE;
    uint8_t dll, dlh, frac;

    dll = (uint8_t)((divisor / 100) & 0xFFU);
    dlh = (uint8_t)(((divisor / 100) & 0xFF00U) >> 8);
    frac = (uint8_t)(((divisor - ((divisor / 100) * 100)) * 16) / 100);

    UART0->MFCR_F.PF = UART0_DEFAULT_PRESCALE_CFG;
    UART0->DLR_F.DLL = dll;
    UART0->DLR_F.DLH = dlh;
    UART0->EFCR_F.FRACD = frac;
}
/**
 * @description:
 * @param {UART_INIT_T *} p_init_t
 * @return {*}
 */
void UART0_Init(UART_INIT_T *p_init_t)
{
    if(p_init_t != NULL)
    {
        UART0->LCR_F.CHARL = p_init_t->datalen;
        UART0->LCR_F.SBN = p_init_t->stopbits;
        UART0->LCR_F.PE = (p_init_t->parity == UART_PARITY_NONE) ? 0x00U:0x01U;
        UART0->LCR_F.EPS = p_init_t->parity;
        //default enbale tx and rx
        UART0->MFCR_F.RX_EN = 1;
        UART0->MFCR_F.TX_EN = 1;
        Uart0_BaudrateSet(p_init_t->buadtare);
    }
}

