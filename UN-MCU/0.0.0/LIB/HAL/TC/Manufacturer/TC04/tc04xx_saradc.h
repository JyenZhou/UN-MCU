/*
 * @Description: 
 * @Author: wangshi@tinychip.com.cn
 * @Date: 2023-01-19 10:17:23
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


#ifndef __TC04XX_SARADC_H__
#define __TC04XX_SARADC_H__
#include "tc04xx.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

//Saradc Channel 
#define                      SARADC_CHANNEL_0                             0x00U
#define                      SARADC_CHANNEL_1                             0x01U
#define                      SARADC_CHANNEL_2                             0x02U
#define                      SARADC_CHANNEL_3                             0x03U
#define                      SARADC_CHANNEL_4                             0x04U
#define                      SARADC_CHANNEL_5                             0x05U
#define                      SARADC_CHANNEL_6                             0x06U
#define                      SARADC_CHANNEL_7                             0x07U
#define                      SARADC_CHANNEL_GND                           0x08U
#define                      SARADC_CHANNEL_TEMP                          0x09U
#define                      SARADC_CHANNEL_INP_VSDIV                     0x0BU
#define                      SARADC_CHANNEL_INN_VSDIV                     0x0CU

//Saradc Trigger Count
#define                      SARADC_TRIG_CNT_1                            0x00U
#define                      SARADC_TRIG_CNT_8                            0x01U
#define                      SARADC_TRIG_CNT_16                           0x02U
#define                      SARADC_TRIG_CNT_32                           0x03U
#define                      SARADC_TRIG_CNT_64                           0x04U
#define                      SARADC_TRIG_CNT_128                          0x05U
#define                      SARADC_TRIG_CNT_256                          0x06U
#define                      SARADC_TRIG_CNT_480                          0x07U

//Saradc Vrefence 
#define                      SARADC_VREF_VDD                              0x00U
#define                      SARADC_VREF_LDOVS                            0x01U
#define                      SARADC_VREF_INTERAL                          0x02U

//Saradc Interal Vrefence 
#define                      SARADC_VREF_INITERAL_2_5V                    0x00U
#define                      SARADC_VREF_INITERAL_1_5V                    0x01U

//Saradc Fifo Format
#define                      SARADC_FIFO_FORMAT_0                         0x00U
#define                      SARADC_FIFO_FORMAT_1                         0x01U
#define                      SARADC_FIFO_FORMAT_2                         0x02U
#define                      SARADC_FIFO_FORMAT_3                         0x03U

//Saradc Fifothold
#define                      FIFO_THRHOLD_1WORD                           0x00U
#define                      FIFO_THRHOLD_2WORD                           0x01U
#define                      FIFO_THRHOLD_3WORD                           0x02U
#define                      FIFO_THRHOLD_4WORD                           0x03U

//Saradc PGA Select
#define                      SARADC_OUTPUT_NULL                           0x00U
#define                      SARADC_OUTPUT_PASS_PGA                       0x01U
#define                      SARADC_OUTPUT_PGA                            0x02U
#define                      SARADC_OUTPUT_FORBIDDEN                      0x03U

//Saradc Pair Select
#define                      SARADC_PAIR0_EN                              0x01U
#define                      SARADC_PAIR1_EN                              0x02U
#define                      SARADC_PAIR2_EN                              0x04U
#define                      SARADC_PAIR3_EN                              0x08U

//Saradc Power Control
#define                      SARADC_PGA_POWER0                           0x00U
#define                      SARADC_PGA_POWER1                           0x01U
#define                      SARADC_PGA_POWER2                           0x02U
#define                      SARADC_PGA_POWER3                           0x03U

//Saradc Interrupt Bit
#define                      SARADC_IT_COMP                              0x01U
#define                      SARADC_IT_DONE                              0x02U
#define                      SARADC_IT_OVRF                              0x04U
#define                      SARADC_IT_RXRDY                             0x08U
#define                      SARADC_IT_EMPTY                             0x10U

//Saradc PGA Gain Setting
#define                      SARADC_PGA_GAIN_PASS                        0x00U
#define                      SARADC_PGA_GAIN_X1                          0x01U
#define                      SARADC_PGA_GAIN_X2                          0x02U
#define                      SARADC_PGA_GAIN_X4                          0x03U
#define                      SARADC_PGA_GAIN_X8                          0x04U
#define                      SARADC_PGA_GAIN_X16                         0x05U
#define                      SARADC_PGA_GAIN_X32                         0x06U

//Saradc Ldovs Output Setting
#define                      SARADC_LDOVS_LEVEL0                         0x00U//1.8V
#define                      SARADC_LDOVS_LEVEL1                         0x01U//2.0V
#define                      SARADC_LDOVS_LEVEL2                         0x02U//2.2V
#define                      SARADC_LDOVS_LEVEL3                         0x03U//2.4V
#define                      SARADC_LDOVS_LEVEL4                         0x04U//2.6V
#define                      SARADC_LDOVS_LEVEL5                         0x05U//2.8V
#define                      SARADC_LDOVS_LEVEL6                         0x06U//3.0V
#define                      SARADC_LDOVS_LEVEL7                         0x07U//3.1V

//Saradc Ldovs Output Setting
#define                      SARADC_VSDIV_LEVEL0                         0x00U//0.3V
#define                      SARADC_VSDIV_LEVEL1                         0x01U//0.4V
#define                      SARADC_VSDIV_LEVEL2                         0x02U//0.5V
#define                      SARADC_VSDIV_LEVEL3                         0x03U//0.6V
#define                      SARADC_VSDIV_LEVEL4                         0x04U//0.7V
#define                      SARADC_VSDIV_LEVEL5                         0x05U//0.8V
#define                      SARADC_VSDIV_LEVEL6                         0x06U//0.9V
#define                      SARADC_VSDIV_LEVEL7                         0x07U//1.0V

//Saradc Ldovs Output Setting
#define                      SARADC_PGA_CHANNEL0                         0x00U
#define                      SARADC_PGA_CHANNEL1                         0x01U
#define                      SARADC_PGA_CHANNEL2                         0x02U
#define                      SARADC_PGA_CHANNEL3                         0x03U
#define                      SARADC_PGA_CHANNEL4                         0x04U
#define                      SARADC_PGA_CHANNEL5                         0x05U
#define                      SARADC_PGA_CHANNEL6                         0x06U
#define                      SARADC_PGA_CHANNEL7                         0x07U

// SARADC config info
typedef struct
{
    uint8_t p_ch;
    uint8_t n_ch;
//    uint8_t pair_sel;
    uint8_t pga_en;

    uint16_t smp_cycle;
    //uint8_t wait_cycle;   //reserved
    uint8_t smp_ch_cycle;
    uint8_t smp_dummy_cycle;
    uint8_t trig_cnt;
    uint8_t smp_avg_en;                        
    uint8_t fifo_threshold;     
    uint8_t fifo_format;
} SARADC_INIT_T;

typedef struct 
{
    uint8_t channel;
    uint8_t pga_stg1_gain;
    uint8_t pga_stg2_gain;
    uint8_t pga_stg1_power;
    uint8_t pga_stg2_power;
}SARADC_PGA_INIT_T;

typedef struct
{
    uint8_t channel;
    uint8_t idac1Cur;
    uint8_t idac1CurDir;
    uint8_t idac2CurDir;
    uint16_t idac2Cur;
}SARADC_IDAC_CFG_T;


typedef struct
{
    uint8_t pair_num;
    uint8_t pair_sel;
    uint8_t pair0_cfg;
    uint8_t pair1_cfg;
    uint8_t pair2_cfg;
    uint8_t pair3_cfg;
}SARADC_SCAN_INIT_T;


#define SARADC_INT_ENABLE(mask)            (SARADC->ADC_IMR &= ~(mask))
#define SARADC_INT_DISABLE(mask)           (SARADC->ADC_IMR |= (mask))

#define SARADC_INT_CLR(mask)               (SARADC->ADC_ICR = (mask))
#define SARADC_INT_FLAG(mask)              ((SARADC->ADC_ISR & (mask))?0x01U:0x00U)
       
#define SARADC_FIFO_STATUS()               (SARADC->ADC_FIFO_STATE_F.ENTRY_VALID)
#define SARADC_FIFO_DATA()                 (SARADC->ADC_FIFO_DATA_F.FIFO_RDATA&0x3FFFU)
       
       
#define SARADC_TRIG()                      (SARADC->ADC_CONF0_F.SW_START = 0x01U)
       
#define SARADC_DMA_ENABLE()                (SARADC->ADC_CONF0_F.DMA_EN = ENABLE)
#define SARADC_DMA_DISABLE()               (SARADC->ADC_CONF0_F.DMA_EN = DISABLE)
       
#define SARADC_TINYWORK_ENABLE()           (SARADC->ADC_CONF0_F.TRIG_EN = ENABLE)
#define SARADC_TINYWORK_DISABLE()          (SARADC->ADC_CONF0_F.TRIG_EN = DISABLE)
       
       
#define SARADC_ENABLE()                    (SARADC->ADC_ANA_TOP_F.CTRL_SAR_EN = ENABLE)
#define SARADC_DISABLE()                   (SARADC->ADC_ANA_TOP_F.CTRL_SAR_EN = DISABLE)
       
#define SARADC_FROM_PGA()                  (SARADC->ADC_ANA_TOP_F.CTRL_SAR_MUX_PGA = SARADC_OUTPUT_PGA)
#define SARADC_PASS_PGA()                  (SARADC->ADC_ANA_TOP_F.CTRL_SAR_MUX_PGA = SARADC_OUTPUT_PASS_PGA)

#define SARADC_PGA_ENABLE()                (SARADC->ADC_PGA_CTRL_F.CTRL_PGA_EN = ENABLE)
#define SARADC_PGA_DISABLE()               (SARADC->ADC_PGA_CTRL_F.CTRL_PGA_EN = DISABLE)
       
#define SARADC_VREFP_SET(vref)             (SARADC->ADC_ANA_TOP_F.CTRL_SAR_REFP_SEL = (vref))
#define SARADC_INTER_VREFP_SET(vrefp)      (SARADC->ADC_ANA_TOP_F.CTRL_SAR_VREFP_VOL_SEL = (vrefp))

#define SARADC_PGA_CHANNEL_SEL(ch)         (SARADC_BASE_ADDR+ADC_ADC_PGA_CHNL0_OFFSET+4*(ch))
#define SARADC_IDAC_ENABLE(ch)             (((ADC_ADC_PGA_CHNL_FIELD_T*)SARADC_PGA_CHANNEL_SEL(ch))->CTRL_IDAC_EN = ENABLE)
#define SARADC_IDAC_DISABLE(ch)            (((ADC_ADC_PGA_CHNL_FIELD_T*)SARADC_PGA_CHANNEL_SEL(ch))->CTRL_IDAC_EN = DISABLE)

#define SARADC_SCANNUM_CFG(num)            (SARADC->ADC_CONF1_F.SCAN_CHNUM = (num))

#define SARADC_TEMP_EN_MASK                (0x01U<<20)
#define SARADC_TEMP_DETECT_ENABLE()        (ANAREG->VBIAS_IBIAS_TOP_F.CTRL_VBIAS_IBIAS_TOP_25_1 |= SARADC_TEMP_EN_MASK)
#define SARADC_TEMP_DETECT_DISABLE()       (ANAREG->VBIAS_IBIAS_TOP_F.CTRL_VBIAS_IBIAS_TOP_25_1 &= ~SARADC_TEMP_EN_MASK)

void SARADC_Init(SARADC_INIT_T * saradc_init_t);
void SARADC_PGA_Init(SARADC_PGA_INIT_T *saradc_pga_init_t);
void SARADC_LDOVS_Config(uint8_t level,uint8_t en);
void SARADC_SetFclkSource(uint8_t clk_div);
void SARADC_IDAC_Config(uint8_t channel,SARADC_IDAC_CFG_T * cfg);
void SARADC_PGA_Config(uint8_t channel,uint8_t stg1_gain,uint8_t stg2_gain);
void SARADC_ScanModeInit(SARADC_SCAN_INIT_T * scan_init_t);
void SARADC_VSDIV_Config(uint8_t level, uint8_t en);
#ifdef __cplusplus
}
#endif
#endif


