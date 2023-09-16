#ifndef __STEPMOTOR2_TIM_H__
#define __STEPMOTOR2_TIM_H__

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
} speedRampData2;


#define DIR2_Pin GPIO_PIN_13
#define DIR2_GPIO_Port GPIOC
#define BJ2_Motor2_PUL2_Pin GPIO_PIN_3
#define BJ2_Motor2_PUL2_GPIO_Port GPIOA
/* 宏定义 --------------------------------------------------------------------*/
#define STEPMOTOR_TIMx2                        TIM2
#define STEPMOTOR_TIM_RCC_CLK_ENABLE2()        __HAL_RCC_TIM2_CLK_ENABLE()
#define STEPMOTOR_TIM_RCC_CLK_DISABLE2()       __HAL_RCC_TIM2_CLK_DISABLE()
#define STEPMOTOR_TIM_CHANNEL_x2              TIM_CHANNEL_4
#define STEPMOTOR_TIM_IT_CCx2                  TIM_IT_CC4
#define STEPMOTOR_TIM_FLAG_CCx2                TIM_FLAG_CC4
#define STEPMOTOR_TIMx_IRQn2                   TIM2_IRQn
#define STEPMOTOR_TIMx_IRQHandler2             TIM2_IRQHandler
#define  htimx_STEPMOTOR2             htim2

#define STEPMOTOR_TIM_GPIO_CLK_ENABLE2()       __HAL_RCC_GPIOA_CLK_ENABLE()     // 输出控制脉冲给电机驱动器
#define STEPMOTOR_TIM_PUL_PORT2               BJ2_Motor2_PUL2_GPIO_Port                            // 对应驱动器的PUL-（驱动器使用共阳接法）
#define STEPMOTOR_TIM_PUL_PIN2                 BJ2_Motor2_PUL2_Pin                      // 而PLU+直接接开发板的5V(或者3.3V)

#define STEPMOTOR_DIR_GPIO_CLK_ENABLE2()       __HAL_RCC_GPIOB_CLK_ENABLE()     // 电机旋转方向控制，如果悬空不接默认正转
#define STEPMOTOR_DIR_PORT2                    DIR2_GPIO_Port                            // 对应驱动器的DIR-（驱动器使用共阳接法）
#define STEPMOTOR_DIR_PIN2                     DIR2_Pin                     // 而DIR+直接接开发板的5V(或者3.3V)
#define STEPMOTOR_DIR_FORWARD2()               HAL_GPIO_WritePin(STEPMOTOR_DIR_PORT2,STEPMOTOR_DIR_PIN2,GPIO_PIN_SET)//1
#define STEPMOTOR_DIR_REVERSAL2()              HAL_GPIO_WritePin(STEPMOTOR_DIR_PORT2,STEPMOTOR_DIR_PIN2,GPIO_PIN_RESET)

#define STEPMOTOR_ENA_GPIO_CLK_ENABLE2()       __HAL_RCC_GPIOB_CLK_ENABLE()     // 电机脱机使能控制，如果悬空不接默认使能电机
#define STEPMOTOR_ENA_PORT2                    GPIOB                            // 对应驱动器的ENA-（驱动器使用共阳接法）
#define STEPMOTOR_ENA_PIN2                     GPIO_PIN_14                      // 而ENA+直接接开发板的5V(或者3.3V)
#define STEPMOTOR_OUTPUT_ENABLE2()             HAL_GPIO_WritePin(STEPMOTOR_ENA_PORT2,STEPMOTOR_ENA_PIN2,GPIO_PIN_SET)
#define STEPMOTOR_OUTPUT_DISABLE2()            HAL_GPIO_WritePin(STEPMOTOR_ENA_PORT2,STEPMOTOR_ENA_PIN2,GPIO_PIN_RESET)

// 定义定时器预分频，定时器实际时钟频率为：72MHz/（STEPMOTOR_TIMx_PRESCALER+1）
//#define STEPMOTOR_TIM_PRESCALER               3  // 步进电机驱动器细分设置为：   32  细分
//#define STEPMOTOR_TIM_PRESCALER               7  // 步进电机驱动器细分设置为：   16  细分
//#define STEPMOTOR_TIM_PRESCALER               15  // 步进电机驱动器细分设置为：   8  细分
#define STEPMOTOR_TIM_PRESCALER2               31  // 步进电机驱动器细分设置为：   4  细分
//#define STEPMOTOR_TIM_PRESCALER               63  // 步进电机驱动器细分设置为：   2  细分
//#define STEPMOTOR_TIM_PRESCALER               127  // 步进电机驱动器细分设置为：   1  细分


// 定义定时器周期，输出比较模式周期设置为0xFFFF
#define STEPMOTOR_TIM_PERIOD2                 0xFFFF

#define FALSE2                                 0
#define TRUE2                                  1
#define CW2                                    0 // 顺时针
#define CCW2                                   1 // 逆时针

#define STOP2                                  0 // 加减速曲线状态：停止
#define ACCEL2                                 1 // 加减速曲线状态：加速阶段
#define DECEL2                                 2 // 加减速曲线状态：减速阶段
#define RUN2                                   3 // 加减速曲线状态：匀速阶段
#define T1_FREQ2                              (SystemCoreClock/(STEPMOTOR_TIM_PRESCALER2+1)) // 频率ft值
#define FSPR2                                  200         //步进电机单圈步数
#define MICRO_STEP2                            4          // 步进电机驱动器细分数
#define SPR2                                   (FSPR2*MICRO_STEP2)   // 旋转一圈需要的脉冲数

// 数学常数
#define ALPHA2                                 ((float)(2*3.14159/SPR2))       // α= 2*pi/spr
#define A_T_x102                               ((float)(10*ALPHA2*T1_FREQ2))
#define T1_FREQ_1482                           ((float)((T1_FREQ2*0.676)/10)) // 0.676为误差修正值
#define A_SQ2                                  ((float)(2*100000*ALPHA2))
#define A_x2002                                ((float)(200*ALPHA2))

/* 扩展变量 ------------------------------------------------------------------*/
extern TIM_HandleTypeDef htimx_STEPMOTOR2;

extern __IO uint8_t wait2;
extern __IO int32_t OLED_SPeed_count2;
extern __IO uint8_t SP2;

extern TIM_HandleTypeDef htim2;

/* 函数声明 ------------------------------------------------------------------*/

void STEPMOTOR_AxisMoveRel2(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed);
void Motor2_MX_TIM2_Init(void);
#endif	/* __STEPMOTOR_TIM_H__ */
/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
