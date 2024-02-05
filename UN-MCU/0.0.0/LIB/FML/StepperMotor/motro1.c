/***********************
  */
	/*利用定时器1 的比较输出模式的翻转转输出
	需要引入定时器的相关XXXtim.c XXXtim_ex.c文件引进来
	这里面需要初始化的是定时器 
	使用定时器 还得同时宏定义下这个 HAL_TIM_MODULE_ENABLED 因为定时器有好几个 无法共用*/
/* ---------------------------------------------------------------*/

#include "motro1.h"
#include <math.h>
#include "tim.h"


/**
  * 函数功能: 相对位置运动：运动给定的步数
  * 输入参数: step：移动的步数 (正数为顺时针，负数为逆时针).
              accel  加速度,实际值为accel*0.1*rad/sec^2
              decel  减速度,实际值为decel*0.1*rad/sec^2 弧度/每秒 平方
              speed  最大速度,实际值为speed*0.1*rad/sec
  * 返 回 值: 无
  * 说    明: 以给定的步数移动步进电机，先加速到最大速度，然后在合适位置开始
  *           减速至停止，使得整个运动距离为指定的步数。如果加减速阶段很短并且
  *           速度很慢，那还没达到最大速度就要开始减速
  */

speedRampData srd               = {STOP,CW,0,0,0,0,0};         // 加减速曲线变量
__IO int32_t  step_position     = 0;           // 当前位置
__IO uint8_t  MotionStatus      = 0;           //是否在运动？0：停止，1：运动
__IO uint8_t wait1;

__IO int32_t OLED_SPeed_count;
__IO uint8_t SP=0;
/* TIM1 init function */
TIM_HandleTypeDef htim1;

static void DIR_GPIOInit()
{
		 GPIO_InitTypeDef GPIO_InitStruct;
		    //正反转
    HAL_GPIO_WritePin(GPIOC, DIR1_Pin, GPIO_PIN_SET);
	    //正反转                     PCPin PCPin */
    GPIO_InitStruct.Pin = DIR1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DIR1_GPIO_Port, &GPIO_InitStruct);

}

static void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
{

    GPIO_InitTypeDef GPIO_InitStruct;
    if(timHandle->Instance==TIM1)
    {
        /* USER CODE BEGIN TIM1_MspPostInit 0 */
        /* USER CODE END TIM1_MspPostInit 0 */
        /**TIM1 GPIO Configuration
        PA11     ------> TIM1_CH4
        */
        GPIO_InitStruct.Pin = BJ1_Motor1_PUL1_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(BJ1_Motor1_PUL1_GPIO_Port, &GPIO_InitStruct);

        /* USER CODE BEGIN TIM1_MspPostInit 1 */
        /* USER CODE END TIM1_MspPostInit 1 */
    }


}

void Motor1_MX_TIM1_Init(void)  
{
	    /*步进电机1的定时器初始化*/
#ifndef StepperMotor1
未初始化步进电机1
#endif
    TIM_ClockConfigTypeDef sClockSourceConfig;
    TIM_MasterConfigTypeDef sMasterConfig;
    TIM_OC_InitTypeDef sConfigOC;
    TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;

    STEPMOTOR_TIM_RCC_CLK_ENABLE();//bj.h
    /*******基本初始化*******/
    htim1.Instance = TIM1;
    htim1.Init.Prescaler = 31;//预分频
    htim1.Init.CounterMode = TIM_COUNTERMODE_UP;//记数模式
    htim1.Init.Period = 0xFFFF;//周期
    htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;//时钟分频
    htim1.Init.RepetitionCounter = 0;//重复计数器
    htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    if (HAL_TIM_OC_Init(&htim1) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }
    /*输出比较初始化*/
    sConfigOC.OCMode = TIM_OCMODE_TOGGLE;
    sConfigOC.Pulse = 500;//
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    HAL_TIM_OC_ConfigChannel(&htimx_STEPMOTOR, &sConfigOC, TIM_CHANNEL_4);//////**********
    TIM_CCxChannelCmd(STEPMOTOR_TIMx, TIM_CHANNEL_4, TIM_CCx_DISABLE);//////*************
    /* 使能比较输出通道 */
    if (HAL_TIM_OC_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
    sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
    sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
    sBreakDeadTimeConfig.DeadTime = 0;
    sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
    sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
    sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
    if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }
    HAL_TIM_MspPostInit(&htim1);//对应引脚初始化

    HAL_TIM_Base_MspInit(&htim1);/*中断优先级并使能 */
    __HAL_TIM_CLEAR_FLAG(&htim1, TIM_FLAG_CC4);//*************清除中断标志位***************
    /* 使能定时器比较输出 */
    __HAL_TIM_ENABLE_IT(&htimx_STEPMOTOR, TIM_IT_CC4);//

    /* Enable the main output */
    __HAL_TIM_MOE_ENABLE(&htimx_STEPMOTOR);
    HAL_TIM_Base_Start(&htimx_STEPMOTOR);// 使能定时器

DIR_GPIOInit();
}

   


void STEPMOTOR_AxisMoveRel(__IO int32_t  step, __IO uint32_t accel, __IO uint32_t decel, __IO uint32_t speed)
{

    __IO uint16_t tim_count;
    // 达到最大速度时的步数
    __IO uint32_t max_s_lim;
    // 必须要开始减速的步数（如果加速没有达到最大速度）
    __IO uint32_t accel_lim;
    wait1=0;
    SP=0;
    if(step < 0) // 步数为负数
    {
        srd.dir = CCW; // 逆时针方向旋转
        STEPMOTOR_DIR_REVERSAL();
        HAL_GPIO_WritePin(STEPMOTOR_DIR_PORT,STEPMOTOR_DIR_PIN,GPIO_PIN_RESET);
        step =-step;   // 获取步数绝对值
        SP=1;//用来负数标志位方便用来在OLED上显示绝对步数
    }
    else
    {
        srd.dir = CW; // 顺时针方向旋转
        HAL_GPIO_WritePin(STEPMOTOR_DIR_PORT,STEPMOTOR_DIR_PIN,GPIO_PIN_SET);//1
        STEPMOTOR_DIR_FORWARD();
        SP=2;
    }

    if(step == 1)    // 步数为1
    {
        srd.accel_count = -1;   // 只移动一步
        srd.run_state = DECEL;  // 减速状态.
        srd.step_delay = 1000;	// 短延时

    }
    else if(step != 0)  // 如果目标运动步数不为0
    {

        // 我们的驱动器用户手册有详细的计算及推导过程

        // 设置最大速度极限, 计算得到min_delay用于定时器的计数器的值。
        // min_delay = (alpha / tt)/ w
        srd.min_delay = (int32_t)(A_T_x10/speed);

        // 通过计算第一个(c0) 的步进延时来设定加速度，其中accel单位为0.1rad/sec^2
        // step_delay = 1/tt * sqrt(2*alpha/accel)
        // step_delay = ( tfreq*0.676/10 )*10 * sqrt( (2*alpha*100000) / (accel*10) )/100
        srd.step_delay = (int32_t)((T1_FREQ_148 * sqrt(A_SQ / accel))/10);

        // 计算多少步之后达到最大速度的限制
        // max_s_lim = speed^2 / (2*alpha*accel)
        max_s_lim = (uint32_t)(speed*speed/(A_x200*accel/10));
        // 如果达到最大速度小于0.5步，我们将四舍五入为0
        // 但实际我们必须移动至少一步才能达到想要的速度
        if(max_s_lim == 0)
        {
            max_s_lim = 1;
        }

        // 计算多少步之后我们必须开始减速
        // n1 = (n1+n2)decel / (accel + decel)
        accel_lim = (uint32_t)(step*decel/(accel+decel));
        // 我们必须加速至少1步才能才能开始减速.
        if(accel_lim == 0)
        {
            accel_lim = 1;
        }

        // 使用限制条件我们可以计算出减速阶段步数
        if(accel_lim <= max_s_lim)
        {
            srd.decel_val = accel_lim - step;
        }
        else
        {
            srd.decel_val = -(max_s_lim*accel/decel);
        }
        // 当只剩下一步我们必须减速
        if(srd.decel_val == 0)
        {
            srd.decel_val = -1;
        }

        // 计算开始减速时的步数
        srd.decel_start = step + srd.decel_val;

        // 如果最大速度很慢，我们就不需要进行加速运动
        if(srd.step_delay <= srd.min_delay)
        {
            srd.step_delay = srd.min_delay;
            srd.run_state = RUN;
        }
        else
        {
            srd.run_state = ACCEL;
        }
        // 复位加速度计数值
        srd.accel_count = 0;
    }
    MotionStatus = 1; // 电机为运动状态
    tim_count=__HAL_TIM_GET_COUNTER(&htimx_STEPMOTOR);
    __HAL_TIM_SET_COMPARE(&htimx_STEPMOTOR,STEPMOTOR_TIM_CHANNEL_x,tim_count+srd.step_delay); // 设置定时器比较值
    TIM_CCxChannelCmd(STEPMOTOR_TIMx, STEPMOTOR_TIM_CHANNEL_x, TIM_CCx_ENABLE);// 使能定时器通道
    STEPMOTOR_OUTPUT_ENABLE();


}

/**
  * 函数功能: 定时器中断服务函数
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 实现加减速过程
  */
void STEPMOTOR_TIMx_IRQHandler(void)//定时器中断处理
{

    __IO uint16_t tim_count=0;
    // 保存新（下）一个延时周期
    uint16_t new_step_delay=0;
    // 加速过程中最后一次延时（脉冲周期）.
    __IO static uint16_t last_accel_delay=0;
    // 总移动步数计数器
    __IO static uint32_t step_count = 0;
    // 记录new_step_delay中的余数，提高下一步计算的精度
    __IO static int32_t rest = 0;
    //定时器使用翻转模式，需要进入两次中断才输出一个完整脉冲
    __IO static uint8_t i=0;



    OLED_SPeed_count=step_count;



    if(__HAL_TIM_GET_IT_SOURCE(&htimx_STEPMOTOR, STEPMOTOR_TIM_IT_CCx) !=RESET)
    {
        // 清楚定时器中断
        __HAL_TIM_CLEAR_IT(&htimx_STEPMOTOR, STEPMOTOR_TIM_IT_CCx);

        // 设置比较值
        tim_count=__HAL_TIM_GET_COUNTER(&htimx_STEPMOTOR);
        __HAL_TIM_SET_COMPARE(&htimx_STEPMOTOR,STEPMOTOR_TIM_CHANNEL_x,tim_count+srd.step_delay);

        i++;     // 定时器中断次数计数值
        if(i==2) // 2次，说明已经输出一个完整脉冲
        {
            i=0;   // 清零定时器中断次数计数值
            switch(srd.run_state) // 加减速曲线阶段
            {
            case STOP:
                step_count = 0;  // 清零步数计数器
                rest = 0;        // 清零余值
                // 关闭通道
                TIM_CCxChannelCmd(STEPMOTOR_TIMx, STEPMOTOR_TIM_CHANNEL_x, TIM_CCx_DISABLE);
                __HAL_TIM_CLEAR_FLAG(&htimx_STEPMOTOR, STEPMOTOR_TIM_FLAG_CCx);
                STEPMOTOR_OUTPUT_DISABLE();
                MotionStatus = 0;  //  电机为停止状态
                break;

            case ACCEL:
                step_count++;      // 步数加1
                if(srd.dir==CW)
                {
                    step_position++; // 绝对位置加1
                }
                else
                {
                    step_position--; // 绝对位置减1
                }
                srd.accel_count++; // 加速计数值加1
                new_step_delay = srd.step_delay - (((2 *srd.step_delay) + rest)/(4 * srd.accel_count + 1));//计算新(下)一步脉冲周期(时间间隔)
                rest = ((2 * srd.step_delay)+rest)%(4 * srd.accel_count + 1);// 计算余数，下次计算补上余数，减少误差
                if(step_count >= srd.decel_start)// 检查是够应该开始减速
                {
                    srd.accel_count = srd.decel_val; // 加速计数值为减速阶段计数值的初始值
                    srd.run_state = DECEL;           // 下个脉冲进入减速阶段
                }
                else if(new_step_delay <= srd.min_delay) // 检查是否到达期望的最大速度
                {
                    last_accel_delay = new_step_delay; // 保存加速过程中最后一次延时（脉冲周期）
                    new_step_delay = srd.min_delay;    // 使用min_delay（对应最大速度speed）
                    rest = 0;                          // 清零余值
                    srd.run_state = RUN;               // 设置为匀速运行状态
                }
                break;

            case RUN:
                step_count++;  // 步数加1
                if(srd.dir==CW)
                {
                    step_position++; // 绝对位置加1
                }
                else
                {
                    step_position--; // 绝对位置减1
                }
                new_step_delay = srd.min_delay;     // 使用min_delay（对应最大速度speed）
                if(step_count >= srd.decel_start)   // 需要开始减速
                {
                    srd.accel_count = srd.decel_val;  // 减速步数做为加速计数值
                    new_step_delay = last_accel_delay;// 加阶段最后的延时做为减速阶段的起始延时(脉冲周期)
                    srd.run_state = DECEL;            // 状态改变为减速
                }
                break;

            case DECEL:
                step_count++;  // 步数加1
                if(srd.dir==CW)
                {
                    step_position++; // 绝对位置加1
                }
                else
                {
                    step_position--; // 绝对位置减1
                }
                srd.accel_count++;
                new_step_delay = srd.step_delay - (((2 * srd.step_delay) + rest)/(4 * srd.accel_count + 1)); //计算新(下)一步脉冲周期(时间间隔)
                rest = ((2 * srd.step_delay)+rest)%(4 * srd.accel_count + 1);// 计算余数，下次计算补上余数，减少误差

                //检查是否为最后一步
                if(srd.accel_count >= 0)
                {
                    srd.run_state = STOP;
                    wait1=1;
                }
                break;
            }
            srd.step_delay = new_step_delay; // 为下个(新的)延时(脉冲周期)赋值
        }
    }
}


