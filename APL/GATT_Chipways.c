
/*************Jyen*************************Jyen********************Jyen***********************Jyen********************
   *                                                                *
 *                                                                   *
*
 		通用属性配置
当不同平台时API相同时参数不通用时
*                                                                     *
  *                                                                 *
   *                                                              *
*********Jyen******************Jyen************************Jyen*****************************************/

#include "GATT.h"

const JHAL_IO_Port LIN_WAKE_EN_Port=JHAL_IOD;
const u8 LIN_WAKE_EN_Pin=2;
const JHAL_IO_Port LIN_SLP_EN_Port=JHAL_IOD;
const u8 LIN_SLP_EN_Pin=3;
const JHAL_IO_Port GAS_VCVH_ALL_EN_Port=JHAL_IOB;
const u8 GAS_VCVH_ALL_EN_Pin=5;

//软硬件版本最大值0x0F   一般每次送样或对外输出时更改  debug测试时候不用频繁改这里
const u8 hardwareVersion=2;
const u8 softwareVersion=1;

//传感器总电压
const   u8 gasVCVH_All=5;

u8 adcChannelRemap[ADC_CHANNEL_NUM]= {20,21,10,11,8,9};


JHAL_ADCConfig adcConfig= {.vref=JHAL_ADC_ReferVoltage_VDD,.vrefVoltageValue=5,.channels=adcChannelRemap,.channelsNumber=ADC_CHANNEL_NUM};
const JHAL_LIN   SystemLin  = JHAL_LIN0;

const JHAL_ADC SystemADC=JHAL_ADC0;
//按GAS_ID顺序定义的 加热处匹配电阻（调电压的）
const float GAS_VH_RL[GAS_Number]= {310,175,233};
//理论的的加热 匹配电压
const float GAS_VHL_Predefine[GAS_Number]= {gasVCVH_All-0.9f,gasVCVH_All-1.8f,gasVCVH_All-1.3f};
//容许偏差 ±x
const float GAS_VHL_Tolerance= 0.4;

