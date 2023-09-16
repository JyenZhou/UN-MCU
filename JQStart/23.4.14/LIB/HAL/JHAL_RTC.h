/*************Jyen*************************Jyen********************Jyen***********************Jyen********************
   *                                                                *
 *                                                                   *
*
 		CAN默认通用HAL驱动
  使用CAN0 帧类型 默认数据帧非远程帧         若增加不同型号单片机 需要在该平台对应.c中增加相关引脚映射
*                                                                     *
  *                                                                 *
   *                                                              *
*********Jyen******************Jyen************************Jyen*****************************************/


#ifndef __JHAL_RTC__H__
#define __JHAL_RTC__H__
#include "JUTIL.h"
#ifdef __CplusPlus
extern "C" {
#endif

    typedef enum
    {
        JHAL_RTC0=0,
        JHAL_RTC_Number
    } JHAL_RTC;





    typedef struct {

        JHAL_TimeUnits itTimeUnit;
//itTimeValue 中断时间值 0-1000
        u16 itTimeValue;

    } JHAL_RTCConfig;

    void JHAL_rtcInit(JHAL_RTC rtc,JHAL_RTCConfig config);
//RTC中断回调
    void JHAL_rtcInterruptCallBack(JHAL_RTC rtc);
    void  JHAL_rtcDeInit(JHAL_RTC rtc);
#ifdef CplusPlus
}
#endif

#endif








