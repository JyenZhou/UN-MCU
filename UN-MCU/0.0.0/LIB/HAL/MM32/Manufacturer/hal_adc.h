/***********************************************************************************************************************
    @file     hal_adc.h
    @author   VV TEAM
    @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE ADC
              FIRMWARE LIBRARY.
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
#ifndef __HAL_ADC_H
#define __HAL_ADC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Files includes ------------------------------------------------------------*/
#include "mm32_device.h"

/** @addtogroup MM32_Hardware_Abstract_Layer
  * @{
  */

/** @defgroup ADC_HAL
  * @brief ADC HAL modules
  * @{
  */

/** @defgroup ADC_Exported_Types
  * @{
  */

/**
  * @brief ADC_Channels
  */
typedef enum {
    ADC_Channel_0                       = 0x00,                                 /*!< ADC Channel 0 */
    ADC_Channel_1                       = 0x01,                                 /*!< ADC Channel 1 */
    ADC_Channel_2                       = 0x02,                                 /*!< ADC Channel 2 */
    ADC_Channel_3                       = 0x03,                                 /*!< ADC Channel 3 */
    ADC_Channel_4                       = 0x04,                                 /*!< ADC Channel 4 */
    ADC_Channel_5                       = 0x05,                                 /*!< ADC Channel 5 */
    ADC_Channel_6                       = 0x06,                                 /*!< ADC Channel 6 */
    ADC_Channel_7                       = 0x07,                                 /*!< ADC Channel 7 */

    ADC_Channel_8                       = 0x08,                                 /*!< ADC Channel 8 */
    ADC_Channel_VoltReference           = 0x08,                                 /*!< Internal reference voltage(ADC1) channel */
    ADC_Channel_Vrefint                 = 0x08,                                 /*!< Internal reference voltage(ADC1) channel */
} ADCCHANNEL_TypeDef;

/**
  * @brief ADC_Sampling_Times
  */
typedef enum {
    ADC_Samctl_2_5 = ADC_CFGR_SAMCTL_2_5,  ///< ADC sample time select 2.5t
    ADC_Samctl_3_5 = ADC_CFGR_SAMCTL_3_5,  ///< ADC sample time select 3.5t
    ADC_Samctl_4_5 = ADC_CFGR_SAMCTL_4_5,  ///< ADC sample time select 4.5t
    ADC_Samctl_5_5 = ADC_CFGR_SAMCTL_5_5,  ///< ADC sample time select 5.5t
    ADC_Samctl_6_5 = ADC_CFGR_SAMCTL_6_5,  ///< ADC sample time select 6.5t
    ADC_Samctl_7_5  = ADC_CFGR_SAMCTL_7_5,   ///< ADC sample time select 7.5t
    ADC_Samctl_14_5  = ADC_CFGR_SAMCTL_14_5,  ///< ADC sample time select 14.5t
    ADC_Samctl_29_5  = ADC_CFGR_SAMCTL_29_5,  ///< ADC sample time select 29.5t
    ADC_Samctl_42_5  = ADC_CFGR_SAMCTL_42_5,  ///< ADC sample time select 41.5t
    ADC_Samctl_56_5  = ADC_CFGR_SAMCTL_56_5,  ///< ADC sample time select 56.5t
    ADC_Samctl_72_5  = ADC_CFGR_SAMCTL_72_5,  ///< ADC sample time select 72.5t
    ADC_Samctl_240_5 = ADC_CFGR_SAMCTL_240_5  ///< ADC sample time select 240.5t
} ADCSAM_TypeDef;

/**
  * @brief ADC_Resolution
  */
typedef enum {
    ADC_Resolution_12b                  = ADC_CFGR_RSLTCTL_12,                  /*!< ADC resolution select 12bit */
    ADC_Resolution_11b                  = ADC_CFGR_RSLTCTL_11,                  /*!< ADC resolution select 11bit */
    ADC_Resolution_10b                  = ADC_CFGR_RSLTCTL_10,                  /*!< ADC resolution select 10bit */
    ADC_Resolution_9b                   = ADC_CFGR_RSLTCTL_9,                   /*!< ADC resolution select 9bit */
    ADC_Resolution_8b                   = ADC_CFGR_RSLTCTL_8                    /*!< ADC resolution select 8bit */
} ADCRSL_TypeDef;
/**
  * @brief ADC_Prescare
  */
typedef enum {
    ADC_PCLK2_PRESCARE_3                = ADC_CFGR_PRE_3,                       /*!< ADC preclk 3 */
    ADC_PCLK2_PRESCARE_5                = ADC_CFGR_PRE_5,                       /*!< ADC preclk 5 */
    ADC_PCLK2_PRESCARE_7                = ADC_CFGR_PRE_7,                       /*!< ADC preclk 7 */
    ADC_PCLK2_PRESCARE_9                = ADC_CFGR_PRE_9,                       /*!< ADC preclk 9 */
    ADC_PCLK2_PRESCARE_11               = ADC_CFGR_PRE_11,                      /*!< ADC preclk 11 */
    ADC_PCLK2_PRESCARE_13               = ADC_CFGR_PRE_13,                      /*!< ADC preclk 13 */
    ADC_PCLK2_PRESCARE_15               = ADC_CFGR_PRE_15,                      /*!< ADC preclk 15 */
    ADC_PCLK2_PRESCARE_17               = ADC_CFGR_PRE_17,                      /*!< ADC preclk 17 */

    ADC_PCLK2_PRESCARE_2                = ADC_CFGR_PRE_2,                       /*!< ADC preclk 2 */
    ADC_PCLK2_PRESCARE_4                = ADC_CFGR_PRE_4,                       /*!< ADC preclk 4 */
    ADC_PCLK2_PRESCARE_6                = ADC_CFGR_PRE_6,                       /*!< ADC preclk 6 */
    ADC_PCLK2_PRESCARE_8                = ADC_CFGR_PRE_8,                       /*!< ADC preclk 8 */
    ADC_PCLK2_PRESCARE_10               = ADC_CFGR_PRE_10,                      /*!< ADC preclk 10 */
    ADC_PCLK2_PRESCARE_12               = ADC_CFGR_PRE_12,                      /*!< ADC preclk 12 */
    ADC_PCLK2_PRESCARE_14               = ADC_CFGR_PRE_14,                      /*!< ADC preclk 14 */
    ADC_PCLK2_PRESCARE_16               = ADC_CFGR_PRE_16                       /*!< ADC preclk 16 */
} ADCPRE_TypeDef;

/**
  * @brief ADC_Conversion_Mode
  */
typedef enum {
    ADC_Mode_Imm                        = ADC_CR_IMM,                           /*!< ADC single convert mode */
    ADC_Mode_Scan                       = ADC_CR_SCAN,                          /*!< ADC single period convert mode */
    ADC_Mode_Continue                   = ADC_CR_CONTINUE                       /*!< ADC continue scan convert mode */
} ADCMODE_TypeDef;

/**
  * @brief ADC_Extrenal_Trigger_Sources_For_Regular_Channels_Conversion
  */
typedef enum {
    ADC1_ExternalTrigConv_T1_CC1        = ADC_CR_T1_CC1,
    ADC1_ExternalTrigConv_T1_CC2        = ADC_CR_T1_CC2,
    ADC1_ExternalTrigConv_T1_CC3        = ADC_CR_T1_CC3,
    ADC1_ExternalTrigConv_T3_TRIG       = ADC_CR_T3_TRIG,
    ADC1_ExternalTrigConv_T3_CC1        = ADC_CR_T3_CC1,
    ADC1_ExternalTrigConv_EXTI_11       = ADC_CR_EXTI_11,
    ADC1_ExternalTrigConv_T1_CC4_CC5    = ADC_CR_T1_CC4_CC5,
    ADC1_ExternalTrigConv_T1_TRIG       = ADC_CR_T1_TRIG,
    ADC1_ExternalTrigConv_T3_CC4        = ADC_CR_T3_CC4,
    ADC1_ExternalTrigConv_EXTI_15       = ADC_CR_EXTI_15,
    ADC1_ExternalTrigConv_T1_CC4        = ADC_CR_TIM1_CC4,
    ADC1_ExternalTrigConv_T1_CC5        = ADC_CR_TIM1_CC5
} EXTERTRIG_TypeDef;

/**
  * @brief ADC_Data_Align
  */
typedef enum {
    ADC_DataAlign_Right                 = ADC_CR_RIGHT,                         /*!< ADC data left align */
    ADC_DataAlign_Left                  = ADC_CR_LEFT                           /*!< ADC data right align */
} ADCDATAALI_TypeDef;

/**
  * @brief ADC_Flags_Definition
  */
typedef enum {
    ADC_IT_EOC                          = 1,                                    /*!< ADC conversion flag */
    ADC_FLAG_EOC                        = 1,
    ADC_IT_AWD                          = 2,                                    /*!< ADC window comparator flag */
    ADC_FLAG_AWD                        = 2
} ADCFLAG_TypeDef;

/**
  * @brief ADC Init Structure definition
  */
typedef struct {
    uint32_t                            ADC_Resolution;                         /*!< Convert data resolution */
    uint32_t                            ADC_PRESCARE;                           /*!< Clock prescaler */
    uint32_t                            ADC_Mode;                               /*!< ADC conversion mode */
    FunctionalState                     ADC_ContinuousConvMode;                 /*!< Useless just for compatibility */
    uint32_t                            ADC_ExternalTrigConv;                   /*!< External trigger source selection */
    uint32_t                            ADC_DataAlign;                          /*!< Data alignmentn */
} ADC_InitTypeDef;

/**
  * @}
  */

/** @defgroup ADC_Exported_Variables
  * @{
  */
#ifdef _HAL_ADC_C_

#define GLOBAL
#else
#define GLOBAL extern
#endif

#undef GLOBAL
/**
  * @}
  */

/** @defgroup ADC_Exported_Functions
  * @{
  */
void ADC_DeInit(ADC_TypeDef* adc);
void ADC_Init(ADC_TypeDef* adc, ADC_InitTypeDef* init_struct);
void ADC_StructInit(ADC_InitTypeDef* init_struct);
void ADC_Cmd(ADC_TypeDef* adc, FunctionalState state);
void ADC_ITConfig(ADC_TypeDef* adc, ADCFLAG_TypeDef adc_interrupt, FunctionalState state);
void ADC_SoftwareStartConvCmd(ADC_TypeDef* adc, FunctionalState state);
void ADC_RegularChannelConfig(ADC_TypeDef* adc, u32 channel, u8 rank, u32 sample_time);//ADCSAM_TypeDef
void ADC_ExternalTrigConvCmd(ADC_TypeDef* adc, FunctionalState state);
void ADC_AnalogWatchdogCmd(ADC_TypeDef* adc, FunctionalState state);
void ADC_AnalogWatchdogThresholdsConfig(ADC_TypeDef* adc, u16 high_threshold, u16 low_threshold);
void ADC_AnalogWatchdogSingleChannelConfig(ADC_TypeDef* adc, ADCCHANNEL_TypeDef channel);
void ADC_TempSensorVrefintCmd(FunctionalState state);
void ADC_ClearITPendingBit(ADC_TypeDef* adc, ADCFLAG_TypeDef adc_interrupt);
void ADC_ClearFlag(ADC_TypeDef* adc, ADCFLAG_TypeDef adc_flag);

uint16_t ADC_GetConversionValue(ADC_TypeDef* adc);

FlagStatus ADC_GetSoftwareStartConvStatus(ADC_TypeDef* adc);
FlagStatus ADC_GetFlagStatus(ADC_TypeDef* adc, ADCFLAG_TypeDef adc_flag);
ITStatus   ADC_GetITStatus(ADC_TypeDef* adc, ADCFLAG_TypeDef adc_interrupt);
void ADC_TempSensorCmd(FunctionalState state);
void ADC_VrefintCmd(FunctionalState state);
void ADC_ANY_CH_Config(ADC_TypeDef* adc, u8 rank, ADCCHANNEL_TypeDef adc_channel);
void ADC_ANY_NUM_Config(ADC_TypeDef* adc, u8 num);
void ADC_ANY_Cmd(ADC_TypeDef* adc, FunctionalState state);

void ADC_SampleTimeConfig(ADC_TypeDef* adc, ADCSAM_TypeDef sampleTime);
uint16_t ADC_GetChannelConvertedValue(ADC_TypeDef *adc, uint8_t channel);


/// @}

/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif


