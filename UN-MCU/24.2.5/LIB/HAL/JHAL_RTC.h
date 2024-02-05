/*************Jyen*************************Jyen********************Jyen***********************Jyen********************
   *                                                                *
 *                                                                   *
*
 		CANĬ��ͨ��HAL����
  ʹ��CAN0 ֡���� Ĭ������֡��Զ��֡         �����Ӳ�ͬ�ͺŵ�Ƭ�� ��Ҫ�ڸ�ƽ̨��Ӧ.c�������������ӳ��
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
//itTimeValue �ж�ʱ��ֵ 0-1000
        u16 itTimeValue;

        __JHAL_RTC_OtherInfo __info;

    } JHAL_RTC;

    bool JHAL_rtcOpen(JHAL_RTC *rtc  );
//RTC�жϻص�
    void JHAL_rtcInterruptCallback(u8 dev);
    bool  JHAL_rtcClose(JHAL_RTC *rtc);
#ifdef CplusPlus
}
#endif

#endif







