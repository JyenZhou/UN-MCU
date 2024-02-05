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
#ifndef __TC04XX_TIMER_H__
#define __TC04XX_TIMER_H__
#include "tc04xx.h"

 /* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

//TIMER ID
#define                      TIMER_0                                      0x00U
#define                      TIMER_1                                      0x01U
#define                      TIMER_2                                      0x02U
#define                      TIMER_3                                      0x03U
#define                      TIMER_4                                      0x04U

//TIMER Count Mode
#define                      TIMER_CNT_EDGE                               0x00U
#define                      TIMER_CNT_CENTER                             0x01U

//TIMER CHANNEL TYPE
#define                      TIMER_CHANNEL_A                              0x00U
#define                      TIMER_CHANNEL_B                              0x01U
#define                      TIMER_CHANNEL_ALL                            0x02U

//TIMER GROUP TYPE
#define                      TIMER_GROUP_SEL_0                            0x00U
#define                      TIMER_GROUP_SEL_1                            0x01U
#define                      TIMER_GROUP_SEL_ALL                          0x02U

//TIMER MODE SET
#define                      TIMER_MODE_SIMPLE_TIMER                      0x00U
#define                      TIMER_MODE_INPUT_PLUSE_CNT                   0x01U
#define                      TIMER_MODE_INPUT_STAMP                       0x02U
#define                      TIMER_MODE_OUTPUT_CMP                        0x03U
#define                      TIMER_MODE_SINGLE_PLUSE                      0x04U
#define                      TIMER_MODE_PWM                               0x05U
#define                      TIMER_MODE_PWM_PAIR                          0x06U
#define                      TIMER_MODE_PWM_PAIR_WITH_DELTA               0x07U

//TIMER MATCHANNEL
#define                      MATCHANNEL_0                                 0x00U
#define                      MATCHANNEL_1                                 0x01U
#define                      MATCHANNEL_2                                 0x02U
#define                      MATCHANNEL_3                                 0x03U

//TIMER MATCHANNEL NUM SELECT
#define                      MATCHNUM_0_0                                 0x00U
#define                      MATCHNUM_0_1                                 0x01U
#define                      MATCHNUM_0_2                                 0x02U
#define                      MATCHNUM_0_3                                 0x03U

//TIMER OUT MODE
#define                      TIMER_OUT_MODE_ACTIVE                        0x00U
#define                      TIMER_OUT_MODE_INACTIVE                      0x01U
#define                      TIMER_OUT_MODE_TOGGLE                        0x02U
#define                      TIMER_OUT_MODE_FROZEN                        0x03U
#define                      TIMER_OUT_MODE_PWM1                          0x04U
#define                      TIMER_OUT_MODE_PWM2                          0x05U
#define                      TIMER_OUT_MODE_FORCE_ACTIVE                  0x06U
#define                      TIMER_OUT_MODE_FORCE_INACTIVE                0x07U

//TIMER CAPTURE MODE
#define                      TIMER_LATCH_MODE_RISING                      0x00U
#define                      TIMER_LATCH_MODE_RISING_FALLING              0x01U


typedef struct
{
  uint8_t timer_id;                    
  uint8_t run_mode;
  uint8_t cnt_mode;          
  uint8_t matchnum;          
  uint32_t prescale;         
  uint32_t period;           
  uint32_t repeatval;        
}TIMER_INIT_T;



#define TIMER_SEL(id)                   ((GPT_REG_TypeDef *)(GPT0_BASE_ADDR+((id)*0x10000)))
#define TIMER_ENABLE(id)                (TIMER_SEL(id)->CTRL_F.CNT_EN = ENABLE)
#define TIMER_DISABLE(id)               (TIMER_SEL(id)->CTRL_F.CNT_EN = DISABLE)

#define TIMER_IT_ENABLE(id)             (TIMER_SEL(id)->INT_MSK_F.INT_MSK = 0)
#define TIMER_IT_DISABLE(id)            (TIMER_SEL(id)->INT_MSK_F.INT_MSK = 1)
#define TIMER_IT_STATUS(id)             (TIMER_SEL(id)->INT_STATUS_F.INT_STATUS)
#define TIMER_IT_CLEAR(id)              ((TIMER_SEL(id)->INT_CLR_F.INT_CLR) = 0x01)
#define TIMER_DELTA_SET(id,delta)       (TIMER_SEL(id)->DELTA_F.DELTA = (delta))

#define TIMER_SETTING_UPDATE(id)        (TIMER_SEL(id)->CTRL_F.MODIFY_DONE = 0x01)
#define TIMER_LATCH_MODE_SET(id,mode)   (TIMER_SEL(id)->CTRL_F.LATCH_CNT_CTRL = (mode))
#define TIMER_LATCH_CNT_GET(id)         (TIMER_SEL(id)->LATCHED_CNT_F.LATCHED_CNT&0xFFFF)

void TIMER_SetFclkSource(uint8_t timer_id,uint8_t clk_src,uint8_t clk_div);
void TIMER_Init(TIMER_INIT_T * timer_init_t);


void TIMER_IO_Out_Config(uint8_t timer_id, uint8_t group, uint8_t channel);
void TIMER_IO_In_Config(uint8_t timer_id, uint8_t group);
void TIMER_MatchVal_Config(uint8_t timer_id, uint8_t channel, uint8_t matchannel, uint16_t matchVal);
void TIMER_MatchMode_Config(uint8_t timer_id, uint8_t channel, uint8_t matchannel, uint8_t mode);
#ifdef __cplusplus
}
#endif
#endif
