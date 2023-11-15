#ifndef __ADCHandlerTask__H__
#define __ADCHandlerTask__H__
#include "JFML.h"
#include "gatt.h"
#ifdef __CplusPlus
extern "C" {
#endif




    typedef struct
    {
//��0 ����  ����λbit0=GAS_NO2  bit1=GAS_NH3 bit2=GAS_CO
        u8 SensorStatus;
        float vL[GAS_Number];
//����ƥ�����ĵ�ѹ
        float vHL[GAS_Number];

    } GASVoltageInfo;

//��ȡ��������ѹ
    GASVoltageInfo getGASVoltageInfo(void);



#ifdef CplusPlus
}
#endif

#endif




