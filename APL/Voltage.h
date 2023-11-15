#ifndef __ADCHandlerTask__H__
#define __ADCHandlerTask__H__
#include "JFML.h"
#include "gatt.h"
#ifdef __CplusPlus
extern "C" {
#endif




    typedef struct
    {
//非0 故障  故障位bit0=GAS_NO2  bit1=GAS_NH3 bit2=GAS_CO
        u8 SensorStatus;
        float vL[GAS_Number];
//加热匹配电阻的电压
        float vHL[GAS_Number];

    } GASVoltageInfo;

//获取传感器电压
    GASVoltageInfo getGASVoltageInfo(void);



#ifdef CplusPlus
}
#endif

#endif




