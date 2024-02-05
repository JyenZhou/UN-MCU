/*
 * @Description: 
 * @Author: wangshi@tinychip.com.cn
 * @Date: 2023-02-02 15:13:03
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
#include "tc04xx_lpm.h"

/*
 * Enter Deep Sleep mode
 */
 __attribute__((section("RAMCODE")))
static void lpmEnterDeepSleep(uint8_t bOnExit)
{
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    if (bOnExit == TRUE)
    {
        SCB->SCR |= ((uint32_t)bOnExit << 1u);
    }
    else
    {
        SCB->SCR &= 0xDU;
    }

    __WFI();
}



void lpmPmuEnterState(uint8_t state, uint8_t bOnExit)
{
    switch(state)
    {
        case ACTIVE_MODE:
            PMU->SLEEP_MODE_F.SW_SLP_MODE = 0x00;
            break;
        case SLEEPWALK_MODE:
            PMU->SLEEP_MODE_F.SW_SLP_MODE = 0x01;
            lpmEnterDeepSleep(bOnExit);
            break;
        case SLEEP_MODE:
            PMU->SLEEP_MODE_F.SW_SLP_MODE = 0x02;
            lpmEnterDeepSleep(bOnExit);
            break;
        case HIBERBNATE_MODE:
            PMU->SLEEP_MODE_F.SW_SLP_MODE = 0x03;
            lpmEnterDeepSleep(bOnExit);
            break;
        default:
            //error
            break;
    }
}
