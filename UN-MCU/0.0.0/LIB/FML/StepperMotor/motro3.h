#ifndef __STEPMOTOR_3TIM_H__
#define __STEPMOTOR_3TIM_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "JHAL.h"

/* 类型定义 ------------------------------------------------------------------*/
typedef struct
{
    __IO uint8_t  run_state ;  // 电机旋转状态
    __IO uint8_t  dir ;        // 电机旋转方向
    __IO int32_t  step_delay;  // 下个脉冲周期（时间间隔），启动时为加速度
    __IO uint32_t decel_start; // 启动减速位置
    __IO int32_t  decel_val;   // 减速阶段步数
    __IO int32_t  min_delay;   // 最小脉冲周期(最大速度，即匀速段速度)
    __IO int32_t  accel_count; // 加减速阶段计数值
} speedRampData3;
#define DIR3_Pin GPIO_PIN_15
#define DIR3_GPIO_Port GPIOC
#define BJ3_Motor3_PUL3_Pin GPIO_PIN_6
#define BJ3_Motor3_PUL3_GPIO_Port GPIOA
/* 宏定义 --------------------------------------------------------------------*/
#define STEPMOTOR_TIMx3                       TIM3
#define STEPMOTOR_TIM_RCC_CLK_ENABLE3()        __HAL_RCC_TIM3_CLK_ENABLE()
#define STEPMOTOR_TIM_RCC_CLK_DISABLE3()       __HAL_RCC_TIM3_CLK_DISABLE()
#define STEPMOTOR_TIM_CHANNEL_x3                TIM_CHANNEL_1
#define STEPMOTOR_TIM_IT_CCx3                  TIM_IT_CC1
#define STEPMOTOR_TIM_FLAG_CCx3                TIM_FLAG_CC1
#define STEPMOTOR_TIMx_IRQn3                   TIM3_IRQn
#define STEPMOTOR_TIMx_IRQHandler3             TIM3_IRQHandler
#define  htimx_STEPMOTOR3             htim3

#define STEPMOTOR_TIM_GPIO_CLK_ENABLE3()       __HAL_RCC_GPIOA_CLK_ENABLE()     // 输出控制脉冲给电机驱动器
#define STEPMOTOR_TIM_PUL_PORT3               BJ3_Motor3_PUL3_GPIO_Port                            // 对应驱动器的PUL-（驱动器使用共阳接法）
#define STEPMOTOR_TIM_PUL_PIN3                 BJ3_Motor3_PUL3_Pin                       // 而PLU+直接接开发板的5V(或者3.3V)

//#define STEPMOTOR_DIR_GPIO_CLK_ENABLE3()       __HAL_RCC_GPIOB_CLK_ENABLE()     // 电机旋转方向控制，如果悬空不接默认正转
#define STEPMOTOR_DIR_PORT3                    DIR3_GPIO_Port                            // 对应驱动器的DIR-（驱动器使用共阳接法）
#define STEPMOTOR_DIR_PIN3                     DIR3_Pin                     // 而DIR+直接接开发板的5V(或者3.3V)
#define STEPMOTOR_DIR_FORWARD3()               HAL_GPIO_WritePin(STEPMOTOR_DIR_PORT3,STEPMOTOR_DIR_PIN3,GPIO_PIN_SET)//1
#define STEPMOTOR_DIR_REVERSAL3()              HAL_GPIO_WritePin(STEPMOTOR_DIR_PORT3,STEPMOTOR_DIR_PIN3,GPIO_PIN_RESET)

#define STEPMOTOR_ENA_GPIO_CLK_ENABLE3()       __HAL_RCC_GPIOB_CLK_ENABLE()     // 电机脱机使能控制，如果悬空不接默认使能电机
#define STEPMOTOR_ENA_PORT3                    GPIOB                            // 对应驱动器的ENA-（驱动器使用共阳接法）
#define STEPMOTOR_ENA_PIN3                    GPIO_PIN_14                      // 而ENA+直接接开发板的5V(或者3.3V)
#define STEPMOTOR_OUTPUT_ENABLE3()             HAL_GPIO_WritePin(STEPMOTOR_ENA_PORT3,STEPMOTOR_ENA_PIN3,GPIO_PIN_SET)
#define STEPMOTOR_OUTPUT_DISABLE3()            HAL_GPIO_WritePin(STEPMOTOR_ENA_PORT3,STEPMOTOR_ENA_PIN3,GPIO_PIN_RESET)

// 定义定时器预分频，定时器实际时钟频率为：72MHz/（STEPMOTOR_TIMx_PRESCALER+1）
//#define STEPMOTOR_TIM_PRESCALER               3  // 步进电机驱动器细分设置为：   32  细分
//#define STEPMOTOR_TIM_PRESCALER               7  // 步进电机驱动器细分设置为：   16  细分
//#define STEPMOTOR_TIM_PRESCALER               15  // 步进电机驱动器细分设置为：   8  细分
#define STEPMOTOR_TIM_PRESCALER3               31  // 步进电机驱动器细分设置为：   4  细分
//#define STEPMOTOR_TIM_PRESCALER               63  // 步进电机驱动器细分设置为：   2  细分
//#define STEPMOTOR_TIM_PRESCALER               127  // 步进电机驱动器细分设置为：   1  细分


// 定义定时器周期，输出比较模式周期设置为0xFFFF
#define STEPMOTOR_TIM_PERIOD3                  0xFFFF

#define FALSE3                                 0
#define TRUE3                                  1
#define CW3                                    0 // 顺时针
#define CCW3                                   1 // 逆时针

#define STOP3                                  0 // 加减速曲线状态：停止
#define ACCEL3                                 1 // 加减速曲线状态：加速阶段
#define DECEL3                                 2 // 加减速曲线状态：减速阶段
#define RUN3                                   3 // 加减速曲线状态：匀速阶段
#define T1_FREQ3                              (SystemCoreClock/(STEPMOTOR_TIM_PRESCALER3+1)) // 频率ft值
#define FSPR3                                 200         //步进电机单圈步数
#define MICRO_STEP3                            4          // 步进电机驱动器细分数
#define SPR3                                   (FSPR3*MICRO_STEP3)   // 旋转一圈需要的脉冲数

// 数学常数
#define ALPHA3                                 ((float)(2*3.14159/SPR3))       // α= 2*pi/spr
#define A_T_x103                               ((float)(10*ALPHA3*T1_FREQ3))
#define T1_FREQ_1483                           ((float)((T1_FREQ3*0.676)/10)) // 0.676为误差修正值
#define A_SQ3                                  ((float)(2*100000*ALPHA3))
#define A_x2003                                ((float)(200*ALPHA3))

/* 扩展变量 ------------------------------------------------------------------*/
extern TIM_HandleTypeDef htimx_STEPMOTOR3;

extern __IO uint8_t wait3;
extern __IO int32_t OLED_SPeed_count3;
extern __IO uint8_t SP3;
extern TIM_HandleTypeDef htim3;

/* 函数声明 ------------------------------------------------------------------*/
void Motor3_MX_TIM3_Init(void);
void STEPMOTOR_AxisMoveRel3(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed);

#endif	/* __STEPMOTOR_TIM_H__ */
/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
