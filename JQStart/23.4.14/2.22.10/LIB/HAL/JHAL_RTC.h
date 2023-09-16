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

    typedef enum
    {
        JHAL_RTC0=0,
        JHAL_RTC_Number
    } JHAL_RTC;





    typedef struct {

        JHAL_TimeUnits itTimeUnit;
//itTimeValue �ж�ʱ��ֵ 0-1000
        u16 itTimeValue;

    } JHAL_RTCConfig;

    void JHAL_rtcInit(JHAL_RTC rtc,JHAL_RTCConfig config);
//RTC�жϻص�
    void JHAL_rtcInterruptCallBack(JHAL_RTC rtc);
    void  JHAL_rtcDeInit(JHAL_RTC rtc);
#ifdef CplusPlus
}
#endif

#endif








