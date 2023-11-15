/*************Jyen*************************Jyen********************Jyen***********************Jyen********************
   *                                                                *
 *                                                                   *
*
 		ADC默认通用HAL驱动
  使用ADC0 若增加不同型号单片机 需要在该平台对应.c中增加相关引脚映射
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
    {   //电压系数计算
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








