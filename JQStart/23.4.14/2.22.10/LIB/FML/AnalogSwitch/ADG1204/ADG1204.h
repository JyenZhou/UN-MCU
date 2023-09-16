#ifndef  __ADG1204__H__
#define  __ADG1204__H__
#include "JHAL.h"
#ifdef __CplusPlus
extern "C" {
#endif

 
#define ADG1204_EN_Pin EN1_Pin
#define ADG1204_EN_GPIO_Port EN1_GPIO_Port
#define ADG1204_A0_Pin A0M1_Pin
#define ADG1204_A0_GPIO_Port A0M1_GPIO_Port
#define ADG1204_A1_Pin A1M1_Pin
#define ADG1204_A1_GPIO_Port A1M1_GPIO_Port


		
		
		


    typedef enum
    {
        ADG1204_Channel_NONE    = 101,//失能
         ADG1204_Channel_1    = 102,
         ADG1204_Channel_2    = 103,
         ADG1204_Channel_3      = 104,
         ADG1204_Channel_4     = 105

    } ADG1204_Channel_X_TypeDefE;
		
void  ADG1204_init(void);
		//下面的是主要函数 上面的可以不用管	
void ADG1204_ChannelSelect(ADG1204_Channel_X_TypeDefE  Channel);



#ifdef CplusPlus
}
#endif

#endif

