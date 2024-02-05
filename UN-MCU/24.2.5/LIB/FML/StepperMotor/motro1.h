#ifndef __STEPMOTOR1_TIM_H__
#define __STEPMOTOR1_TIM_H__

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
} speedRampData;


#define DIR1_Pin GPIO_PIN_11
#define DIR1_GPIO_Port GPIOC
#define BJ1_Motor1_PUL1_Pin GPIO_PIN_11
#define BJ1_Motor1_PUL1_GPIO_Port GPIOA
/* 宏定义 --------------------------------------------------------------------*/
#define STEPMOTOR_TIMx                        TIM1
#define STEPMOTOR_TIM_RCC_CLK_ENABLE()        __HAL_RCC_TIM1_CLK_ENABLE()
#define STEPMOTOR_TIM_RCC_CLK_DISABLE()       __HAL_RCC_TIM1_CLK_DISABLE()
#define STEPMOTOR_TIM_CHANNEL_x                TIM_CHANNEL_4
#define STEPMOTOR_TIM_IT_CCx                  TIM_IT_CC4
#define STEPMOTOR_TIM_FLAG_CCx                TIM_FLAG_CC4
#define STEPMOTOR_TIMx_IRQn                   TIM1_CC_IRQn
#define STEPMOTOR_TIMx_IRQHandler             TIM1_CC_IRQHandler
#define  htimx_STEPMOTOR             htim1

#define STEPMOTOR_TIM_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()     // 输出控制脉冲给电机驱动器
#define STEPMOTOR_TIM_PUL_PORT               BJ1_Motor1_PUL_GPIO_Port                            // 对应驱动器的PUL-（驱动器使用共阳接法）
#define STEPMOTOR_TIM_PUL_PIN                 BJ1_Motor1_PUL_Pin                       // 而PLU+直接接开发板的5V(或者3.3V)

#define STEPMOTOR_DIR_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()     // 电机旋转方向控制，如果悬空不接默认正转
#define STEPMOTOR_DIR_PORT                    DIR1_GPIO_Port                            // 对应驱动器的DIR-（驱动器使用共阳接法）
#define STEPMOTOR_DIR_PIN                     DIR1_Pin                     // 而DIR+直接接开发板的5V(或者3.3V)
#define STEPMOTOR_DIR_FORWARD()               HAL_GPIO_WritePin(STEPMOTOR_DIR_PORT,STEPMOTOR_DIR_PIN,GPIO_PIN_SET)//1
#define STEPMOTOR_DIR_REVERSAL()              HAL_GPIO_WritePin(STEPMOTOR_DIR_PORT,STEPMOTOR_DIR_PIN,GPIO_PIN_RESET)

#define STEPMOTOR_ENA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()     // 电机脱机使能控制，如果悬空不接默认使能电机
#define STEPMOTOR_ENA_PORT                    GPIOB                            // 对应驱动器的ENA-（驱动器使用共阳接法）
#define STEPMOTOR_ENA_PIN                     GPIO_PIN_14                      // 而ENA+直接接开发板的5V(或者3.3V)
#define STEPMOTOR_OUTPUT_ENABLE()             HAL_GPIO_WritePin(STEPMOTOR_ENA_PORT,STEPMOTOR_ENA_PIN,GPIO_PIN_SET)
#define STEPMOTOR_OUTPUT_DISABLE()            HAL_GPIO_WritePin(STEPMOTOR_ENA_PORT,STEPMOTOR_ENA_PIN,GPIO_PIN_RESET)

// 定义定时器预分频，定时器实际时钟频率为：72MHz/（STEPMOTOR_TIMx_PRESCALER+1）
//#define STEPMOTOR_TIM_PRESCALER               3  // 步进电机驱动器细分设置为：   32  细分
//#define STEPMOTOR_TIM_PRESCALER               7  // 步进电机驱动器细分设置为：   16  细分
//#define STEPMOTOR_TIM_PRESCALER               15  // 步进电机驱动器细分设置为：   8  细分
#define STEPMOTOR_TIM_PRESCALER               31  // 步进电机驱动器细分设置为：   4  细分
//#define STEPMOTOR_TIM_PRESCALER               63  // 步进电机驱动器细分设置为：   2  细分
//#define STEPMOTOR_TIM_PRESCALER               127  // 步进电机驱动器细分设置为：   1  细分


// 定义定时器周期，输出比较模式周期设置为0xFFFF
#define STEPMOTOR_TIM_PERIOD                  0xFFFF


#define CW                                    0 // 顺时针
#define CCW                                   1 // 逆时针

#define STOP                                  0 // 加减速曲线状态：停止
#define ACCEL                                 1 // 加减速曲线状态：加速阶段
#define DECEL                                 2 // 加减速曲线状态：减速阶段
#define RUN                                   3 // 加减速曲线状态：匀速阶段
#define T1_FREQ                               (SystemCoreClock/(STEPMOTOR_TIM_PRESCALER+1)) // 频率ft值
#define FSPR                                  200         //步进电机单圈步数
#define MICRO_STEP                            4          // 步进电机驱动器细分数
#define SPR                                   (FSPR*MICRO_STEP)   // 旋转一圈需要的脉冲数

// 数学常数
#define ALPHA                                 ((float)(2*3.14159/SPR))       // α= 2*pi/spr
#define A_T_x10                               ((float)(10*ALPHA*T1_FREQ))
#define T1_FREQ_148                           ((float)((T1_FREQ*0.676)/10)) // 0.676为误差修正值
#define A_SQ                                  ((float)(2*100000*ALPHA))
#define A_x200                                ((float)(200*ALPHA))

/* 扩展变量 ------------------------------------------------------------------*/


extern TIM_HandleTypeDef htim1;
extern speedRampData srd;
extern TIM_HandleTypeDef htimx_STEPMOTOR;
extern __IO int32_t  step;
extern __IO uint8_t wait1;
extern __IO int32_t OLED_SPeed_count;
extern __IO uint8_t SP;

/* 函数声明 ------------------------------------------------------------------*/
void Motor1_MX_TIM1_Init(void);
void STEPMOTOR_TIMx_Init(void);
void STEPMOTOR_AxisMoveRel(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed);

#endif	/* __STEPMOTOR_TIM_H__ */
/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
