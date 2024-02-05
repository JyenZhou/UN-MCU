/*************************************************************************************
    Copyright (c) 2020 Tinychip Microelectronics Co.,Ltd
    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
**************************************************************************************/
#include "tc04xx.h"
#include "system_tc04xx.h"

#define     DEFAULT_RST_FILTER_CNT          0x400
#if defined (__CC_ARM) /* Keil ��Vision 5.29.0.0 */

extern uint32_t Load$$RW_IRAM1$$Base;     /* Load Address of DDR_RW_DATA region*/
extern uint32_t Image$$RW_IRAM1$$Base;    /* Exec Address of DDR_RW_DATA region*/
extern uint32_t Image$$RW_IRAM1$$Length;  /* Length of DDR_RW_DATA region*/
extern uint32_t Image$$RW_IRAM1$$ZI$$Base;
extern uint32_t Image$$RW_IRAM1$$ZI$$Limit;
#endif
uint32_t  SystemCoreClock = DEFAULT_SYSTEM_CLOCK;
#if defined (__CC_ARM) /* Keil ��Vision 5.29.0.0 */

void CopyDataRWtoImage(void)
{
    uint32_t *src;
    uint32_t *dst;
    uint32_t length;

    dst    = &(Image$$RW_IRAM1$$Base);
    src    = &(Load$$RW_IRAM1$$Base);
    length = (unsigned int) & (Image$$RW_IRAM1$$Length);
    length /= sizeof(unsigned int);

    if (dst != src)
    {
        while (length > 0)
        {
            dst[length - 1] = src[length - 1];
            length--;
        }
    }
}

#endif
/**
 * @description: SystemInit Get the SystemCoreClock
 * @param none
 * @return none
 */
uint32_t SystemGetHClkFreq(void)
{
    uint32_t u32Val = 0;
    uint8_t hclkSel = 0;
    hclkSel = CLKRST->HCLK_SEL_F.HCLK_SEL;
    switch (hclkSel)
    {
    // clk16m
    case CLOCK_SEL_16M:
    {
        u32Val = 16000000ul;
        break;
    }
    case CLOCK_SEL_32M:
    {
        u32Val = 32000000ul;
        break;
    }
    // clk32k
    case CLOCK_SEL_32K:
    case CLOCK_SEL_32K_2:
    {
        u32Val = 32768ul;
        break;
    }
    default:
        u32Val = 0u;
        break;
    }

    return u32Val;
}
/**
 * @description: SystemInit Update the SystemCoreClock
 * @param none
 * @return none
 */
void SystemCoreClockUpdate(void)
{
    /*Later update SystemCoreClock according to clock tree*/
    SystemCoreClock = SystemGetHClkFreq();
    /*select the interal clk as systick clk source */
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
}

/**
* @description: SystemInit Config the RC32M
* @param none
* @return none
*/
void SystemClock32M(void)
{
    uint8_t wait_cycle = 20;

    FLASH->RCTC_F.CONFIG_CYCLE_OF_RC = (1);

    ANAREG->OSCRC_16M_TOP_F.CTRL_FD_EN_0_ENABLE = ENABLE;
    while (wait_cycle--); //wait for ready
    CLKRST->HCLK_SEL_F.HCLK_SEL = CLOCK_SEL_32M;
    SystemCoreClockUpdate();
}

/**
* @description: SystemInit Load the Trim From Flash
* @param none
* @return none
*/
void CopyAnalogTrimValue(uint8_t index)
{
    switch (index)
    {
    case 0:
        //LDO15 trim
        ANAREG->TRIM_CONSTGM_VALUE_F.TRIM_CONSTGM = *(volatile uint32_t *)(0x10800C08);
        break;
    case 1:
        //RC32K trim
        ANAREG->TRIM_OSCRC32K_VALUE_F.OSCRC32K_TRIM_RES = *(volatile uint32_t *)(0x10800C0C);
        break;
    case 2:
        //RC16M trim
        ANAREG->TRIM_OSCRC16M_VALUE_F.TRIM_OSCRC16M = *(volatile uint32_t *)(0x10800C10);
        break;
    case 3:
        //BIAS trim
        ANAREG->TRIM_VBIAS_IBIAS_VALUE_F.TRIM_VBIAS_IBIAS_TOP = *(volatile uint32_t *)(0x10800C14);
        break;
    case 4:
        //BGR trim
        ANAREG->TRIM_BGR_VALUE_F.TRIM_BGR = *(volatile uint32_t *)(0x10800C18);
        break;
    case 5:
        break;
    case 6:
    {
        //SARADC trim
        SARADC->TRIM_ADSAR_VALUE0 = *(volatile uint32_t *)(0x10800C20);
        SARADC->TRIM_ADSAR_VALUE1 = *(volatile uint32_t *)(0x10800C24);
        SARADC->TRIM_ADSAR_VALUE2 = *(volatile uint32_t *)(0x10800C28);
        SARADC->TRIM_ADSAR_VALUE3 = *(volatile uint32_t *)(0x10800C2C);
        SARADC->TRIM_ADSAR_VALUE4 = *(volatile uint32_t *)(0x10800C30);
        SARADC->TRIM_ADSAR_VALUE5 = *(volatile uint32_t *)(0x10800C34);
        SARADC->TRIM_ADSAR_VALUE6 = *(volatile uint32_t *)(0x10800C38);
        SARADC->TRIM_ADSAR_VALUE7 = *(volatile uint32_t *)(0x10800C3C);
        SARADC->TRIM_ADSAR_VALUE8 = *(volatile uint32_t *)(0x10800C40);
        SARADC->TRIM_ADSAR_COEFF0 = 0;
        SARADC->TRIM_ADSAR_COEFF1 = 0;
        break;
    }
    case 7:
        SARADC->TRIM_ADSAR_COEFF0 = 0;
        SARADC->TRIM_ADSAR_COEFF1 = 0;
        break;
    default:
        break;
    }
}
/**
 * @description: SystemInit Load the Trim
 * @param none
 * @return none
 */
void SystemAnlogTrim()
{
    uint8_t i;
    uint8_t trimStatus = (uint8_t)(*(volatile uint32_t *)(0x10800C04));
    for (i = 0; i < 8; i++)
    {
        if (((trimStatus >> i) & 0x1) == 1)
            continue;
        CopyAnalogTrimValue(i);
    }
}
/**
 * @description: SystemInit Default Config the flash
 * @param none
 * @return none
 */
void SystemConfigFlash(void)
{
    FLASH->RCTC_F.CONFIG_CYCLE_OF_RC = (0);
}

/**
 * @description: SystemInit Default Config the Systick
 * @param none
 * @return none
 */
void SystemConfigSystick(void)
{
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
}
/**
 * @description: SystemInit Default Config the Rst cnt
 * @param {uint16_t} filter_cnt
 * @return {*}
 */
void SystemConfigRstFilterCnt(uint16_t filter_cnt)
{
    CLKRST->PAD_RSTN_FLT_F.FILTER_CNT_MAX = filter_cnt;

}
/**
 * @description: SystemInit
 * @param none
 * @return none
 */
void SystemInit(void)
{
 #if defined (__CC_ARM)
    CopyDataRWtoImage();
#endif
    SystemConfigSystick();
    SystemConfigFlash();
    SystemConfigRstFilterCnt(DEFAULT_RST_FILTER_CNT);    //0x400 - 32ms
    SystemAnlogTrim();
}
/**
 * @description: resume all interrupt enabled
 * @param none
 * @return none
 */
void interrupt_enable(void)
{
    __enable_irq();
}

/**
 * @description: mask all interrupt but NMI and HardFault
 * @param none
 * @return none
 */
void interrupt_disable(void)
{
    __disable_irq();
}

