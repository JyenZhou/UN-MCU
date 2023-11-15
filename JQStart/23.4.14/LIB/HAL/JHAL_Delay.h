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



    u32 JHAL_uidGetHigh(void);
    u32 JHAL_uidGetMiddle(void);
    u32 JHAL_uidGetLow(void);
// 96λUID�൱��3��32λ����  ÿ8λ�����൱��һ���ַ�  ���������Ҫ4*3+1 =13���ַ��ռ� Ϊ�˼��� ��λuid�ȴ�
    void uid2string(char* string,int buffSize );



    //  JHAL_delayOpen((JHAL_Delay){NULL});
    void  JHAL_delayOpen(JHAL_Delay config);
    void  JHAL_delayMs(u32 ms);
    void  JHAL_delayUs(u32 us);
    void  JHAL_delay(JHAL_TimeUnits timeUntis,u16 value);

    //�쳣��� ������Ҫ��ֹδ��ʼ������
    void  JHAL_delayTryCheckedExceptions(void);




#ifdef CplusPlus
}
#endif

#endif







