



#include "../JHAL_Timer.h"





bool JHAL_timerOpen(JHAL_Timer *timer )
{
    NVIC_InitTypeDef        NVIC_InitStruct;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

    RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM1, ENABLE);

    TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
    TIM_TimeBaseInitStruct.TIM_Prescaler         = (RCC_GetPCLK1Freq() / 10000 - 1);
    TIM_TimeBaseInitStruct.TIM_CounterMode       = TIM_CounterMode_Up;
	
	if(timer->itTimeUnit==JHAL_TimeUnits_MS)
	{
		 TIM_TimeBaseInitStruct.TIM_Period            = 5*timer->itTimeValue - 1;
	}else 	if(timer->itTimeUnit==JHAL_TimeUnits_S)
	{
		 	 TIM_TimeBaseInitStruct.TIM_Period            = 5000*timer->itTimeValue - 1;
	}else
	{
		   JHAL_Fault_Handler("JHAL_timerOpen");
	}
    
    TIM_TimeBaseInitStruct.TIM_ClockDivision     = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);

    TIM_ClearFlag(TIM1, TIM_FLAG_Update);
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

    NVIC_InitStruct.NVIC_IRQChannel = TIM1_BRK_UP_TRG_COM_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 0x01;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    TIM_Cmd(TIM1, ENABLE);
    return true;
}




/**
  * 函数功能: 非阻塞模式下定时器的回调函数
  * 输入参数: htim：定时器句柄
  * 返 回 值: 无
  * 说    明: 里面执行语句
  */
void TIM1_BRK_UP_TRG_COM_IRQHandler()

{
    if (SET == TIM_GetITStatus(TIM1, TIM_IT_Update))
    {
        JHAL_timerInterruptCallBack(1);

        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }


}


