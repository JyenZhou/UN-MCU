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
#ifndef __JHAL_Delay__H__
#define  __JHAL_Delay__H__

#ifdef __CplusPlus
extern "C" {
#endif

#include "JUTIL.h"

    typedef struct {
//��Ƶ
        u8 basicFrequency;

    } JHAL_DealyConfig;


    //JHAL_delayInit((JHAL_DealyConfig){NULL});
    void  JHAL_delayInit(JHAL_DealyConfig config);
    void  JHAL_delayMs(u32 ms);
    void  JHAL_delayUs(u32 us);
    void  JHAL_delay(JHAL_TimeUnits timeUntis,u16 value);

    //�쳣��� ������Ҫ��ֹδ��ʼ������
    void  JHAL_delayTryCheckedExceptions(void);




#ifdef CplusPlus
}
#endif

#endif








