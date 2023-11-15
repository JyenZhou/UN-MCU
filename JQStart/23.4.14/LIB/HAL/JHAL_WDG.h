/*************Jyen*************************Jyen********************Jyen***********************Jyen********************
   *                                                                *
 *                                                                   *
*
 		ADCĬ��ͨ��HAL����
  ʹ��ADC0 �����Ӳ�ͬ�ͺŵ�Ƭ�� ��Ҫ�ڸ�ƽ̨��Ӧ.c�������������ӳ��
*                                                                     *
  *                                                                 *
   *                                                              *
*********Jyen******************Jyen************************Jyen*****************************************/
#ifndef __JHAL_WDG__H__
#define  __JHAL_WDG__H__

#ifdef __CplusPlus
extern "C" {
#endif

#include "JUTIL.h"

    typedef enum
    {
        //ȫ�ֿ��Ź�
        JHAL_WDG_GWDG=0,
        //���ڿ��Ź�
        JHAL_WDG_WWDG,
    } JHAL_WDG_Dev;


    typedef struct
    {

        bool isOpen:1;

    } __JHAL_WDG_OtherInfo ;



    typedef struct
    {
        JHAL_WDG_Dev dev;

//itTimeValue �ж�ʱ��ֵ 0-1000
        u16 itTimeValue;

//���ڿ��Ź�ʱ ��Сʱ����Ч
        u16 itTimeMinValue;
        u16 itTimeMaxValue;
        JHAL_TimeUnits itTimeUnit;

        __JHAL_WDG_OtherInfo __info;
    } JHAL_WDG;


//JHAL_wdgInit((JHAL_WDGType){NULL},(JHAL_WDGConfig){NULL});
    bool  JHAL_wdgOpen(JHAL_WDG *wdg);
    bool  JHAL_wdgClose(JHAL_WDG  *wdg);
    void  JHAL_wdgFeed(JHAL_WDG  *wdg);
    //��λǰ�жϣ����ж�����û��ι����λ */
    void JHALwdgBeforeInterruptCallBack(JHAL_WDG_Dev dev);



#ifdef CplusPlus
}
#endif

#endif







