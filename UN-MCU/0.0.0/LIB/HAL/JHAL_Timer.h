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



    typedef struct {
        bool isOpen;

    } __JHAL_Timer_therInfo;


    typedef struct {

        u8 id;
//�жϵ�ʱ�䵥λ
        JHAL_TimeUnits itTimeUnit;
//itTimeValue �ж�ʱ��ֵ 0-1000
        u16 itTimeValue;
        __JHAL_Timer_therInfo __info;
        //			����ڲ�ʹ�����������һ��Ҫ�ȸ�ֵ
        void*  dev;
    } JHAL_Timer;



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
      *  JHAL_timerOpen(( JHAL_Timer *) NULL);
     **/
    /* -----------------------------Jyen-2022-11-07-------------------------------------- */

    bool JHAL_timerOpen(JHAL_Timer *timer );
    bool  JHAL_timerDeInit(JHAL_Timer *timer);
//�жϻص�  ע�������Ӧ����uartʵ�� id ���Ǵ���ȥ��dev  ��Ϊ����dev����ʵ������һ��uart ֻ�����Ų�һ��
    void JHAL_timerInterruptCallback( u8 realityId);

#ifdef CplusPlus
}
#endif

#endif








