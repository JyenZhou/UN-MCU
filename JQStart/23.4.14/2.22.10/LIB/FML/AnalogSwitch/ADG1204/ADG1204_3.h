#ifndef  __ADG1204_3__H__
#define  __ADG1204_3__H__
#include "ADG1204.h"
#ifdef __CplusPlus
extern "C" {
#endif

 
#define ADG1204_3_EN_Pin EN3_Pin
#define ADG1204_3_EN_GPIO_Port EN3_GPIO_Port
#define ADG1204_3_A0_Pin A0M3_Pin
#define ADG1204_3_A0_GPIO_Port A0M3_GPIO_Port
#define ADG1204_3_A1_Pin A1M3_Pin
#define ADG1204_3_A1_GPIO_Port A1M3_GPIO_Port


 
		
void  ADG1204_3_init(void);
		//���������Ҫ���� ����Ŀ��Բ��ù�	
void ADG1204_3_ChannelSelect(ADG1204_Channel_X_TypeDefE  Channel);



#ifdef CplusPlus
}
#endif

#endif

