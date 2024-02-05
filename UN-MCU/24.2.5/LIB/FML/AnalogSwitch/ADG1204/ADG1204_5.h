#ifndef  __ADG1204_5__H__
#define  __ADG1204_5__H__
#include "ADG1204.h"
#ifdef __CplusPlus
extern "C" {
#endif

 
#define ADG1204_5_EN_Pin EN5_Pin
#define ADG1204_5_EN_GPIO_Port EN5_GPIO_Port
#define ADG1204_5_A0_Pin A0M5_Pin
#define ADG1204_5_A0_GPIO_Port A0M5_GPIO_Port
#define ADG1204_5_A1_Pin A1M5_Pin
#define ADG1204_5_A1_GPIO_Port A1M5_GPIO_Port

	

void  ADG1204_5_init(void);
		//下面的是主要函数 上面的可以不用管	
void ADG1204_5_ChannelSelect(ADG1204_Channel_X_TypeDefE  Channel);



#ifdef CplusPlus
}
#endif

#endif

