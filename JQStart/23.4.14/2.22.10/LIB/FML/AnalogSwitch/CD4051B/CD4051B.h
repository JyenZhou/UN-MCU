#ifndef  __CD4051BBM96__H__
#define  __CD4051BBM96__H__
#include "JHAL.h"
#ifdef __CplusPlus
extern "C" {
#endif



#define CD4051B_EN_Pin VCC_EN_Pin
#define CD4051B_EN_GPIO_Port VCC_EN_GPIO_Port
#define CD4051B_A_Pin VCC_A_Pin
#define CD4051B_A_GPIO_Port VCC_A_GPIO_Port
#define CD4051B_B_Pin VCC_B_Pin
#define CD4051B_B_GPIO_Port VCC_B_GPIO_Port
#define CD4051B_C_Pin VCC_C_Pin
#define CD4051B_C_GPIO_Port VCC_C_GPIO_Port
		


    typedef enum
    {
        NONE    = 101,//失能
        Channel_1    = 102,
        Channel_2    = 103,
        Channel_3      = 104,
        Channel_4     = 105,
        Channel_5    =  106,
        Channel_6      = 107,
        Channel_7     = 108,
        Channel_8     = 109


    } CD4051B_Channel_X_TypeDefE;
		
void  CD4051B_init(void);
		//下面的是主要函数 上面的可以不用管	
void CD4051B_ChannelSelect(CD4051B_Channel_X_TypeDefE  Channel);



#ifdef CplusPlus
}
#endif

#endif








