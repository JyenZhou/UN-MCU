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
    } JHAL_WDGType;



    typedef struct
    {

//itTimeValue �ж�ʱ��ֵ 0-1000
        u16 itTimeValue;

//���ڿ��Ź�ʱ�õ�  ��Сʱ�� ��itTimeValue�൱����󴰿��ڣ�
        u16 itTimeMinValue;
        JHAL_TimeUnits itTimeUnit;
    } JHAL_WDGConfig;



    bool  JHAL_wdgInit(JHAL_WDGType wdg, JHAL_WDGConfig config);
    bool  JHAL_wdgDeInit(JHAL_WDGType wdg);
    void  JHAL_wdgFeed(void);
    //��λǰ�жϣ����ж�����û��ι����λ */
    void JHALwdgBeforeInterruptCallBack(JHAL_WDGType wdg);



#ifdef CplusPlus
}
#endif

#endif








