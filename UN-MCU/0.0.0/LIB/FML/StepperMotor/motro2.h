#ifndef __STEPMOTOR2_TIM_H__
#define __STEPMOTOR2_TIM_H__

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
} speedRampData2;


#define DIR2_Pin GPIO_PIN_13
#define DIR2_GPIO_Port GPIOC
#define BJ2_Motor2_PUL2_Pin GPIO_PIN_3
#define BJ2_Motor2_PUL2_GPIO_Port GPIOA
/* �궨�� --------------------------------------------------------------------*/
#define STEPMOTOR_TIMx2                        TIM2
#define STEPMOTOR_TIM_RCC_CLK_ENABLE2()        __HAL_RCC_TIM2_CLK_ENABLE()
#define STEPMOTOR_TIM_RCC_CLK_DISABLE2()       __HAL_RCC_TIM2_CLK_DISABLE()
#define STEPMOTOR_TIM_CHANNEL_x2              TIM_CHANNEL_4
#define STEPMOTOR_TIM_IT_CCx2                  TIM_IT_CC4
#define STEPMOTOR_TIM_FLAG_CCx2                TIM_FLAG_CC4
#define STEPMOTOR_TIMx_IRQn2                   TIM2_IRQn
#define STEPMOTOR_TIMx_IRQHandler2             TIM2_IRQHandler
#define  htimx_STEPMOTOR2             htim2

#define STEPMOTOR_TIM_GPIO_CLK_ENABLE2()       __HAL_RCC_GPIOA_CLK_ENABLE()     // ���������������������
#define STEPMOTOR_TIM_PUL_PORT2               BJ2_Motor2_PUL2_GPIO_Port                            // ��Ӧ��������PUL-��������ʹ�ù����ӷ���
#define STEPMOTOR_TIM_PUL_PIN2                 BJ2_Motor2_PUL2_Pin                      // ��PLU+ֱ�ӽӿ������5V(����3.3V)

#define STEPMOTOR_DIR_GPIO_CLK_ENABLE2()       __HAL_RCC_GPIOB_CLK_ENABLE()     // �����ת������ƣ�������ղ���Ĭ����ת
#define STEPMOTOR_DIR_PORT2                    DIR2_GPIO_Port                            // ��Ӧ��������DIR-��������ʹ�ù����ӷ���
#define STEPMOTOR_DIR_PIN2                     DIR2_Pin                     // ��DIR+ֱ�ӽӿ������5V(����3.3V)
#define STEPMOTOR_DIR_FORWARD2()               HAL_GPIO_WritePin(STEPMOTOR_DIR_PORT2,STEPMOTOR_DIR_PIN2,GPIO_PIN_SET)//1
#define STEPMOTOR_DIR_REVERSAL2()              HAL_GPIO_WritePin(STEPMOTOR_DIR_PORT2,STEPMOTOR_DIR_PIN2,GPIO_PIN_RESET)

#define STEPMOTOR_ENA_GPIO_CLK_ENABLE2()       __HAL_RCC_GPIOB_CLK_ENABLE()     // ����ѻ�ʹ�ܿ��ƣ�������ղ���Ĭ��ʹ�ܵ��
#define STEPMOTOR_ENA_PORT2                    GPIOB                            // ��Ӧ��������ENA-��������ʹ�ù����ӷ���
#define STEPMOTOR_ENA_PIN2                     GPIO_PIN_14                      // ��ENA+ֱ�ӽӿ������5V(����3.3V)
#define STEPMOTOR_OUTPUT_ENABLE2()             HAL_GPIO_WritePin(STEPMOTOR_ENA_PORT2,STEPMOTOR_ENA_PIN2,GPIO_PIN_SET)
#define STEPMOTOR_OUTPUT_DISABLE2()            HAL_GPIO_WritePin(STEPMOTOR_ENA_PORT2,STEPMOTOR_ENA_PIN2,GPIO_PIN_RESET)

// ���嶨ʱ��Ԥ��Ƶ����ʱ��ʵ��ʱ��Ƶ��Ϊ��72MHz/��STEPMOTOR_TIMx_PRESCALER+1��
//#define STEPMOTOR_TIM_PRESCALER               3  // �������������ϸ������Ϊ��   32  ϸ��
//#define STEPMOTOR_TIM_PRESCALER               7  // �������������ϸ������Ϊ��   16  ϸ��
//#define STEPMOTOR_TIM_PRESCALER               15  // �������������ϸ������Ϊ��   8  ϸ��
#define STEPMOTOR_TIM_PRESCALER2               31  // �������������ϸ������Ϊ��   4  ϸ��
//#define STEPMOTOR_TIM_PRESCALER               63  // �������������ϸ������Ϊ��   2  ϸ��
//#define STEPMOTOR_TIM_PRESCALER               127  // �������������ϸ������Ϊ��   1  ϸ��


// ���嶨ʱ�����ڣ�����Ƚ�ģʽ��������Ϊ0xFFFF
#define STEPMOTOR_TIM_PERIOD2                 0xFFFF

#define FALSE2                                 0
#define TRUE2                                  1
#define CW2                                    0 // ˳ʱ��
#define CCW2                                   1 // ��ʱ��

#define STOP2                                  0 // �Ӽ�������״̬��ֹͣ
#define ACCEL2                                 1 // �Ӽ�������״̬�����ٽ׶�
#define DECEL2                                 2 // �Ӽ�������״̬�����ٽ׶�
#define RUN2                                   3 // �Ӽ�������״̬�����ٽ׶�
#define T1_FREQ2                              (SystemCoreClock/(STEPMOTOR_TIM_PRESCALER2+1)) // Ƶ��ftֵ
#define FSPR2                                  200         //���������Ȧ����
#define MICRO_STEP2                            4          // �������������ϸ����
#define SPR2                                   (FSPR2*MICRO_STEP2)   // ��תһȦ��Ҫ��������

// ��ѧ����
#define ALPHA2                                 ((float)(2*3.14159/SPR2))       // ��= 2*pi/spr
#define A_T_x102                               ((float)(10*ALPHA2*T1_FREQ2))
#define T1_FREQ_1482                           ((float)((T1_FREQ2*0.676)/10)) // 0.676Ϊ�������ֵ
#define A_SQ2                                  ((float)(2*100000*ALPHA2))
#define A_x2002                                ((float)(200*ALPHA2))

/* ��չ���� ------------------------------------------------------------------*/
extern TIM_HandleTypeDef htimx_STEPMOTOR2;

extern __IO uint8_t wait2;
extern __IO int32_t OLED_SPeed_count2;
extern __IO uint8_t SP2;

extern TIM_HandleTypeDef htim2;

/* �������� ------------------------------------------------------------------*/

void STEPMOTOR_AxisMoveRel2(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed);
void Motor2_MX_TIM2_Init(void);
#endif	/* __STEPMOTOR_TIM_H__ */
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
