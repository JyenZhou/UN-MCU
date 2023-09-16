#ifndef  __ADG1204_4__H__
#define  __ADG1204_4__H__
#include "ADG1204.h"
#ifdef __CplusPlus
extern "C" {
#endif

 
#define ADG1204_4_EN_Pin EN4_Pin
#define ADG1204_4_EN_GPIO_Port EN4_GPIO_Port
#define ADG1204_4_A0_Pin A0M4_Pin
#define ADG1204_4_A0_GPIO_Port A0M4_GPIO_Port
#define ADG1204_4_A1_Pin A1M4_Pin
#define ADG1204_4_A1_GPIO_Port A1M4_GPIO_Port


		
		
		


    typedef enum
    {
        ADG1204_4_Channel_NONE    = 101,//失能
         ADG1204_4_Channel_1    = 102,
         ADG1204_4_Channel_2    = 103,
         ADG1204_4_Channel_3      = 104,
         ADG1204_4_Channel_4     = 105

    } ADG1204_4_Channel_X_TypeDefE;
		
void  ADG1204_4_init(void);
		//下面的是主要函数 上面的可以不用管	
void ADG1204_4_ChannelSelect(ADG1204_Channel_X_TypeDefE  Channel);



#ifdef CplusPlus
}
#endif

#endif

