#ifndef __STEPMOTOR_3TIM_H__
#define __STEPMOTOR_3TIM_H__

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
} speedRampData3;
#define DIR3_Pin GPIO_PIN_15
#define DIR3_GPIO_Port GPIOC
#define BJ3_Motor3_PUL3_Pin GPIO_PIN_6
#define BJ3_Motor3_PUL3_GPIO_Port GPIOA
/* �궨�� --------------------------------------------------------------------*/
#define STEPMOTOR_TIMx3                       TIM3
#define STEPMOTOR_TIM_RCC_CLK_ENABLE3()        __HAL_RCC_TIM3_CLK_ENABLE()
#define STEPMOTOR_TIM_RCC_CLK_DISABLE3()       __HAL_RCC_TIM3_CLK_DISABLE()
#define STEPMOTOR_TIM_CHANNEL_x3                TIM_CHANNEL_1
#define STEPMOTOR_TIM_IT_CCx3                  TIM_IT_CC1
#define STEPMOTOR_TIM_FLAG_CCx3                TIM_FLAG_CC1
#define STEPMOTOR_TIMx_IRQn3                   TIM3_IRQn
#define STEPMOTOR_TIMx_IRQHandler3             TIM3_IRQHandler
#define  htimx_STEPMOTOR3             htim3

#define STEPMOTOR_TIM_GPIO_CLK_ENABLE3()       __HAL_RCC_GPIOA_CLK_ENABLE()     // ���������������������
#define STEPMOTOR_TIM_PUL_PORT3               BJ3_Motor3_PUL3_GPIO_Port                            // ��Ӧ��������PUL-��������ʹ�ù����ӷ���
#define STEPMOTOR_TIM_PUL_PIN3                 BJ3_Motor3_PUL3_Pin                       // ��PLU+ֱ�ӽӿ������5V(����3.3V)

//#define STEPMOTOR_DIR_GPIO_CLK_ENABLE3()       __HAL_RCC_GPIOB_CLK_ENABLE()     // �����ת������ƣ�������ղ���Ĭ����ת
#define STEPMOTOR_DIR_PORT3                    DIR3_GPIO_Port                            // ��Ӧ��������DIR-��������ʹ�ù����ӷ���
#define STEPMOTOR_DIR_PIN3                     DIR3_Pin                     // ��DIR+ֱ�ӽӿ������5V(����3.3V)
#define STEPMOTOR_DIR_FORWARD3()               HAL_GPIO_WritePin(STEPMOTOR_DIR_PORT3,STEPMOTOR_DIR_PIN3,GPIO_PIN_SET)//1
#define STEPMOTOR_DIR_REVERSAL3()              HAL_GPIO_WritePin(STEPMOTOR_DIR_PORT3,STEPMOTOR_DIR_PIN3,GPIO_PIN_RESET)

#define STEPMOTOR_ENA_GPIO_CLK_ENABLE3()       __HAL_RCC_GPIOB_CLK_ENABLE()     // ����ѻ�ʹ�ܿ��ƣ�������ղ���Ĭ��ʹ�ܵ��
#define STEPMOTOR_ENA_PORT3                    GPIOB                            // ��Ӧ��������ENA-��������ʹ�ù����ӷ���
#define STEPMOTOR_ENA_PIN3                    GPIO_PIN_14                      // ��ENA+ֱ�ӽӿ������5V(����3.3V)
#define STEPMOTOR_OUTPUT_ENABLE3()             HAL_GPIO_WritePin(STEPMOTOR_ENA_PORT3,STEPMOTOR_ENA_PIN3,GPIO_PIN_SET)
#define STEPMOTOR_OUTPUT_DISABLE3()            HAL_GPIO_WritePin(STEPMOTOR_ENA_PORT3,STEPMOTOR_ENA_PIN3,GPIO_PIN_RESET)

// ���嶨ʱ��Ԥ��Ƶ����ʱ��ʵ��ʱ��Ƶ��Ϊ��72MHz/��STEPMOTOR_TIMx_PRESCALER+1��
//#define STEPMOTOR_TIM_PRESCALER               3  // �������������ϸ������Ϊ��   32  ϸ��
//#define STEPMOTOR_TIM_PRESCALER               7  // �������������ϸ������Ϊ��   16  ϸ��
//#define STEPMOTOR_TIM_PRESCALER               15  // �������������ϸ������Ϊ��   8  ϸ��
#define STEPMOTOR_TIM_PRESCALER3               31  // �������������ϸ������Ϊ��   4  ϸ��
//#define STEPMOTOR_TIM_PRESCALER               63  // �������������ϸ������Ϊ��   2  ϸ��
//#define STEPMOTOR_TIM_PRESCALER               127  // �������������ϸ������Ϊ��   1  ϸ��


// ���嶨ʱ�����ڣ�����Ƚ�ģʽ��������Ϊ0xFFFF
#define STEPMOTOR_TIM_PERIOD3                  0xFFFF

#define FALSE3                                 0
#define TRUE3                                  1
#define CW3                                    0 // ˳ʱ��
#define CCW3                                   1 // ��ʱ��

#define STOP3                                  0 // �Ӽ�������״̬��ֹͣ
#define ACCEL3                                 1 // �Ӽ�������״̬�����ٽ׶�
#define DECEL3                                 2 // �Ӽ�������״̬�����ٽ׶�
#define RUN3                                   3 // �Ӽ�������״̬�����ٽ׶�
#define T1_FREQ3                              (SystemCoreClock/(STEPMOTOR_TIM_PRESCALER3+1)) // Ƶ��ftֵ
#define FSPR3                                 200         //���������Ȧ����
#define MICRO_STEP3                            4          // �������������ϸ����
#define SPR3                                   (FSPR3*MICRO_STEP3)   // ��תһȦ��Ҫ��������

// ��ѧ����
#define ALPHA3                                 ((float)(2*3.14159/SPR3))       // ��= 2*pi/spr
#define A_T_x103                               ((float)(10*ALPHA3*T1_FREQ3))
#define T1_FREQ_1483                           ((float)((T1_FREQ3*0.676)/10)) // 0.676Ϊ�������ֵ
#define A_SQ3                                  ((float)(2*100000*ALPHA3))
#define A_x2003                                ((float)(200*ALPHA3))

/* ��չ���� ------------------------------------------------------------------*/
extern TIM_HandleTypeDef htimx_STEPMOTOR3;

extern __IO uint8_t wait3;
extern __IO int32_t OLED_SPeed_count3;
extern __IO uint8_t SP3;
extern TIM_HandleTypeDef htim3;

/* �������� ------------------------------------------------------------------*/
void Motor3_MX_TIM3_Init(void);
void STEPMOTOR_AxisMoveRel3(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed);

#endif	/* __STEPMOTOR_TIM_H__ */
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
