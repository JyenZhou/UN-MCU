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
#include "tc04xx_wdg.h"


/**
 * @description: 
 * @param {uint8_t} clk_src
 * @param {uint8_t} PrescalerValue
 * @return {*}
 */
void WDG_SetFclkSource(uint8_t clk_src,uint8_t PrescalerValue)
{
    SYSCFG->WDG_CLKRST_CTRL_F.SW_PCLK_EN_WDG = 1;
    // SYSCFG->WDG_CLKRST_CTRL_F.FCEN = 1;
    SYSCFG->WDG_CLKRST_CTRL_F.SW_FCLK_SEL_WDG = clk_src;
    SYSCFG->WDG_CLKRST_CTRL_F.SW_FCLK_DIV_WDG = PrescalerValue;
}

/**
 * @description: 
 * @param {uint16_t} loadVal
 * @param {uint8_t} mode
 * @return {*}
 */
void WDG_Init(uint16_t loadVal,uint8_t mode)
{
    WDG->CTRL_F.REO = mode;
    WDG->CNT_MAX = loadVal;
}
