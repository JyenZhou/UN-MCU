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


#ifndef __JHAL_Timer__H__
#define __JHAL_Timer__H__
#include "JUTIL.h"
#ifdef __CplusPlus
extern "C" {
#endif


    typedef enum
    {
        JHAL_Timer0=0,
        JHAL_Timer1,
        JHAL_Timer_Number
    } JHAL_Timer;




    typedef struct {
//�жϵ�ʱ�䵥λ
        JHAL_TimeUnits itTimeUnit;
//itTimeValue �ж�ʱ��ֵ 0-1000
        u16 itTimeValue;

    } JHAL_TimerConfig;



    /* ----------------------------JHAL_timerInit----------------------------------- */
    /**
      * ����:
      *
      *
      *
      * ����:
    **		  	 timer: [����/��]
    **			 config: [����/��]
      *
      * ����ֵ:��
      *
     **/
    /* -----------------------------Jyen-2022-11-07-------------------------------------- */

    void JHAL_timerInit(JHAL_Timer timer,JHAL_TimerConfig config);
    void  JHAL_timerDeInit(JHAL_Timer timer);
//�жϻص�
    void JHAL_timerInterruptCallBack(JHAL_Timer timer);

#ifdef CplusPlus
}
#endif

#endif








