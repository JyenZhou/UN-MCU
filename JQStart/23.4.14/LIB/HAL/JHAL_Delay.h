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

    } JHAL_Delay;






    //  JHAL_delayOpen((JHAL_Delay){NULL});
    void  JHAL_delayOpen(JHAL_Delay config);
    void  JHAL_delayMs(u32 ms);
    void  JHAL_delayUs(u32 us);
    void  JHAL_delay(JHAL_TimeUnits timeUntis,u16 value);

		//�������ʱ�Ǻ�ͨ����صĲ��ɴ��   ׼ȷ�Ƚϸ� ʹ��ʱ�жϱ�������Ҳ������ʹ��    ��osDelay������������ʱ�����ͬ��
   
		//����os delay Ĭ�Ͽ��Բ�ʵ��  ��ʹ���������������Ӳ��ʱ
	  void  JHAL_delayOsMs (u32 ms); 
    void  JHAL_delayOsUs(u32 us);
    void  JHAL_delayOs(JHAL_TimeUnits timeUntis,u16 value);

 

#ifdef CplusPlus
}
#endif

#endif








