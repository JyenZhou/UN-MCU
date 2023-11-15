
/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*
 		�Ͳɼ���ѹ��ص�����
*                                                                     *
  *                                                                 *
   *                                                              *
*********@����*Jyen******************@����*Jyen***********************@����*Jyen********************/

#include "voltage.h"





/*------------------Jyen--------------------------Jyen-----------------------*/
/*******************************************************************************
*@ �������ܻ���: ���Ȳ�����һ�����Ҫһ���ȡ
  * @�������:   ��
  * @�� �� ֵ: ��
  * @��ע: ��
  *
  *
  ******************************************************************************
  *
  *------------------Jyen-------------------------Jyen-------------------------*/

GASVoltageInfo getGASVoltageInfo(void)
{

    GASVoltageInfo sensorsInfo= {0};
    sensorsInfo.vHL[GAS_NO2] =JHAL_adcAqcSingle(SystemADC,adcChannelRemap[(uint8)AD_CH_NO2_VH]).middleVoltage;
    sensorsInfo.vL[GAS_NO2]   =JHAL_adcAqcSingle(SystemADC,adcChannelRemap[(uint8)AD_CH_NO2_VL]).middleVoltage;

    //���ȵ�ѹ�����ж�
    if(fabs (sensorsInfo.vHL[GAS_NO2] -GAS_VHL_Predefine[GAS_NO2])>GAS_VHL_Tolerance )
    {
        sensorsInfo.SensorStatus|=0x01;

    }
#ifndef ASM31X003
    sensorsInfo.vHL[GAS_NH3]  =JHAL_adcAqcSingle(SystemADC,adcChannelRemap[(uint8)AD_CH_NH3_VH]).middleVoltage;
#endif

    sensorsInfo.vL[GAS_NH3]  =JHAL_adcAqcSingle(SystemADC,adcChannelRemap[(uint8)AD_CH_NH3_VL]).middleVoltage;



    if(fabs (sensorsInfo.vHL[GAS_NH3] -GAS_VHL_Predefine[GAS_NH3])>GAS_VHL_Tolerance )
    {
        sensorsInfo.SensorStatus|=0x02;

    }
#ifndef ASM31X003
    sensorsInfo.vHL[GAS_CO]  =JHAL_adcAqcSingle(SystemADC,adcChannelRemap[(uint8)AD_CH_CO_VH]).middleVoltage;
#endif
    sensorsInfo.vL[GAS_CO]=JHAL_adcAqcSingle(SystemADC,adcChannelRemap[(uint8)AD_CH_CO_VL]).middleVoltage;


    if(fabs (sensorsInfo.vHL[GAS_CO] -GAS_VHL_Predefine[GAS_CO])>GAS_VHL_Tolerance )

    {
        sensorsInfo.SensorStatus|=0x04;

    }





    return sensorsInfo;

}






