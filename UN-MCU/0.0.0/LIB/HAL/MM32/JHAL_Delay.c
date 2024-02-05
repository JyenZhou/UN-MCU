#include "../JHAL_Delay.h"


//注意  ：这个定时器不知道关闭所有中断后是否依然生效 导致硬延时失败 ，通信时卡死

static  vu32  __JHAL_DealyMsCount=0;


void  JHAL_delayOpen(JHAL_Delay config)
{

    if (SysTick_Config(RCC_GetHCLKFreq() / 1000  ))
    {
        while (1)
        {
        }
    }



    NVIC_SetPriority(SysTick_IRQn, 0x0);
}




void  JHAL_delayMs(u32 ms)
{

    __JHAL_DealyMsCount = ms;

    while (0 != __JHAL_DealyMsCount)
    {
    }


}


void SysTick_Handler(void)
{

    if (0 != __JHAL_DealyMsCount)
    {
        __JHAL_DealyMsCount--;
    }
}



