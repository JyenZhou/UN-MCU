/***********************************************************************************************************************
    @file     hal_rcc.c
    @author   VV TEAM
    @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE SERIES OF
              MM32 FIRMWARE LIBRARY.
  **********************************************************************************************************************
    @attention

    <h2><center>&copy; Copyright(c) <2023> <MindMotion></center></h2>

      Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
    following conditions are met:
    1. Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
       the following disclaimer in the documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or
       promote products derived from this software without specific prior written permission.

      THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
    INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
    WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *********************************************************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/
#define _HAL_RCC_C_

/* Files includes ------------------------------------------------------------*/
#include "hal_rcc.h"

uint8_t tbPresc[] = {0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 6, 7, 8, 9};

/** @addtogroup MM32_Hardware_Abstract_Layer
  * @{
  */

/** @addtogroup RCC_HAL
  * @{
  */

/** @addtogroup RCC_Exported_Functions
  * @{
  */

/**
  * @brief  Resets the RCC clock configuration to default state.
  * @param  None.
  * @retval None.
  */
void RCC_DeInit(void)
{
    SET_BIT(RCC->CR, RCC_CR_HSION);
    CLEAR_BIT(RCC->CFGR, RCC_CFGR_SW);
    CLEAR_BIT(RCC->CR, RCC_CR_HSEON | RCC_CR_CSSON);
    CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP);
    CLEAR_REG(RCC->CFGR);
    CLEAR_REG(RCC->CIR);
}

/**
  * @brief  Configures the External High Speed oscillator (HSE).
  * @param  hsesource: specifies the new state of HSE.
  *         This parameter can be one of the following values:
  * @arg    RCC_HSE_OFF: HSE oscillator OFF
  * @arg    RCC_HSE_ON: HSE oscillator ON
  * @arg    RCC_HSE_Bypass: HSE oscillator bypassed with external clock
  * @retval None.
  */
void RCC_HSEConfig(RCCHSE_TypeDef state)
{
    RCC->CR &= ~(RCC_CR_HSEBYP | RCC_CR_HSEON);
    switch (state) {
        case RCC_HSE_Bypass:
            RCC->CR |= RCC_CR_HSEBYP;
            RCC->CR |= RCC_CR_HSEON;
            break;
        case RCC_HSE_ON:
            RCC->CR |= RCC_CR_HSEON;
            break;
        default:
            break;
    }
}

/**
  * @brief  Checks whether the specified RCC flag is set or not.
  * @param  flag: specifies the flag to check.
  *         This parameter can be one of the following values:
  * @arg    RCC_FLAG_HSIRDY: HSI oscillator clock ready
  * @arg    RCC_FLAG_HSERDY: HSE oscillator clock ready
  * @arg    RCC_FLAG_PLLRDY: PLL clock ready
  * @arg    RCC_FLAG_LSERDY: LSE oscillator clock ready
  * @arg    RCC_FLAG_LSIRDY: LSI oscillator clock ready
  * @arg    RCC_FLAG_PINRST: Pin reset
  * @arg    RCC_FLAG_PORRST: POR/PDR reset
  * @arg    RCC_FLAG_SFTRST: Software reset
  * @arg    RCC_FLAG_IWDGRST: Independent Watchdog reset
  * @arg    RCC_FLAG_WWDGRST: Window Watchdog reset
  * @arg    RCC_FLAG_LPWRRST: Low Power reset
  * @retval The new state of flag (SET or RESET).
  */
FlagStatus RCC_GetFlagStatus(RCC_FLAG_TypeDef flag)
{
    return ((((flag >> 5) == CR_REG_INDEX) ? RCC->CR : (((flag >> 5) == BDCR_REG_INDEX) ? 0 : RCC->CSR)) &
            (1 << (flag & 0x1F)))
           ? SET : RESET;
}

/**
  * @brief  Waits for HSE start-up.
  * @param  None.
  * @retval An ErrorStatus enumuration value:
  *         - SUCCESS: HSE oscillator is stable and ready to use
  *         - ERROR: HSE oscillator not yet ready
  */
ErrorStatus RCC_WaitForHSEStartUp(void)
{
    uint32_t StartUpCounter = 0;

    FlagStatus HSEStatus;

    do {
        HSEStatus = RCC_GetFlagStatus(RCC_FLAG_HSERDY);
        StartUpCounter++;
    } while ((HSEStatus == RESET) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

    return (ErrorStatus)(RCC_GetFlagStatus(RCC_FLAG_HSERDY) != RESET) ? SUCCESS : ERROR;
}

/**
  * @brief  Waits for flag start-up.
  * @param  flag: specifies the flag to check.
  *         This parameter can be one of the following values:
  * @arg    RCC_FLAG_HSIRDY: HSI oscillator clock ready
  * @arg    RCC_FLAG_HSERDY: HSE oscillator clock ready
  * @arg    RCC_FLAG_PLLRDY: PLL clock ready
  * @arg    RCC_FLAG_LSERDY: LSE oscillator clock ready
  * @arg    RCC_FLAG_LSIRDY: LSI oscillator clock ready
  * @arg    RCC_FLAG_PINRST: Pin reset
  * @arg    RCC_FLAG_PORRST: POR/PDR reset
  * @arg    RCC_FLAG_SFTRST: Software reset
  * @arg    RCC_FLAG_IWDGRST: Independent Watchdog reset
  * @arg    RCC_FLAG_WWDGRST: Window Watchdog reset
  * @retval An ErrorStatus enumuration value:
  *         - SUCCESS: HSE oscillator is stable and ready to use
  *         - ERROR: HSE oscillator not yet ready
  */
ErrorStatus RCC_WaitForFlagStartUp(RCC_FLAG_TypeDef flag)
{
    uint32_t StartUpCounter = 0;

    while (RCC_GetFlagStatus(flag) == RESET) {
        if (StartUpCounter++ > HSE_STARTUP_TIMEOUT) {
            return ERROR;
        }
    }
    return SUCCESS;
}

/**
  * @brief  Enables or disables the Internal High Speed oscillator (HSI).
  * @param  state: new state of the HSI.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None.
  */
void RCC_HSICmd(FunctionalState state)
{
    MODIFY_REG(RCC->CR, RCC_CR_HSION, (state << RCC_CR_HSION_Pos));
}

/**
  * @brief  Configures the system clock (SYSCLK).
  * @param  sys_clk_source: specifies the clock source used as system
  *         clock. This parameter can be one of the following values:
  * @arg    RCC_HSI: specifies HSI as system clock
  * @arg    RCC_HSE: specifies HSE as system clock
  * @arg    RCC_PLL: specifies PLL as system clock
  * @arg    RCC_LSI: specifies LSI as system clock
  * @retval None.
  */
void RCC_SYSCLKConfig(SYSCLK_TypeDef sys_clk_source)
{
    MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, (sys_clk_source << RCC_CFGR_SW_Pos));
}

/**
  * @brief  Returns the clock source used as system clock.
  * @param  None.
  * @retval The clock source used as system clock. The returned value can
  *         be one of the following:
  *         - 0x00: HSI/6 used as system clock
  *         - 0x04: HSE used as system clock
  *         - 0x08: PLL used as system clock
  */
uint8_t RCC_GetSYSCLKSource(void)
{
    return ((uint8_t)READ_BIT(RCC->CFGR, RCC_CFGR_SWS));
}

/**
  * @brief  Configures the AHB clock (hclk).
  * @param  sys_clk: defines the AHB clock divider. This clock is derived
  *                    from the system clock (SYSCLK).
  *         This parameter can be one of the following values:
  * @arg    RCC_SYSCLK_Div1: AHB clock = SYSCLK
  * @arg    RCC_SYSCLK_Div2: AHB clock = SYSCLK/2
  * @arg    RCC_SYSCLK_Div4: AHB clock = SYSCLK/4
  * @arg    RCC_SYSCLK_Div8: AHB clock = SYSCLK/8
  * @arg    RCC_SYSCLK_Div16: AHB clock = SYSCLK/16
  * @arg    RCC_SYSCLK_Div64: AHB clock = SYSCLK/64
  * @arg    RCC_SYSCLK_Div128: AHB clock = SYSCLK/128
  * @arg    RCC_SYSCLK_Div256: AHB clock = SYSCLK/256
  * @arg    RCC_SYSCLK_Div512: AHB clock = SYSCLK/512
  * @retval None.
  */
void RCC_HCLKConfig(RCC_AHB_CLK_TypeDef sys_clk)
{
    MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, sys_clk);
}

/**
  * @brief  Configures the Low Speed APB clock (pclk1).
  * @param  hclk: defines the APB1 clock divider. This clock is derived from
  *                  the AHB clock (hclk).
  *         This parameter can be one of the following values:
  * @arg    RCC_HCLK_Div1: APB1 clock = hclk
  * @arg    RCC_HCLK_Div2: APB1 clock = hclk/2
  * @arg    RCC_HCLK_Div4: APB1 clock = hclk/4
  * @arg    RCC_HCLK_Div8: APB1 clock = hclk/8
  * @arg    RCC_HCLK_Div16: APB1 clock = hclk/16
  * @retval None.
  */
void RCC_PCLK1Config(RCC_APB1_CLK_TypeDef hclk)
{
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, hclk);
}

/**
  * @brief  Configures the ADC clock (ADCCLK).
  * @param  pclk1: defines the ADC clock divider. This clock is derived from
  *                   the APB1 clock (pclk1).
  *         This parameter can be one of the following values:
  * @arg    RCC_PCLK1_Div2: ADC clock = pclk1/2
  * @arg    RCC_PCLK1_Div4: ADC clock = pclk1/4
  * @arg    RCC_PCLK1_Div6: ADC clock = pclk1/6
  * @arg    RCC_PCLK1_Div8: ADC clock = pclk1/8
  * @retval None.
  */
void RCC_ADCCLKConfig(RCC_ADCCLKSOURCE_TypeDef pclk1)
{
    MODIFY_REG(RCC->CFGR, ADC_CFGR_PRE, pclk1);
}

/**
  * @brief  Enables or disables the Internal Low Speed oscillator (LSI).
  *         LSI can not be disabled if the IWDG is running.
  * @param  state: new state of the LSI.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None.
  */
void RCC_LSICmd(FunctionalState state)
{
    MODIFY_REG(RCC->CSR, RCC_CSR_LSION, (state << RCC_CSR_LSION_Pos));
}

/**
  * @brief  Returns the clock frequency of different on chip clocks.
  * @param  None.
  * @retval sys_clk : System clock frequency
  */
uint32_t RCC_GetSysClockFreq(void)
{
    uint32_t result;
    switch (RCC->CFGR & RCC_CFGR_SWS) {
        case RCC_CFGR_SWS_LSI:
            result = LSI_VALUE;
            break;

        case RCC_CFGR_SWS_HSE:
            result = HSE_VALUE;
            break;

        case RCC_CFGR_SWS_HSI:
            result = HSI_48MHz;
            break;

        default:
            result =  HSI_48MHz_DIV6;
            break;
    }
    return result;
}

/**
  * @brief  Returns the hclk frequency of different on chip clocks.
  * @param  None.
  * @retval hclk frequency
  */
uint32_t RCC_GetHCLKFreq(void)
{
    return (RCC_GetSysClockFreq() >> tbPresc[(RCC->CFGR & RCC_CFGR_HPRE) >> RCC_CFGR_HPRE_Pos]);
}

/**
  * @brief  Returns the pclk1 frequency of different on chip clocks.
  * @param  None.
  * @retval pclk1 frequency
  */
uint32_t RCC_GetPCLK1Freq(void)
{
    return (RCC_GetHCLKFreq() >> tbPresc[(RCC->CFGR & RCC_CFGR_PPRE1) >> RCC_CFGR_PPRE1_Pos]);
}

/**
  * @brief  Returns the frequency of different on chip clocks.
  * @param  clk: pointer to a RCC_ClocksTypeDef structure which
  *   will hold the clocks frequency.
  * @retval None.
  */
void RCC_GetClocksFreq(RCC_ClocksTypeDef* clk)
{
    uint8_t tbADCPresc[] = {2, 4, 6, 8};

    clk->SYSCLK_Frequency = RCC_GetSysClockFreq();
    clk->HCLK_Frequency   = RCC_GetHCLKFreq();
    clk->PCLK1_Frequency  = RCC_GetPCLK1Freq();

    clk->ADCCLK_Frequency = clk->PCLK1_Frequency / tbADCPresc[(RCC->CFGR & ADC_CFGR_PRE) >> ADC_CFGR_PRE_Pos];
}

/**
  * @brief  Enables or disables the AHB peripheral clock.
  * @param  ahb_periph: specifies the AHB peripheral to gates its clock.
  *   This parameter can be any combination of the following values:
  * @arg RCC_AHBENR_SRAM, RCC_AHBENR_FLITF,
  *      RCC_AHBENR_GPIOA, RCC_AHBENR_GPIOB,
  * @param  state: new state of the specified peripheral clock.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None.
  */
void RCC_AHBPeriphClockCmd(uint32_t ahb_periph, FunctionalState state)
{
    (state) ? (RCC->AHBENR |= ahb_periph) : (RCC->AHBENR &= ~ahb_periph);
}

/**
  * @brief  Enables or disables the Low Speed APB (APB1) peripheral clock.
  * @param  apb1_periph: specifies the APB1 peripheral to gates its
  *   clock.
  *   This parameter can be any combination of the following values:
  * @param  state: new state of the specified peripheral clock.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None.
  */
void RCC_APB1PeriphClockCmd(uint32_t apb1_periph, FunctionalState state)
{
    (state) ? (RCC->APB1ENR |= apb1_periph) : (RCC->APB1ENR &= ~apb1_periph);
}

/**
  * @brief  Forces or releases Low Speed APB (APB1) peripheral reset.
  * @param  apb1_periph: specifies the APB1 peripheral to reset.
  *   This parameter can be any combination of the following values:
  * @param  state: new state of the specified peripheral clock.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None.
  */
void RCC_APB1PeriphResetCmd(uint32_t apb1_periph, FunctionalState state)
{
    (state) ? (RCC->APB1RSTR |= apb1_periph) : (RCC->APB1RSTR &= ~apb1_periph);
}

/**
  * @brief  Forces or releases Low Speed AHB peripheral reset.
  * @param  ahb_periph: specifies the AHB peripheral to reset.
  *   This parameter can be any combination of the following values:
  * @param  state: new state of the specified peripheral clock.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None.
  */
void RCC_AHBPeriphResetCmd(uint32_t ahb_periph, FunctionalState state)
{
    (state) ? (RCC->AHBRSTR |= ahb_periph) : (RCC->AHBRSTR &= ~ahb_periph);
}

/**
  * @brief  Enables or disables the Clock Security System.
  * @param  state: new state of the Clock Security System..
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None.
  */
void RCC_ClockSecuritySystemCmd(FunctionalState state)
{
    MODIFY_REG(RCC->CR, RCC_CR_CSSON, (state << RCC_CR_CSSON_Pos));
}

/**
  * @brief  Selects the clock source to output on MCO pin.
  * @param  mco_src: specifies the clock source to output.
  *   This parameter can be one of the following values:
  * @arg RCC_MCO_NoClock: No clock selected
  * @arg RCC_MCO_LSI: LSI oscillator clock selected
  * @arg RCC_MCO_LSE: LSE oscillator clock selected
  * @arg RCC_MCO_SYSCLK: System clock selected
  * @arg RCC_MCO_HSI: HSI oscillator clock selected
  * @arg RCC_MCO_HSE: HSE oscillator clock selected
  * @arg RCC_MCO_PLLCLK_Div2: PLL clock divided by 2 selected
  * @retval None.
  */
void RCC_MCOConfig(RCC_MCO_TypeDef mco_src)
{
    MODIFY_REG(RCC->CFGR, RCC_CFGR_MCO, mco_src);
}

/**
  * @brief  Clears the RCC reset flags.
  *   The reset flags are: RCC_FLAG_PINRST, RCC_FLAG_PORRST,
  *   RCC_FLAG_SFTRST, RCC_FLAG_IWDGRST, RCC_FLAG_WWDGRST,
  *   RCC_FLAG_LPWRRST
  * @param  None.
  * @retval None.
  */
void RCC_ClearFlag(void)
{
    SET_BIT(RCC->CSR, RCC_CSR_RMVF);
}

/**
  * @brief  Enables or disables the specified RCC interrupts.
  * @param  it: specifies the RCC interrupt sources to be enabled or
  * disabled.
  *   This parameter can be any combination of the following values:
  * @arg RCC_IT_LSIRDY: LSI ready interrupt
  * @arg RCC_IT_LSERDY: LSE ready interrupt
  * @arg RCC_IT_HSIRDY: HSI ready interrupt
  * @arg RCC_IT_HSERDY: HSE ready interrupt
  * @arg RCC_IT_PLLRDY: PLL ready interrupt
  * @param  state: new state of the specified RCC interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None.
  */
void RCC_ITConfig(RCC_IT_TypeDef it, FunctionalState state)
{
    (state) ? SET_BIT(RCC->CIR, it << RCC_CIR_LSIRDYIE_Pos) : CLEAR_BIT(RCC->CIR, it << RCC_CIR_LSIRDYIE_Pos);
}

/**
  * @brief  Checks whether the specified RCC interrupt has occurred or not.
  * @param  it: specifies the RCC interrupt source to check.
  *   This parameter can be one of the following values:
  * @arg RCC_IT_LSIRDY: LSI ready interrupt
  * @arg RCC_IT_LSERDY: LSE ready interrupt
  * @arg RCC_IT_HSIRDY: HSI ready interrupt
  * @arg RCC_IT_HSERDY: HSE ready interrupt
  * @arg RCC_IT_PLLRDY: PLL ready interrupt
  * @arg RCC_IT_CSS: Clock Security System interrupt
  * @retval The new state of it (SET or RESET).
  */
ITStatus RCC_GetITStatus(RCC_IT_TypeDef it)
{
    return (ITStatus)READ_BIT(RCC->CIR, (it << RCC_CIR_LSIRDYF_Pos));
}

/**
  * @brief  Clears the RCC interrupt pending bits.
  * @param  it: specifies the interrupt pending bit to clear.
  *   This parameter can be any combination of the following values:
  * @arg RCC_IT_LSIRDY: LSI ready interrupt
  * @arg RCC_IT_LSERDY: LSE ready interrupt
  * @arg RCC_IT_HSIRDY: HSI ready interrupt
  * @arg RCC_IT_HSERDY: HSE ready interrupt
  * @arg RCC_IT_PLLRDY: PLL ready interrupt
  * @arg RCC_IT_CSS: Clock Security System interrupt
  * @retval None.
  */
void RCC_ClearITPendingBit(uint8_t it)
{
    SET_BIT(RCC->CIR, (it << RCC_CIR_LSIRDYC_Pos));
}

/**
  * @brief  Forces or releases Low Speed APB (APB1) peripheral reset.
  * @param  apb1_periph: specifies the APB1 peripheral to reset.
  *   This parameter can be any combination of the following values:
  * @retval None.
  */
void RCC_APB1PeriphReset(uint32_t apb1_periph)
{
    RCC->APB1RSTR |= apb1_periph;
    RCC->APB1RSTR &= ~apb1_periph;
}

/**
  * @brief  Forces or releases High Speed AHB (AHB1) peripheral reset.
  * @param  ahb1_periph: specifies the AHB1 peripheral to reset.
  *   This parameter can be any combination of the following values:
  * @retval None.
  */
void RCC_AHBPeriphReset(uint32_t ahb1_periph)
{
    RCC->AHBRSTR |= ahb1_periph;
    RCC->AHBRSTR &= ~ahb1_periph;
}

/* New Function Interface                                                     */

/**
  * @brief  Forces or releases Low Speed APB (APB1) peripheral reset.
  * @param  apb1_periph: specifies the APB1 peripheral to reset.
  *   This parameter can be any combination of the following values:
  * @retval None.
  */
void exRCC_APB1PeriphReset(uint32_t apb1_periph)
{
    RCC->APB1RSTR |= apb1_periph;
    RCC->APB1RSTR &= ~apb1_periph;
}



/**
  * @brief  Forces or releases High Speed AHB (AHB1) peripheral reset.
  * @param  ahb1_periph: specifies the AHB1 peripheral to reset.
  *   This parameter can be any combination of the following values:
  * @retval None.
  */
void exRCC_AHBPeriphReset(uint32_t ahb1_periph)
{
    RCC->AHBRSTR |= ahb1_periph;
    RCC->AHBRSTR &= ~ahb1_periph;
}

/**
  * @brief  Disable systick
  * @param  None.
  * @retval None.
  */
void exRCC_SystickDisable(void)
{
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

/**
  * @brief  Enable systick
  * @param  None.
  * @retval None.
  */
void exRCC_SystickEnable(uint32_t sys_tick_period)
{
    SysTick_Config(RCC_GetHCLKFreq() / 1000000 * sys_tick_period);
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
