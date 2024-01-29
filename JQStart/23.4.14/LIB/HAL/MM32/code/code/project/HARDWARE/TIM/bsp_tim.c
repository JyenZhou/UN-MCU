#include "bsp_tim.h"
/*
定时器文件说明：
1、Tim1_Init为定时器1初始化，定时器1用来输出PWM方波来模拟DAC
2、Tim14_Init为定时器14初始化，定时器14用来提供调度器的时钟，该定时器的中断优先级最低
3、TIM1_BRK_UP_TRG_COM_IRQHandler 中断服务函数，未使用
4、TIM14_IRQHandler 定时器14中断服务函数，其中更新调度器时基，
	 在本函数内执行sys_timer_task，是为了保证系统时间不被应用程序阻塞。
5、dac_task PWM模拟DAC任务函数，更新频率为2HZ，DAC_MAX_OUT表示最大输出电压
*/

extern u32 SystemCoreClock;

void Tim1_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA, ENABLE);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_1);

    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM1, ENABLE);

    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = 1000;
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    //Setting Clock Segmentation
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    //TIM Upward Counting Mode
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

//    TIM_ARRPreloadConfig(TIM1, ENABLE);
//    TIM_ClearFlag(TIM1, TIM_FLAG_Update);
//    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
		
//	  exNVIC_Init_TypeDef  NVIC_InitStruct;
//    NVIC_InitStruct.NVIC_IRQChannel = TIM1_BRK_UP_TRG_COM_IRQn;
//    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
//    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
//    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;

//    exNVIC_Init(&NVIC_InitStruct);

		TIM_OCInitTypeDef  TIM_OCInitStructure;

    TIM_OCStructInit(&TIM_OCInitStructure);
    //Select Timer Mode: TIM Pulse Width Modulation Mode 1
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    //Setting the Pulse Value of the Capture Comparison Register to be Loaded
    TIM_OCInitStructure.TIM_Pulse = 0;
    //Output polarity: TIM output is more polar
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);

    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM1, ENABLE);
    TIM_CtrlPWMOutputs	(TIM1, ENABLE);
		
		TIM_Cmd(TIM1, ENABLE);
}

void Tim14_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM14, ENABLE);

    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
//		TIM_TimeBaseStructure.TIM_Period = 200 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = SystemCoreClock / 1000000 - 1;
    //Setting Clock Segmentation
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    //TIM Upward Counting Mode
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);

    TIM_ARRPreloadConfig(TIM14, ENABLE);
    TIM_ClearFlag(TIM14, TIM_FLAG_Update);
    TIM_ITConfig(TIM14, TIM_IT_Update, ENABLE);
		
	  exNVIC_Init_TypeDef  NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = TIM14_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    exNVIC_Init(&NVIC_InitStruct);
		
		TIM_Cmd(TIM14, ENABLE);
}

//void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
//{
//    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
//		{
//        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
//    }
//}
void TIM14_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM14, TIM_IT_Update) != RESET)
		{
				UpdateTimers();
				RunTask(sys_timer_task,     	0);
				RunTask(receive_data_task,		9);
        TIM_ClearITPendingBit(TIM14, TIM_IT_Update);
    }
}

uint32_t  sys_timer_task(void){
_SS
  while(UERROR){
		
//		if(sys_info.run_timer <  INIF_TIME)
//		{
//			led_sys.led_sta = LED_FLASH_1HZ;
//		}
//		else
//		{
//			led_sys.led_sta = LED_FLASH_5HZ;
//		}  //可以不在这里做LED处理
		sys_info.run_timer++;
		WaitX(1000); 
  }
_EE
}
//DA模式输出的代码，通过PWM调节电压输出
#if 0
uint32_t	dac_task(void){
static	int16_t	temp = 0;
_SS
  while(UERROR){
		if(fabs(sys_info.dac_output_vol-sys_info.dac_input_vol)>0.2f)
		{
			temp = sys_info.dac_output_vol*200;
		}
		else
		{
			if(sys_info.dac_output_vol-sys_info.dac_input_vol > 0.05)
			{
				temp = temp+10;
			}
			else if(sys_info.dac_output_vol-sys_info.dac_input_vol < -0.05)
			{
				temp = temp-10;
			}
			else
			{
				//NULL
			}
			if(sys_info.dac_output_vol-sys_info.dac_input_vol > 0.005)
			{
				temp = temp+1;
			}
			else if(sys_info.dac_output_vol-sys_info.dac_input_vol < -0.005)
			{
				temp = temp-1;
			}
			else
			{
				//NULL
			}
		}
		if(temp > DAC_MAX_OUT*200)
		{
			temp = DAC_MAX_OUT*200;
		}
		if(temp < 0)
		{
			temp = 0;
		}
		TIM_SetCompare1(TIM1,(uint32_t)temp);
		WaitX(500); 
  }
_EE
}
#endif
