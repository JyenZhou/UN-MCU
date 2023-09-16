#ifndef __MULITICHANNELADC__H__
#define  __MULITICHANNELADC__H__

#ifdef __CplusPlus
extern "C" {
#endif

#include "JUTIL.h"
#define ADC_NUMOFCHANNEL                2

//对于12位的ADC，3.3V的ADC值为0xfff,温度为25度时对应的电压值为1.43V即0x6EE
#define V25  0x6EE
    //斜率 每摄氏度4.3mV 对应每摄氏度0x05
#define AVG_SLOPE 0x05

// AD转换结果值


    extern ADC_HandleTypeDef hadc1;

 //       float  TEMP=(1.42 -FADC_ConvertedValueLocal[ADC_NUMOFCHANNEL-2])*1000/4.35 + 25;//转换为温度值,实际应用中,可考虑用毫伏为单位,避免浮点运算
//   Current_Temperature=(V25-ADC_ConvertedValue[ADC_NUMOFCHANNEL-2])/AVG_SLOPE+25;//转换为温度值,实际应用中,可考虑用毫伏为单位,避免浮点运算
//    float cofe=1.2/((ADC_ConvertedValue[ADC_NUMOFCHANNEL-1]&0xFFF)*3.3f/4096); // ADC_ConvertedValue[0]只取最低12有效数据
    extern 	  uint32_t ADC_ConvertedValue[ADC_NUMOFCHANNEL];
    void utilADC_Init (void);


#ifdef CplusPlus
}
#endif

#endif




