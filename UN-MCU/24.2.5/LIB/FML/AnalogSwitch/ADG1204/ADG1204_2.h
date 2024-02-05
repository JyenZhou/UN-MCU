#ifndef  __ADG1204_2__H__
#define  __ADG1204_2__H__
#include "ADG1204.h"
#ifdef __CplusPlus
extern "C" {
#endif

 
#define ADG1204_2_EN_Pin EN2_Pin
#define ADG1204_2_EN_GPIO_Port EN2_GPIO_Port
#define ADG1204_2_A0_Pin A0M2_Pin
#define ADG1204_2_A0_GPIO_Port A0M2_GPIO_Port
#define ADG1204_2_A1_Pin A1M2_Pin
#define ADG1204_2_A1_GPIO_Port A1M2_GPIO_Port


		
		
		
		
void  ADG1204_2_init(void);
		//下面的是主要函数 上面的可以不用管	
void ADG1204_2_ChannelSelect(ADG1204_Channel_X_TypeDefE  Channel);



#ifdef CplusPlus
}
#endif

#endif

