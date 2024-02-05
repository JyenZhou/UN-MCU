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





    typedef struct {
        bool isOpen:1;

    } __JHAL_RTC_OtherInfo;


    typedef struct {
        u8 id:3;
        JHAL_TimeUnits itTimeUnit;
//itTimeValue 中断时间值 0-1000
        u16 itTimeValue;

        __JHAL_RTC_OtherInfo __info;

    } JHAL_RTC;

    bool JHAL_rtcOpen(JHAL_RTC *rtc  );
//RTC中断回调
    void JHAL_rtcInterruptCallback(u8 dev);
    bool  JHAL_rtcClose(JHAL_RTC *rtc);
#ifdef CplusPlus
}
#endif

#endif








