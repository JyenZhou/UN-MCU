/*
 * @Description: 
 * @Author: wangshi@tinychip.com.cn
 * @Date: 2023-02-03 09:56:12
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
#include "tc04xx_lvd.h"

#define     LVD_VDD_VREF            0x06

void LVD_SetFclkSource(uint8_t clk_src,uint16_t clk_div)
{
    SYSCFG->LVD_CLKRST_CTRL_F.SW_RST_LVD = 1;
    SYSCFG->LVD_CLKRST_CTRL_F.SW_PCLK_EN_LVD = 1;
    SYSCFG->LVD_CLKRST_CTRL_F.SW_FCLK_EN_LVD = 1;
    SYSCFG->LVD_CLKRST_CTRL_F.SW_FCLK_SEL_LVD = clk_src;
    SYSCFG->LVD_CLKRST_CTRL_F.SW_FCLK_DIV_LVD = clk_div;
    SYSCFG->LVD_CLKRST_CTRL_F.SW_RST_LVD = 0;
}


void LVD_Config(uint8_t volt_level,uint8_t high_power_en,uint8_t rst_en)
{
    ANAREG->LVD_TOP_F.CTRL_LVD5_1_HPM = high_power_en;
    ANAREG->LVD_TOP_F.CTRL_LVD5_4_2_VREF_SEL = LVD_VDD_VREF;
    ANAREG->LVD_TOP_F.CTRL_LVD5_8_5_TRIP_VOL_SEL = volt_level;
    ANAREG->LVD_CFG_F.LVD_RST_EN = rst_en;
}

// void LVD15_Config(uint8_t volt_level,uint8_t high_power_en,uint8_t rst_en)
// {
//     ANAREG->LVD_CFG_F.LVD15_RST_EN = rst_en;
//     ANAREG->LDO15_TOP_F.CTRL_LDO15_15_4 &= ~(0x7);
//     ANAREG->LDO15_TOP_F.CTRL_LDO15_15_4 |= volt_level;
//     ANAREG->LDO15_TOP_F.CTRL_LDO15_2_1 = high_power_en;
// }
