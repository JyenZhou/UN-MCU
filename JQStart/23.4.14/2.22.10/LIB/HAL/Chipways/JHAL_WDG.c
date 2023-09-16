#include "../JHAL_WDG.h"
#include "xl_wdog.h"



bool __JHAL_wdgIsENCheak(JHAL_WDGType id, bool isEN) {
    static bool sEN[2]= {false};
    if(sEN[(u8)id]!=isEN)
    {
        sEN[(u8)id]=isEN;
        return true;
    }
    return  false;
}
void __JHAL_wdgNVIC(FunctionalState isEn)
{

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = WDOG_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = isEn;
    NVIC_Init(&NVIC_InitStructure);

}

void  JHAL_wdgFeed(void)
{

    WDOG_CountRestart(WDOG);
}

bool  JHAL_wdgDeInit(JHAL_WDGType wdg)
{
    if(__JHAL_wdgIsENCheak(wdg,false)) {
        SIM_SCGC_Cmd(SIM_SCGC_WDG,DISABLE);
        __JHAL_wdgNVIC(DISABLE);
        WDOG_DeInit(WDOG);
        WDOG_EnableCmd(WDOG,DISABLE);
        return true;
    }
    return false;
}


bool  JHAL_wdgInit(JHAL_WDGType wdg, JHAL_WDGConfig config)
{
    if(__JHAL_wdgIsENCheak(wdg,true)) {
        __JHAL_wdgNVIC(ENABLE);
        SIM_SCGC_Cmd(SIM_SCGC_WDG,ENABLE);
        WDOG_DeInit(WDOG);
        WDOG_EnableCmd(WDOG,DISABLE);
        WDOG_InitTypeDef WDOG_InitStructure;
        WDOG_InitStructure.WDOG_ClkSource = WDOG_Bus_Clock; //bus clock时钟源 45MHZ
        WDOG_InitStructure.WDOG_PulseLength = WDOG_PclkCycles_32; //分频
        WDOG_InitStructure.WDOG_DEBUGEn = DISABLE;
        WDOG_InitStructure.WDOG_STOPEn = DISABLE;
        WDOG_InitStructure.WDOG_WAITEn = DISABLE;
        WDOG_InitStructure.WDOG_PRESEn = ENABLE;//256预分频使能
        WDOG_InitStructure.WDOG_ResponseMode = WDOG_ResponseMode_Interrupt; //产生一个中断 WDOG_ResponseMode_Reset
        WDOG_Init(WDOG,&WDOG_InitStructure);
        // WDOG_SetTimeoutPeriod(WDOG,13);		//1S		//0~31 : 0~28 t = 2^(num + 4) 29~31 : t = 2^4 = 16
        WDOG_SetTimeoutPeriod(WDOG,20);	//先大概用个狗
        WDOG_CountRestart(WDOG);
        WDOG_EnableCmd(WDOG,ENABLE);
        return true;
    }
    return  false;
}


/** ----------------------------WDOG_IRQHandler-----------------------------------
  * 描述： 狗前中断  需要使用如下函数进行喂狗否则两次后复位
  * WDOG_CountRestart(WDOG);
    WDOG_ClrInt(WDOG);
  * 参数：
**	  	 None
  *
  * 返回值:无
  *-----------------------------Jyen-2022-11-14-------------------------------------- */

void WDOG_IRQHandler(void)
{
    JHALwdgBeforeInterruptCallBack(JHAL_WDG_GWDG);
}


__weak   void JHALwdgBeforeInterruptCallBack(JHAL_WDGType wdg)
{
}
