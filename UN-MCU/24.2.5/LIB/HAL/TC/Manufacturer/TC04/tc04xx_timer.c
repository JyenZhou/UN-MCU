/*
 * @Description:
 * @Author: wangshi@tinychip.com.cn
 * @Date: 2023-02-07 11:03:01
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
#include "tc04xx_timer.h"

#define     TIMER_IO_OUT_AF_CFG         0x02
#define     TIMER_IO_IN_AF_CFG          0x03
#define     TIMER_IO_PB28_AF_CFG        0x04
/**
 * @description: 
 * @param {uint8_t} timer_id
 * @param {uint8_t} clk_src
 * @param {uint16_t} clk_div
 * @return {*}
 */
void TIMER_SetFclkSource(uint8_t timer_id, uint8_t clk_src, uint8_t clk_div)
{
    switch (timer_id)
    {
        case TIMER_0:
            SYSCFG->GPT0_CLKRST_CTRL_F.SW_PCLK_EN_GPT0 = 1;
            SYSCFG->GPT0_CLKRST_CTRL_F.SW_FCLK_EN_GPT0 = 1;
            SYSCFG->GPT0_CLKRST_CTRL_F.SW_FCLK_SEL_GPT0 = clk_src;
            SYSCFG->GPT0_CLKRST_CTRL_F.SW_FCLK_DIV_GPT0 = clk_div;
            break;
        case TIMER_1:
            SYSCFG->GPT1_CLKRST_CTRL_F.SW_PCLK_EN_GPT1 = 1;
            SYSCFG->GPT1_CLKRST_CTRL_F.SW_FCLK_EN_GPT1 = 1;
            SYSCFG->GPT1_CLKRST_CTRL_F.SW_FCLK_SEL_GPT1 = clk_src;
            SYSCFG->GPT1_CLKRST_CTRL_F.SW_FCLK_DIV_GPT1 = clk_div;
            break;
        case TIMER_2:
            SYSCFG->GPT2_CLKRST_CTRL_F.SW_PCLK_EN_GPT2 = 1;
            SYSCFG->GPT2_CLKRST_CTRL_F.SW_FCLK_EN_GPT2 = 1;
            SYSCFG->GPT2_CLKRST_CTRL_F.SW_FCLK_SEL_GPT2 = clk_src;
            SYSCFG->GPT2_CLKRST_CTRL_F.SW_FCLK_DIV_GPT2 = clk_div;
            break;
        case TIMER_3:
            SYSCFG->GPT3_CLKRST_CTRL_F.SW_PCLK_EN_GPT3 = 1;
            SYSCFG->GPT3_CLKRST_CTRL_F.SW_FCLK_EN_GPT3 = 1;
            SYSCFG->GPT3_CLKRST_CTRL_F.SW_FCLK_SEL_GPT3 = clk_src;
            SYSCFG->GPT3_CLKRST_CTRL_F.SW_FCLK_DIV_GPT3 = clk_div;
            break;
        case TIMER_4:
            SYSCFG->GPT4_CLKRST_CTRL_F.SW_PCLK_EN_GPT4 = 1;
            SYSCFG->GPT4_CLKRST_CTRL_F.SW_FCLK_EN_GPT4 = 1;
            SYSCFG->GPT4_CLKRST_CTRL_F.SW_FCLK_SEL_GPT4 = clk_src;
            SYSCFG->GPT4_CLKRST_CTRL_F.SW_FCLK_DIV_GPT4 = clk_div;
            break;
        default:
            //error
            break;
    }
}
/**
 * @description: 
 * @param {uint8_t} timer_id
 * @param {uint8_t} group
 * @param {uint8_t} channel
 * @return {*}
 */
void TIMER_IO_Out_Config(uint8_t timer_id, uint8_t group, uint8_t channel)
{
    if (timer_id == TIMER_0)
    {
        if (group == TIMER_GROUP_SEL_0)
        {
            if (channel == TIMER_CHANNEL_A)
            {
                PINMUX->PB9_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
            else if (channel == TIMER_CHANNEL_B)
            {
                PINMUX->PB10_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
            else //both
            {
                PINMUX->PB9_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
                PINMUX->PB10_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
        }
        else if (group == TIMER_GROUP_SEL_1)
        {
            if (channel == TIMER_CHANNEL_A)
            {
                PINMUX->PB14_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
            else if (channel == TIMER_CHANNEL_B)
            {
                PINMUX->PB15_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
            else //both
            {
                PINMUX->PB14_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
                PINMUX->PB15_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
        }
        else //both
        {
            if (channel == TIMER_CHANNEL_A)
            {
                PINMUX->PB9_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
                PINMUX->PB14_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
            else if (channel == TIMER_CHANNEL_B)
            {
                PINMUX->PB10_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
                PINMUX->PB15_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
            else //both
            {
                PINMUX->PB9_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
                PINMUX->PB10_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
                PINMUX->PB14_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
                PINMUX->PB15_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
        }
    }
    if (timer_id == TIMER_1)
    {
        if (group == TIMER_GROUP_SEL_0)
        {
            if (channel == TIMER_CHANNEL_A)
            {
                PINMUX->PB11_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
            else if (channel == TIMER_CHANNEL_B)
            {
                PINMUX->PB12_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
            else //both
            {
                PINMUX->PB11_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
                PINMUX->PB12_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
        }
        else if (group == TIMER_GROUP_SEL_1)
        {
            if (channel == TIMER_CHANNEL_A)
            {
                PINMUX->PB16_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
            else if (channel == TIMER_CHANNEL_B)
            {
                PINMUX->PB17_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
            else //both
            {
                PINMUX->PB16_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
                PINMUX->PB17_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
        }
        else //both
        {
            if (channel == TIMER_CHANNEL_A)
            {
                PINMUX->PB11_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
                PINMUX->PB16_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
            else if (channel == TIMER_CHANNEL_B)
            {
                PINMUX->PB12_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
                PINMUX->PB17_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
            else //both
            {
                PINMUX->PB11_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
                PINMUX->PB12_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
                PINMUX->PB16_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
                PINMUX->PB17_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
        }
    }
    if (timer_id == TIMER_2)
    {
        if (group == TIMER_GROUP_SEL_0)
        {
            if (channel == TIMER_CHANNEL_A)
            {
                PINMUX->PB13_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
            else if (channel == TIMER_CHANNEL_B)
            {
                PINMUX->PB30_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
            else //both
            {
                PINMUX->PB13_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
                PINMUX->PB30_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
        }
        else if (group == TIMER_GROUP_SEL_1)
        {
            if (channel == TIMER_CHANNEL_A)
            {
                PINMUX->PB24_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
            else if (channel == TIMER_CHANNEL_B)
            {
                PINMUX->PB30_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
            else //both
            {
                PINMUX->PB24_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
                PINMUX->PB30_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
        }
        else //both
        {
            if (channel == TIMER_CHANNEL_A)
            {
                PINMUX->PB13_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
                PINMUX->PB24_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
            else if (channel == TIMER_CHANNEL_B)
            {
                PINMUX->PB30_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
            else //both
            {
                PINMUX->PB13_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
                PINMUX->PB24_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
                PINMUX->PB30_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
        }
    }
    if (timer_id == TIMER_3)
    {
        if (group == TIMER_GROUP_SEL_0)
        {
            if (channel == TIMER_CHANNEL_A)
            {
                PINMUX->PB22_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
            else if (channel == TIMER_CHANNEL_B)
            {
                PINMUX->PB23_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
            else //both
            {
                PINMUX->PB22_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
                PINMUX->PB23_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
        }
        else if (group == TIMER_GROUP_SEL_1)
        {
            if (channel == TIMER_CHANNEL_A)
            {
                PINMUX->PB25_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
            else if (channel == TIMER_CHANNEL_B)
            {
                PINMUX->PB26_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
            else //both
            {
                PINMUX->PB25_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
                PINMUX->PB26_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
        }
        else //both
        {
            if (channel == TIMER_CHANNEL_A)
            {
                PINMUX->PB22_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
                PINMUX->PB25_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
            else if (channel == TIMER_CHANNEL_B)
            {
                PINMUX->PB23_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
                PINMUX->PB26_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
            else //both
            {
                PINMUX->PB22_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
                PINMUX->PB23_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
                PINMUX->PB25_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
                PINMUX->PB26_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
        }
    }
    if (timer_id == TIMER_4)
    {
        if (group == TIMER_GROUP_SEL_0)
        {
            if (channel == TIMER_CHANNEL_A)
            {
                PINMUX->PB27_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
            else if (channel == TIMER_CHANNEL_B)
            {
                PINMUX->PB29_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
            else //both
            {
                PINMUX->PB27_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
                PINMUX->PB29_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
        }
        else if (group == TIMER_GROUP_SEL_1)
        {
            if (channel == TIMER_CHANNEL_A)
            {
                PINMUX->PB28_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
            else if (channel == TIMER_CHANNEL_B)
            {
                PINMUX->PB29_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
            else //both
            {
                PINMUX->PB28_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
                PINMUX->PB29_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
        }
        else //both
        {
            if (channel == TIMER_CHANNEL_A)
            {
                PINMUX->PB27_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
                PINMUX->PB28_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
            else if (channel == TIMER_CHANNEL_B)
            {
                PINMUX->PB29_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
            else //both
            {
                PINMUX->PB27_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
                PINMUX->PB28_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
                PINMUX->PB29_CFG_F.SW_SRC_SEL = TIMER_IO_OUT_AF_CFG;
            }
        }
    }

}
/**
 * @description: 
 * @param {uint8_t} timer_id
 * @param {uint8_t} group
 * @return {*}
 */
void TIMER_IO_In_Config(uint8_t timer_id, uint8_t group)
{
    if (timer_id == TIMER_0)
    {
        PINMUX->PB9_CFG_F.SW_SRC_SEL = TIMER_IO_IN_AF_CFG;
    }

    if (timer_id == TIMER_1)
    {
        if (group == TIMER_GROUP_SEL_0)
        {
            PINMUX->PB11_CFG_F.SW_SRC_SEL = TIMER_IO_IN_AF_CFG;
        }
        else if (group == TIMER_GROUP_SEL_1)
        {
            PINMUX->PB16_CFG_F.SW_SRC_SEL = TIMER_IO_IN_AF_CFG;
        }
        else //both 
        {
            PINMUX->PB11_CFG_F.SW_SRC_SEL = TIMER_IO_IN_AF_CFG;
            PINMUX->PB16_CFG_F.SW_SRC_SEL = TIMER_IO_IN_AF_CFG;
        }
    }

    if (timer_id == TIMER_2)
    {
        if (group == TIMER_GROUP_SEL_0)
        {
            PINMUX->PB13_CFG_F.SW_SRC_SEL = TIMER_IO_IN_AF_CFG;
        }
        else if (group == TIMER_GROUP_SEL_1)
        {
            PINMUX->PB24_CFG_F.SW_SRC_SEL = TIMER_IO_IN_AF_CFG;
        }
        else //both 
        {
            PINMUX->PB13_CFG_F.SW_SRC_SEL = TIMER_IO_IN_AF_CFG;
            PINMUX->PB24_CFG_F.SW_SRC_SEL = TIMER_IO_IN_AF_CFG;
        }
    }

    if (timer_id == TIMER_3)
    {
        if (group == TIMER_GROUP_SEL_0)
        {
            PINMUX->PB22_CFG_F.SW_SRC_SEL = TIMER_IO_IN_AF_CFG;
        }
        else if (group == TIMER_GROUP_SEL_1)
        {
            PINMUX->PB25_CFG_F.SW_SRC_SEL = TIMER_IO_IN_AF_CFG;
        }
        else //both 
        {
            PINMUX->PB22_CFG_F.SW_SRC_SEL = TIMER_IO_IN_AF_CFG;
            PINMUX->PB25_CFG_F.SW_SRC_SEL = TIMER_IO_IN_AF_CFG;
        }
    }

    if (timer_id == TIMER_4)
    {
        if (group == TIMER_GROUP_SEL_0)
        {
            PINMUX->PB27_CFG_F.SW_SRC_SEL = TIMER_IO_IN_AF_CFG;
        }
        else if (group == TIMER_GROUP_SEL_1)
        {
            PINMUX->PB28_CFG_F.SW_SRC_SEL = TIMER_IO_PB28_AF_CFG;
        }
        else //both 
        {
            PINMUX->PB27_CFG_F.SW_SRC_SEL = TIMER_IO_IN_AF_CFG;
            PINMUX->PB28_CFG_F.SW_SRC_SEL = TIMER_IO_PB28_AF_CFG;
        }
    }
}


/**
 * @description: 
 * @param {uint8_t} timer_id
 * @param {uint8_t} channel
 * @param {uint8_t} matchNum
 * @param {uint8_t} mode
 * @return {*}
 */
void TIMER_MatchMode_Config(uint8_t timer_id, uint8_t channel, uint8_t matchannel, uint8_t mode)
{
    GPT_REG_TypeDef *timer = TIMER_SEL(timer_id);

    if (channel == TIMER_CHANNEL_A)
    {
        if (matchannel == MATCHANNEL_0)
        {
            timer->MATCHA_OP_F.MATCHA0_OP = mode;
        }

        if (matchannel == MATCHANNEL_1)
        {
            timer->MATCHA_OP_F.MATCHA1_OP = mode;
        }

        if (matchannel == MATCHANNEL_2)
        {
            timer->MATCHA_OP_F.MATCHA2_OP = mode;
        }

        if (matchannel == MATCHANNEL_3)
        {
            timer->MATCHA_OP_F.MATCHA3_OP = mode;
        }
    }
    else if (channel == TIMER_CHANNEL_B)
    {
        if (matchannel == MATCHANNEL_0)
        {
            timer->MATCHB_OP_F.MATCHB0_OP = mode;
        }

        if (matchannel == MATCHANNEL_1)
        {
            timer->MATCHB_OP_F.MATCHB1_OP = mode;
        }

        if (matchannel == MATCHANNEL_2)
        {
            timer->MATCHB_OP_F.MATCHB2_OP = mode;
        }

        if (matchannel == MATCHANNEL_3)
        {
            timer->MATCHB_OP_F.MATCHB3_OP = mode;
        }
    }
    else
    {
        if (matchannel == MATCHANNEL_0)
        {
            timer->MATCHA_OP_F.MATCHA0_OP = mode;
            timer->MATCHB_OP_F.MATCHB0_OP = mode;
        }

        if (matchannel == MATCHANNEL_1)
        {
            timer->MATCHA_OP_F.MATCHA1_OP = mode;
            timer->MATCHB_OP_F.MATCHB1_OP = mode;
        }

        if (matchannel == MATCHANNEL_2)
        {
            timer->MATCHA_OP_F.MATCHA2_OP = mode;
            timer->MATCHB_OP_F.MATCHB2_OP = mode;
        }

        if (matchannel == MATCHANNEL_3)
        {
            timer->MATCHA_OP_F.MATCHA3_OP = mode;
            timer->MATCHB_OP_F.MATCHB3_OP = mode;
        }  
    }
}
/**
 * @description: 
 * @param {uint8_t} timer_id
 * @param {uint8_t} channel
 * @param {uint8_t} matchNum
 * @param {uint16_t} *matchVal
 * @return {*}
 */
void TIMER_MatchVal_Config(uint8_t timer_id, uint8_t channel, uint8_t matchannel, uint16_t matchVal)
{
    GPT_REG_TypeDef *timer = TIMER_SEL(timer_id);

    if (channel == TIMER_CHANNEL_A)
    {
        if (matchannel == MATCHANNEL_0)
        {
            timer->MATCHA0_VAL_F.MATCHA0_VAL = matchVal;
        }

        if (matchannel == MATCHANNEL_1)
        {
            timer->MATCHA1_VAL_F.MATCHA1_VAL = matchVal;
        }

        if (matchannel == MATCHANNEL_2)
        {
            timer->MATCHA2_VAL_F.MATCHA2_VAL = matchVal;
        }

        if (matchannel == MATCHANNEL_3)
        {
            timer->MATCHA3_VAL_F.MATCHA3_VAL = matchVal;
        }

    }
    else if (channel == TIMER_CHANNEL_B)
    {
        if (matchannel == MATCHANNEL_0)
        {
            timer->MATCHB0_VAL_F.MATCHB0_VAL = matchVal;
        }

        if (matchannel == MATCHANNEL_1)
        {
            timer->MATCHB1_VAL_F.MATCHB1_VAL = matchVal;
        }

        if (matchannel == MATCHANNEL_2)
        {
            timer->MATCHB2_VAL_F.MATCHB2_VAL = matchVal;
        }

        if (matchannel == MATCHANNEL_3)
        {
            timer->MATCHB3_VAL_F.MATCHB3_VAL = matchVal;
        }
    }
    else
    {
        if (matchannel == MATCHANNEL_0)
        {
            timer->MATCHA0_VAL_F.MATCHA0_VAL = matchVal;
            timer->MATCHB0_VAL_F.MATCHB0_VAL = matchVal;
        }

        if (matchannel == MATCHANNEL_1)
        {
            timer->MATCHA1_VAL_F.MATCHA1_VAL = matchVal;
            timer->MATCHB1_VAL_F.MATCHB1_VAL = matchVal;
        }

        if (matchannel == MATCHANNEL_2)
        {
            timer->MATCHA2_VAL_F.MATCHA2_VAL = matchVal;
            timer->MATCHB2_VAL_F.MATCHB2_VAL = matchVal;
        }

        if (matchannel == MATCHANNEL_3)
        {
            timer->MATCHA3_VAL_F.MATCHA3_VAL = matchVal;
            timer->MATCHB3_VAL_F.MATCHB3_VAL = matchVal;
        }
    }
}

/**
 * @description:
 * @param {TIMER_CFG_T} *cfg
 * @return {*}
 */
void TIMER_Init(TIMER_INIT_T *timer_init_t)
{
    if(timer_init_t != NULL)
    {
        GPT_REG_TypeDef *timer = TIMER_SEL(timer_init_t->timer_id);

        timer->SW_FORCE_SHDW_F.SW_FORCE_SHDW = 1;
        timer->CTRL_F.MODE = timer_init_t->run_mode;
        timer->CTRL_F.CNT_WAVE = timer_init_t->cnt_mode;
        timer->CTRL_F.MATCH_NUM = timer_init_t->matchnum;
        timer->PRESCALE = timer_init_t->prescale;
        timer->PERIOD = timer_init_t->period;
        timer->REPEAT_VAL = timer_init_t->repeatval;
    }
}

