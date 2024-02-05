#include "../JHAL_RTC.h"
 
bool __JHAL_rtcIsENCheak(JHAL_RTC id, bool isEN) {
    static bool sEN[JHAL_RTC_Number]= {false};
    if(sEN[(u8)id]!=isEN)
    {
        sEN[(u8)id]=isEN;
        return true;
    }
    return  false;
}

void JHAL_rtcInit(JHAL_RTC rtc,JHAL_RTCConfig config)
{
    if(__JHAL_rtcIsENCheak(rtc,true)) {

    }
}
void  JHAL_rtcDeInit(JHAL_RTC rtc)
{
    if(__JHAL_rtcIsENCheak(rtc,false)) {

    }
}

/**
 * @brief  RTC 中断函数
 * @param
 * @retval 无返回值
 */
void RTC_IRQHandler(void)
{


    JHAL_rtcInterruptCallBack(JHAL_RTC0);

}



__attribute__((weak)) void JHAL_rtcInterruptCallBack(JHAL_RTC rtc)
{
}
