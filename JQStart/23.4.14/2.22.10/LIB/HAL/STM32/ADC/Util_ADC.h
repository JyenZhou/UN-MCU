#ifndef __MULITICHANNELADC__H__
#define  __MULITICHANNELADC__H__

#ifdef __CplusPlus
extern "C" {
#endif

#include "JUTIL.h"
#define ADC_NUMOFCHANNEL                2

//����12λ��ADC��3.3V��ADCֵΪ0xfff,�¶�Ϊ25��ʱ��Ӧ�ĵ�ѹֵΪ1.43V��0x6EE
#define V25  0x6EE
    //б�� ÿ���϶�4.3mV ��Ӧÿ���϶�0x05
#define AVG_SLOPE 0x05

// ADת�����ֵ


    extern ADC_HandleTypeDef hadc1;

 //       float  TEMP=(1.42 -FADC_ConvertedValueLocal[ADC_NUMOFCHANNEL-2])*1000/4.35 + 25;//ת��Ϊ�¶�ֵ,ʵ��Ӧ����,�ɿ����ú���Ϊ��λ,���⸡������
//   Current_Temperature=(V25-ADC_ConvertedValue[ADC_NUMOFCHANNEL-2])/AVG_SLOPE+25;//ת��Ϊ�¶�ֵ,ʵ��Ӧ����,�ɿ����ú���Ϊ��λ,���⸡������
//    float cofe=1.2/((ADC_ConvertedValue[ADC_NUMOFCHANNEL-1]&0xFFF)*3.3f/4096); // ADC_ConvertedValue[0]ֻȡ���12��Ч����
    extern 	  uint32_t ADC_ConvertedValue[ADC_NUMOFCHANNEL];
    void utilADC_Init (void);


#ifdef CplusPlus
}
#endif

#endif




