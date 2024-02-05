/***********************************************************************************************************************
    @file     hal_adc.c
    @author   VV TEAM
    @brief    THIS FILE PROVIDES ALL THE ADC FIRMWARE FUNCTIONS.
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
#define _HAL_ADC_C_

/* Files includes ------------------------------------------------------------*/
#include "hal_adc.h"

/** @addtogroup MM32_Hardware_Abstract_Layer
  * @{
  */

/** @addtogroup ADC_HAL
  * @{
  */

/** @addtogroup ADC_Exported_Functions
  * @{
  */

/**
  * @brief  Deinitializes the adc peripheral registers to their default
  *         reset values.
  * @param  adc: select the ADC peripheral.
  * @retval None.
  */
void ADC_DeInit(ADC_TypeDef* adc)
{
    if(ADC1 == adc) {
        exRCC_APB1PeriphReset(RCC_APB1RSTR_ADC1);
    }
}

/**
  * @brief  Initializes the adc peripheral according to the specified parameters
  *         in the init_struct, Please use this function if you want to be
  *         compatible with older versions of the library.
  * @param  adc: select the ADC peripheral.
  * @param  init_struct: pointer to an ADC_InitTypeDef structure that contains
  *         the configuration information for the specified ADC peripheral.
  * @retval None.
  */
void ADC_Init(ADC_TypeDef* adc, ADC_InitTypeDef* init_struct)
{
    adc->ADCFG &= ~(ADC_CFGR_PRE | ADC_CFGR_RSLTCTL);
    adc->ADCFG |= (uint32_t)(init_struct->ADC_PRESCARE) | init_struct->ADC_Resolution;

    adc->ADCR &= ~(ADC_CR_ALIGN | ADC_CR_MODE | ADC_CR_TRGSEL);
    adc->ADCR |= ((uint32_t)init_struct->ADC_DataAlign) | init_struct->ADC_ExternalTrigConv | ((uint32_t)init_struct->ADC_Mode);
}

/**
  * @brief  Fills each init_struct member with its default value.
  * @param  init_struct : pointer to an ADC_InitTypeDef structure which will be
  *         initialized.
  * @retval None.
  */
void ADC_StructInit(ADC_InitTypeDef* init_struct)
{
    init_struct->ADC_Resolution         = ADC_Resolution_12b;
    init_struct->ADC_PRESCARE           = ADC_PCLK2_PRESCARE_2;
    init_struct->ADC_Mode               = ADC_CR_IMM;                           /*!< ADC_Mode_Single; */
    init_struct->ADC_ContinuousConvMode = DISABLE;                              /*!< useless */
    init_struct->ADC_ExternalTrigConv   = ADC1_ExternalTrigConv_T1_CC1;
    init_struct->ADC_DataAlign          = ADC_DataAlign_Right;
}

/**
  * @brief  Enables or disables the specified ADC peripheral.
  * @param  adc:select the ADC peripheral.
  * @param  state: new state of the adc peripheral.
  * @retval None.
  */
void ADC_Cmd(ADC_TypeDef* adc, FunctionalState state)
{
    (state) ? (adc->ADCFG |= ADC_CFGR_ADEN) : (adc->ADCFG &= ~ADC_CFGR_ADEN);
}

/**
  * @brief  Enables or disables the specified ADC interrupts.
  * @param  adc: select the ADC peripheral.
  * @param  adc_interrupt: specifies the ADC interrupt sources to be enabled or disabled.
  * @param  state: New state of the specified ADC interrupts.
  * @retval None.
  */
void ADC_ITConfig(ADC_TypeDef* adc, ADCFLAG_TypeDef adc_interrupt, FunctionalState state)
{
    if (adc_interrupt == ADC_IT_EOC)
        (state) ? (adc->ADCR |= ADC_CR_ADIE) : (adc->ADCR &= ~ADC_CR_ADIE);
    else
        (state) ? (adc->ADCR |= ADC_CR_ADWIE) : (adc->ADCR &= ~ADC_CR_ADWIE);
}

/**
  * @brief  Enables or disables the selected ADC software start conversion .
  * @param  adc:  select the ADC peripheral.
  * @param  state: New state of the selected ADC software start conversion.
  * @retval None.
  */
void ADC_SoftwareStartConvCmd(ADC_TypeDef* adc, FunctionalState state)
{
    (state) ? (adc->ADCR |= ADC_CR_ADST) : (adc->ADCR &= ~ADC_CR_ADST);
}

/**
  * @brief  Gets the selected ADC Software start conversion Status.
  * @param  adc: select the ADC peripheral.
  * @retval  The new state of ADC software start conversion (SET or RESET).
  */
FlagStatus ADC_GetSoftwareStartConvStatus(ADC_TypeDef* adc)
{
    return (((adc->ADCR & ADC_CR_ADST) != (uint32_t)RESET) ? SET : RESET);
}

/**
  * @brief  Enables or disables the adc conversion through external trigger.
  * @param  adc: select the ADC peripheral.
  * @param  state: New state of the selected ADC external trigger.
  * @retval None.
  */
void ADC_ExternalTrigConvCmd(ADC_TypeDef* adc, FunctionalState state)
{
    (state) ? (adc->ADCR |= ADC_CR_TRGEN) : (adc->ADCR &= ~ADC_CR_TRGEN);
}


/**
  * @brief  Returns the last adc conversion result data for regular channel.
  * @param  adc: select the ADC peripheral.
  * @retval The data conversion value.
  */
uint16_t ADC_GetConversionValue(ADC_TypeDef* adc)
{
    return (uint16_t)adc->ADDATA;
}

/**
  * @brief  Returns the last ADC conversion result data in dual mode.
  * @param  None
  * @retval The Data conversion value.
  */
uint32_t ADC_GetDualModeConversionValue()
{
    return (*(uint32_t*)ADC1_BASE);
}


/**
  * @brief  Enables or disables the analog watchdog.
  * @param  adc:  to select the ADC peripheral.
  * @param  state: New state of the selected ADC analog watchdog.
  * @retval None.
  */
void ADC_AnalogWatchdogCmd(ADC_TypeDef* adc, FunctionalState state)
{
    (state) ? (adc->ADCFG |= ADC_CFGR_ADWEN) : (adc->ADCFG &= ~ADC_CFGR_ADWEN);
}

/**
  * @brief  Configures the high and low thresholds of the analog watchdog.
  * @param  adc:  select the ADC peripheral.
  * @param  high_threshold: the ADC analog watchdog High threshold value.
  *         This parameter must be a 12bit value.
  * @param  low_threshold: the ADC analog watchdog Low threshold value.
  *         This parameter must be a 12bit value.
  * @retval None.
  */
void ADC_AnalogWatchdogThresholdsConfig(ADC_TypeDef* adc, uint16_t high_threshold, uint16_t low_threshold)
{
    uint32_t tempThreshold;
    tempThreshold = high_threshold;
    adc->ADCMPR    = (tempThreshold << 16) | low_threshold;
}

/**
  * @brief  Configures the analog watchdog guarded single channel
  * @param  adc: select the ADC peripheral.
  * @param  channel: the ADC channel to configure for the analog watchdog.
  * @retval None.
  */
void ADC_AnalogWatchdogSingleChannelConfig(ADC_TypeDef* adc, ADCCHANNEL_TypeDef channel)
{
    adc->ADCR &= ~ADC_CR_CMPCH;
    adc->ADCR |= (channel << ADC_CR_CMPCH_Pos);
}

/**
  * @brief  Enables or disables the temperature sensor and Vrefint channel.
  * @param  state: New state of the temperature sensor.
  * @retval None.
  */
void ADC_TempSensorVrefintCmd(FunctionalState state)
{
    (state) ? (ADC1->ADCFG |=  (ADC_CFGR_VEN))
    : (ADC1->ADCFG &= ~(ADC_CFGR_VEN));
}

/**
  *  @brief  Enables or disables the temperature sensor .
  *  @param  state: New state of the temperature sensor.
  *  @retval None.
  */
void ADC_TempSensorCmd(FunctionalState state)
{
    ADC_TempSensorVrefintCmd(state);
}
/**
  * @brief  Enables or disables the Vrefint channel.
  * @param  state: New state of the Vrefint channel.
  * @retval None.
  */
void ADC_VrefintCmd(FunctionalState state)
{
    ADC_TempSensorVrefintCmd(state);
}

/**
  * @brief  Checks whether the specified ADC flag is set or not.
  * @param  adc: select the ADC peripheral.
  * @param  adc_flag: specifies the flag to check.
  * @retval The New state of adc_flag (SET or RESET).
  */
FlagStatus ADC_GetFlagStatus(ADC_TypeDef* adc, ADCFLAG_TypeDef adc_flag)
{
    return (adc_flag == ADC_IT_EOC) ? ((adc->ADSTA & ADC_SR_ADIF) ? SET : RESET) : ((adc->ADSTA & ADC_SR_ADWIF) ? SET : RESET);
}

/**
  * @brief  Clears the adc's pending flags.
  * @param  adc: select the ADC peripheral.
  * @param  adc_flag: specifies the flag to clear.
  * @retval None.
  */
void ADC_ClearFlag(ADC_TypeDef* adc, ADCFLAG_TypeDef adc_flag)
{
    (adc_flag == ADC_IT_EOC) ? (adc->ADSTA |= ADC_SR_ADIF) : (adc->ADSTA |= ADC_SR_ADWIF);
}

/**
  * @brief  Checks whether the specified adc's interrupt has occurred or not.
  * @param  adc: select the ADC peripheral.
  * @param  adc_interrupt: specifies the ADC interrupt source to check.
  * @retval The new state of adc_interrupt (SET or RESET).
  */
ITStatus ADC_GetITStatus(ADC_TypeDef* adc, ADCFLAG_TypeDef adc_interrupt)
{
    return (adc_interrupt == ADC_IT_EOC) ? ((adc->ADSTA & ADC_SR_ADIF) ? SET : RESET) : ((adc->ADSTA & ADC_SR_ADWIF) ? SET : RESET);
}

/**
  * @brief  Clears the adc's interrupt pending bits.
  * @param  adc: select the ADC peripheral.
  * @param  adc_interrupt: specifies the ADC interrupt pending bit to clear.
  * @retval None.
  */
void ADC_ClearITPendingBit(ADC_TypeDef* adc, ADCFLAG_TypeDef adc_interrupt)
{
    (adc_interrupt == ADC_IT_EOC) ? (adc->ADSTA |= ADC_SR_ADIF) : (adc->ADSTA |= ADC_SR_ADWIF);
}

/**
  * @brief  Configures the adc any channels conversion anychan and channel.
  * @param  adc: select the ADC peripheral.
  * @param  anychan: anychan can be 0x0~0xf for the convert sequence.
  * @param  adc_channel: Configuring the target channel to be converted.
  * @retval None.
  */
void ADC_ANY_CH_Config(ADC_TypeDef* adc, uint8_t rank, ADCCHANNEL_TypeDef adc_channel)
{
    rank = rank & 0xF;
    if(rank < 8) {
        adc->CHANY0 &= ~(0x0F << (4 * rank));
        adc->CHANY0 |= (adc_channel << (4 * rank));
    }
    else {
        adc->CHANY1 &= ~(0x0F << (4 * (rank - 8)));
        adc->CHANY1 |= (adc_channel << (4 * (rank - 8)));
    }
}

/**
  * @brief  Configures the adc any channels conversion Max rank number
  * @param  adc: select the ADC peripheral.
  * @param  num: Configuring the max rank number for the ADC.
  * @retval None.
  */
void ADC_ANY_NUM_Config(ADC_TypeDef* adc, uint8_t num)
{
    if(num > 15) num = 15;                                                      //15 ? 16 need to be confirmed
    adc->ANYCFG = num;
}

/**
  * @brief  Enables or disables the ANY channel converter.
  * @param  state: enable or disable the ANY channel converter mode.
  * @retval None.
  */
void ADC_ANY_Cmd(ADC_TypeDef* adc, FunctionalState state)
{
    (state) ? (adc->ANYCR |= ADC1_CHANY_CR_MDEN) : (adc->ANYCR &= ~ADC1_CHANY_CR_MDEN);
}

/** 
  * @brief  Set ADCn sample time.
  * @param  ADCn: where n can be 1, 2 to select the ADC peripheral.
  * @param  sampleTime: the ADC Channel n Sample time to configure.
  * @retval None.
  */
void ADC_SampleTimeConfig(ADC_TypeDef* adc, ADCSAM_TypeDef sampleTime)
{
    adc->CFGR &= ~ADC_CFGR_SAMCTL;
    adc->CFGR |= sampleTime;
}


/**
  * @brief  Get channel convertion result.
  * @param  adc  :   select the ADC peripheral.
  * @param  channel : the valaue may ADC_Channel_0, ... , ADC_Channel_8 etc.
  * @retval The Data conversion value.
  */
uint16_t ADC_GetChannelConvertedValue(ADC_TypeDef *adc, uint8_t channel)
{
    return ((uint16_t)(*(__IO uint32_t *)((uint32_t)adc + 0x18 + ((uint32_t)channel << 2))));
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

