/*
 * @Description: 
 * @Author: wangshi@tinychip.com.cn
 * @Date: 2023-01-29 13:26:29
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
#include "tc04xx_rtc.h"

/**
 * @description: 
 * @param {uint8_t} clk_src
 * @param {uint16_t} clk_div
 * @return {*}
 */
void RTC_SetFclkSource(uint8_t clk_src,uint8_t clk_div)
{
    SYSCFG->RTC_CLKRST_CTRL_F.SW_RST_RTC = 1;
    SYSCFG->RTC_CLKRST_CTRL_F.SW_FCLK_EN_RTC = 1;
    SYSCFG->RTC_CLKRST_CTRL_F.SW_FCLK_SEL_RTC = clk_src;
    SYSCFG->RTC_CLKRST_CTRL_F.SW_FCLK_DIV_RTC = clk_div;
    SYSCFG->RTC_CLKRST_CTRL_F.SW_RST_RTC = 0;
    
}
/**
 * @description: 
 * caution:1.the operation will clear the rtc cnt and load new config
 *         2.between operations must wait at least 3 rtc cycle 
 * @return {*}
 */
void RTC_Flush_Setting(void)
{
    //flush the rtc setting
    RTC->RTC_CTRL_0_F.RTC_LOAD_NOW = 1;
    RTC->RTC_CTRL_0_F.RTC_LOAD = 1;
}
/**
 * @description: 
 * @param {uint8_t} Wave_Cnt
 * @param {uint8_t} Output_En
 * @return {*}
 */
void RTC_WaveConfig(uint8_t Wave_Cnt,uint8_t Output_En)
{
    RTC->RTC_CTRL_0_F.SW_WAVE_DIV = Wave_Cnt;       
    RTC->RTC_CTRL_0_F.SW_WAVE_TEST_EN = Output_En;
}
/**
 * @description: 
 * @param {uint8_t} Trigger_ID
 * @param {uint16_t} Trigger_Cnt
 * @return {*}
 */
void RTC_Trigger_Config(uint8_t Trigger_ID,uint16_t Trigger_Cnt)
{
    switch(Trigger_ID)
    {
        case RTC_TRIG_0:
            RTC->RTC_CTRL_1_F.RTC_TRIG_NUM_0 = Trigger_Cnt;
            break;
        case RTC_TRIG_1:
            RTC->RTC_CTRL_1_F.RTC_TRIG_NUM_1 = Trigger_Cnt;
            break;
        case RTC_TRIG_2:
            RTC->RTC_CTRL_2_F.RTC_TRIG_NUM_2 = Trigger_Cnt;
            break;
        default:
            //error
            break;
    }
}
/**
 * @description: 
 * @param {uint8_t} Trigger_ID
 * @return {*}
 */
void RTC_Trigger_Enable(uint8_t Trigger_ID)
{
    switch(Trigger_ID)
    {
        case RTC_TRIG_0:
            RTC->RTC_CTRL_0_F.RTC_TRIG_EN_0 = 1;
            break;
        case RTC_TRIG_1:
            RTC->RTC_CTRL_0_F.RTC_TRIG_EN_1 = 1;
            break;
        case RTC_TRIG_2:
            RTC->RTC_CTRL_0_F.RTC_TRIG_EN_2 = 1;
            break;
        default:
            //error
            break;
    }
}

/**
 * @description: 
 * @param {uint8_t} Trigger_ID
 * @return {*}
 */
void RTC_Trigger_Disable(uint8_t Trigger_ID)
{
    switch(Trigger_ID)
    {
        case RTC_TRIG_0:
            RTC->RTC_CTRL_0_F.RTC_TRIG_EN_0 = 0;
            break;
        case RTC_TRIG_1:
            RTC->RTC_CTRL_0_F.RTC_TRIG_EN_1 = 0;
            break;
        case RTC_TRIG_2:
            RTC->RTC_CTRL_0_F.RTC_TRIG_EN_2 = 0;
            break;
        default:
            //error
            break;
    }
}
