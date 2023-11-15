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
#ifndef __JHAL_DAC__H__
#define  __JHAL_DAC__H__

#ifdef __CplusPlus
extern "C" {
#endif

#include "JUTIL.h"




    typedef enum {
        JHAL_DAC_ReferVoltage_NONE,
        JHAL_DAC_ReferVoltage_VDD,
        JHAL_DAC_ReferVoltage_Vref,

    }
    JHAL_DAC_ReferSoure;


    typedef struct
    {   //��ѹϵ������
        float calculationCoefficient;
    } __JHAL_DACOtherInfo;





    typedef struct
    {
        u8 dev:3;
        JHAL_DAC_ReferSoure vref;


        float vrefVoltageValue;
        __JHAL_DACOtherInfo __other;
    } JHAL_DAC;



    bool  JHAL_dacOpen( JHAL_DAC *dac);


    bool  JHAL_dacCloase(JHAL_DAC *dac  );

    void JHAL_dacSetVoltage (JHAL_DAC *dac,float voltage);

    void JHAL_dacSetDA(JHAL_DAC *dac,u16 da );


#ifdef CplusPlus
}
#endif

#endif








