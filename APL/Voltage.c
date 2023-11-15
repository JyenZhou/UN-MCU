
/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*
 		和采集电压相关的任务
*                                                                     *
  *                                                                 *
   *                                                              *
*********@作者*Jyen******************@作者*Jyen***********************@作者*Jyen********************/

#include "voltage.h"





/*------------------Jyen--------------------------Jyen-----------------------*/
/*******************************************************************************
*@ 函数功能或简介: 加热测量是一起的需要一起获取
  * @输入参数:   无
  * @返 回 值: 无
  * @备注: 无
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

    //加热电压故障判断
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







