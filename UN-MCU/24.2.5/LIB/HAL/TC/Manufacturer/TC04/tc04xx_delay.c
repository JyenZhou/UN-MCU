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
#include "tc04xx_delay.h"
#include "system_tc04xx.h"
#include <stdbool.h>

/**
 * @description: 
 * @param {uint32_t} ms
 * @return {*}
 */
void delay_ms(uint32_t ms)
{
    uint32_t ms_cnt = SystemCoreClock/1000;
    bool tick_done = true;
    
    SysTick->LOAD = ms*ms_cnt;
    SysTick->VAL  = 0;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

    do
    {
        if((SysTick->CTRL & 0x10000U) != 0x00U)
        {
            tick_done = false;
        }
    } while(tick_done);
    
    
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

/**
 * @description: 
 * @param {uint32_t} ms
 * @return {*}
 */
void delay_us(uint32_t us)
{
    uint32_t us_cnt = SystemCoreClock/1000000;
    bool tick_done = true;
    
    SysTick->LOAD = us*us_cnt;
    SysTick->VAL  = 0;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

    do
    {
        if((SysTick->CTRL & 0x10000U) != 0x00U)
        {
            tick_done = false;
        }
    } while(tick_done);
    
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

