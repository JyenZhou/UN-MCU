#include "../JHAL_RTC.h"
#include "xl_rtc.h"
#include <xl_ics.h>
/**
 * @param  RTC_SOURCE_CLK  RTC时钟源选择
 *		@arg 0: bus clock
 *		@arg 1：内部基准时钟
 *		@arg 2：外部振荡器
 *		@arg 3：低功耗振荡器
 */
#define RTC_SOURCE_CLK  1

/**
  * 描述    设置RTC时间
  * 输入   无
  * 返回   无
  */

void __JHAL_rtcTimeInit(RTC_Type *RTCx,JHAL_TimeUnits type,u16 itTimeValue)
{
    RTC_InitTypeDef RTC_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    NVIC_InitStructure.NVIC_IRQChannel = (uint8_t)RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    SIM_SCGC_Cmd(SIM_SCGC_RTC,ENABLE);  //打开RTC模块总线时钟


#if (RTC_SOURCE_CLK == 1)
    {
        ICS_IRCEnableCmd(ENABLE);
        RTC_InitStructure.RTC_ClkSrc = RTC_Internal_Clock;  // 100/150KHZ
        RTC_InitStructure.RTC_ClkPres = RTC_Clk_Prescaler_64or1000; //分频 64
        RTC_Init(RTCx,&RTC_InitStructure);  //初始化
        if(type==JHAL_TimeUnits_S) {

            while(3000*itTimeValue>65535);
            RTC_SetModValue(RTCx, 2363*itTimeValue);		//模数，count ~1S
        } else if(type==JHAL_TimeUnits_MS) {
            while(((2363*itTimeValue)/1000)>65535);
            RTC_SetModValue(RTCx, (2363*itTimeValue)/1000);		//模数，count ~1MS

        } else {
            while(true);
        }




    }
#elif (RTC_SOURCE_CLK == 2)
    {
        OSC_InitTypeDef OSC_InitStructure;

        OSC_InitStructure.OSC_OutputSelect = OSC_OSCCILLATOR_COLOR;
        OSC_InitStructure.OSC_RangeSelect = OSC_RANGE_HIGH;
        OSC_InitStructure.OSC_ModeSelect = OSC_HG_MODE;
        OSC_Init(&OSC_InitStructure);

        OSC_EnableCmd(ENABLE);

        SIM_SCGC1_Cmd(SIM_SCGC1_RTCEC,ENABLE);
        RTC_InitStructure.RTC_ClkSrc = RTC_External_Clock;  //24MHZ
        RTC_InitStructure.RTC_ClkPres = RTC_Clk_Prescaler_64or1000; //分频 64
        RTC_Init(RTCx,&RTC_InitStructure);  //初始化
        RTC_SetModValue(RTCx, 37000);		//模数，count 100ms
    }
#elif (RTC_SOURCE_CLK == 3)
    {
        SIM_SCGC1_Cmd(SIM_SCGC1_RTCLPOC,ENABLE);
        RTC_InitStructure.RTC_ClkSrc = RTC_Realtime_Clock;  //32KHZ
        RTC_InitStructure.RTC_ClkPres = RTC_Clk_Prescaler_32or100; //不分频
        RTC_Init(RTCx,&RTC_InitStructure);  //初始化
        RTC_SetModValue(RTCx, 320);		//模数，count
    }
#else //Bus clock
    {
        RTC_InitStructure.RTC_ClkSrc = RTC_Bus_Clock;  //使用bus时钟源 48MHZ
        RTC_InitStructure.RTC_ClkPres = RTC_Clk_Prescaler_64or1000; //分频 1000
        RTC_Init(RTCx,&RTC_InitStructure);  //初始化
        RTC_SetModValue(RTCx, 48000);		//模数，count 1S
    }
#endif



}



/***************************************************************************
 *设置RTC中断，
 *注意 ：RTC的中断源不能直接触发内核中断。RTC中断必须以外部中断Eint17-19做为信号传输通道，才能触发内核的中断。
 *		所以，使用RTC的中断必须要同时配置Eint17-19中断。
 **************************************************************************/
void __JHAL_rtcInterruptEnable(RTC_Type *RTCx)
{
    RTC_InterruptEn(RTCx,ENABLE );		//使能中断
    RTC_EnableCmd(RTCx, ENABLE);	//RTC 使能
    JHAL_enableInterrupts();

}

 

bool JHAL_rtcOpen( JHAL_RTC  *config)
{
    if(!config->__info.isOpen) {
        __JHAL_rtcTimeInit(RTC,config->itTimeUnit,config->itTimeValue); //设置初始时间
        __JHAL_rtcInterruptEnable(RTC);  //配置中断
			
			
			return   config->__info.isOpen=true;
    }
		return false;
}
bool  JHAL_rtcClose(JHAL_RTC *config)
{
    if( config->__info.isOpen) {
        //打开RTC模块总线时钟  因为低功耗后可能会关闭这个时钟  到时候操作无效
        SIM_SCGC_Cmd(SIM_SCGC_RTC,ENABLE);
        RTC_DeInit(RTC);
			
			
				    config->__info.isOpen=false;
			return true;
    }
		return false;
		
}

/**
 * @brief  RTC 中断函数
 * @param
 * @retval 无返回值
 */
void RTC_IRQHandler(void)
{

    RTC_ClrInterruptFlag(RTC); //清空中断
    JHAL_rtcInterruptCallBack(0);

}



__attribute__((weak)) void JHAL_rtcInterruptCallBack(u8 dev)
{
}
