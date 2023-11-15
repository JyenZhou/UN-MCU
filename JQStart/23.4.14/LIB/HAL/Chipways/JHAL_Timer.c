#include "../JHAL_Timer.h"
#include "xl_pit.h"




void __JHAL_timerInit(u8 dev, JHAL_TimeUnits unti,u16 itTimeValue)
{
    JHAL_disableInterrupts();
    SIM_SOPT0_BusClockDivide(BUSCLOCK_OUTPUT_DIVIDE_128);
    SIM_SOPT0_BusClockOutputCmd(ENABLE); //bus clock out

    SIM_SCGC_Cmd(SIM_SCGC_PIT,ENABLE); //PIT模块使能 系统集成模块 SCGC = 0x2

    PIT_DeInit(); //timer = 0 禁用定时器



    if(dev==0) {
        PIT_Init((uint8_t)PIT_Channel0,PIT_Count_Mode,0); //set PIT[0] 用户定义模式 0分频
        PIT_InterruptEn((uint8_t)PIT_Channel0,ENABLE); //中断使能
        PIT_SetLoadCount((uint8_t)PIT_Channel0,unti/JHAL_TimeUnits_MS*48000* itTimeValue); //计数 48000  		//1ms定时器
        PIT_EnableCmd((uint8_t)PIT_Channel0,ENABLE );	 // 使能定时器

    } else if(dev==1) {

        PIT_Init((uint8_t)PIT_Channel1,PIT_Count_Mode,0);//set PIT[1]
        PIT_InterruptEn((uint8_t)PIT_Channel1,ENABLE);
        PIT_SetLoadCount((uint8_t)PIT_Channel1,unti/JHAL_TimeUnits_MS*48000* itTimeValue);
        PIT_EnableCmd((uint8_t)PIT_Channel1,ENABLE );


    } else {
        while(true);
    }

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //
    NVIC_InitStructure.NVIC_IRQChannel = (  dev==0)?(uint8_t)PIT_CH0_IRQn: (uint8_t)PIT_CH1_IRQn;  //中断通道，中断号->中断函数
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ; //抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	//子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 	//使能中断
    NVIC_Init(&NVIC_InitStructure);


    JHAL_enableInterrupts();

}



 

bool JHAL_timerOpen(JHAL_Timer   *config)
{
	  if(!config->__info. isOpen) {
    
        __JHAL_timerInit(config->dev,config->itTimeUnit,config->itTimeValue); //设置初始时间
    
		 return  config->__info.isOpen=  true;
	}
		 return  false;
}

bool  JHAL_timerClose(JHAL_Timer *timer)
{
    if( timer->__info. isOpen) {
			
        PIT_DeInit(); //timer = 0 禁用定时器
			 timer->__info.isOpen=false;
		 return true;
    }
		 return  false;
}






/**
 * @brief  PIT通道0中断处理函数
 */
void PIT_CH0_IRQHandler(void)
{

    JHAL_timerInterruptCallBack(0);
    PIT_ClrInterrupt((uint8_t)PIT_Channel0); //清楚PIT中断
}

/**
 * @brief  PIT通道1中断处理函数
 */
void PIT_CH1_IRQHandler(void)
{

    JHAL_timerInterruptCallBack(1);
    PIT_ClrInterrupt((uint8_t)PIT_Channel1); //清楚PIT中断
}


__attribute__((weak)) void JHAL_timerInterruptCallBack(u8 realityId)
{
}
