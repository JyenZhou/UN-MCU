#ifndef __STEPMOTOR1_TIM_H__
#define __STEPMOTOR1_TIM_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "JHAL.h"
/* ���Ͷ��� ------------------------------------------------------------------*/





typedef struct
{
    __IO uint8_t  run_state ;  // �����ת״̬
    __IO uint8_t  dir ;        // �����ת����
    __IO int32_t  step_delay;  // �¸��������ڣ�ʱ������������ʱΪ���ٶ�
    __IO uint32_t decel_start; // ��������λ��
    __IO int32_t  decel_val;   // ���ٽ׶β���
    __IO int32_t  min_delay;   // ��С��������(����ٶȣ������ٶ��ٶ�)
    __IO int32_t  accel_count; // �Ӽ��ٽ׶μ���ֵ
} speedRampData;


#define DIR1_Pin GPIO_PIN_11
#define DIR1_GPIO_Port GPIOC
#define BJ1_Motor1_PUL1_Pin GPIO_PIN_11
#define BJ1_Motor1_PUL1_GPIO_Port GPIOA
/* �궨�� --------------------------------------------------------------------*/
#define STEPMOTOR_TIMx                        TIM1
#define STEPMOTOR_TIM_RCC_CLK_ENABLE()        __HAL_RCC_TIM1_CLK_ENABLE()
#define STEPMOTOR_TIM_RCC_CLK_DISABLE()       __HAL_RCC_TIM1_CLK_DISABLE()
#define STEPMOTOR_TIM_CHANNEL_x                TIM_CHANNEL_4
#define STEPMOTOR_TIM_IT_CCx                  TIM_IT_CC4
#define STEPMOTOR_TIM_FLAG_CCx                TIM_FLAG_CC4
#define STEPMOTOR_TIMx_IRQn                   TIM1_CC_IRQn
#define STEPMOTOR_TIMx_IRQHandler             TIM1_CC_IRQHandler
#define  htimx_STEPMOTOR             htim1

#define STEPMOTOR_TIM_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()     // ���������������������
#define STEPMOTOR_TIM_PUL_PORT               BJ1_Motor1_PUL_GPIO_Port                            // ��Ӧ��������PUL-��������ʹ�ù����ӷ���
#define STEPMOTOR_TIM_PUL_PIN                 BJ1_Motor1_PUL_Pin                       // ��PLU+ֱ�ӽӿ������5V(����3.3V)

#define STEPMOTOR_DIR_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()     // �����ת������ƣ�������ղ���Ĭ����ת
#define STEPMOTOR_DIR_PORT                    DIR1_GPIO_Port                            // ��Ӧ��������DIR-��������ʹ�ù����ӷ���
#define STEPMOTOR_DIR_PIN                     DIR1_Pin                     // ��DIR+ֱ�ӽӿ������5V(����3.3V)
#define STEPMOTOR_DIR_FORWARD()               HAL_GPIO_WritePin(STEPMOTOR_DIR_PORT,STEPMOTOR_DIR_PIN,GPIO_PIN_SET)//1
#define STEPMOTOR_DIR_REVERSAL()              HAL_GPIO_WritePin(STEPMOTOR_DIR_PORT,STEPMOTOR_DIR_PIN,GPIO_PIN_RESET)

#define STEPMOTOR_ENA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()     // ����ѻ�ʹ�ܿ��ƣ�������ղ���Ĭ��ʹ�ܵ��
#define STEPMOTOR_ENA_PORT                    GPIOB                            // ��Ӧ��������ENA-��������ʹ�ù����ӷ���
#define STEPMOTOR_ENA_PIN                     GPIO_PIN_14                      // ��ENA+ֱ�ӽӿ������5V(����3.3V)
#define STEPMOTOR_OUTPUT_ENABLE()             HAL_GPIO_WritePin(STEPMOTOR_ENA_PORT,STEPMOTOR_ENA_PIN,GPIO_PIN_SET)
#define STEPMOTOR_OUTPUT_DISABLE()            HAL_GPIO_WritePin(STEPMOTOR_ENA_PORT,STEPMOTOR_ENA_PIN,GPIO_PIN_RESET)

// ���嶨ʱ��Ԥ��Ƶ����ʱ��ʵ��ʱ��Ƶ��Ϊ��72MHz/��STEPMOTOR_TIMx_PRESCALER+1��
//#define STEPMOTOR_TIM_PRESCALER               3  // �������������ϸ������Ϊ��   32  ϸ��
//#define STEPMOTOR_TIM_PRESCALER               7  // �������������ϸ������Ϊ��   16  ϸ��
//#define STEPMOTOR_TIM_PRESCALER               15  // �������������ϸ������Ϊ��   8  ϸ��
#define STEPMOTOR_TIM_PRESCALER               31  // �������������ϸ������Ϊ��   4  ϸ��
//#define STEPMOTOR_TIM_PRESCALER               63  // �������������ϸ������Ϊ��   2  ϸ��
//#define STEPMOTOR_TIM_PRESCALER               127  // �������������ϸ������Ϊ��   1  ϸ��


// ���嶨ʱ�����ڣ�����Ƚ�ģʽ��������Ϊ0xFFFF
#define STEPMOTOR_TIM_PERIOD                  0xFFFF


#define CW                                    0 // ˳ʱ��
#define CCW                                   1 // ��ʱ��

#define STOP                                  0 // �Ӽ�������״̬��ֹͣ
#define ACCEL                                 1 // �Ӽ�������״̬�����ٽ׶�
#define DECEL                                 2 // �Ӽ�������״̬�����ٽ׶�
#define RUN                                   3 // �Ӽ�������״̬�����ٽ׶�
#define T1_FREQ                               (SystemCoreClock/(STEPMOTOR_TIM_PRESCALER+1)) // Ƶ��ftֵ
#define FSPR                                  200         //���������Ȧ����
#define MICRO_STEP                            4          // �������������ϸ����
#define SPR                                   (FSPR*MICRO_STEP)   // ��תһȦ��Ҫ��������

// ��ѧ����
#define ALPHA                                 ((float)(2*3.14159/SPR))       // ��= 2*pi/spr
#define A_T_x10                               ((float)(10*ALPHA*T1_FREQ))
#define T1_FREQ_148                           ((float)((T1_FREQ*0.676)/10)) // 0.676Ϊ�������ֵ
#define A_SQ                                  ((float)(2*100000*ALPHA))
#define A_x200                                ((float)(200*ALPHA))

/* ��չ���� ------------------------------------------------------------------*/


extern TIM_HandleTypeDef htim1;
extern speedRampData srd;
extern TIM_HandleTypeDef htimx_STEPMOTOR;
extern __IO int32_t  step;
extern __IO uint8_t wait1;
extern __IO int32_t OLED_SPeed_count;
extern __IO uint8_t SP;

/* �������� ------------------------------------------------------------------*/
void Motor1_MX_TIM1_Init(void);
void STEPMOTOR_TIMx_Init(void);
void STEPMOTOR_AxisMoveRel(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed);

#endif	/* __STEPMOTOR_TIM_H__ */
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/