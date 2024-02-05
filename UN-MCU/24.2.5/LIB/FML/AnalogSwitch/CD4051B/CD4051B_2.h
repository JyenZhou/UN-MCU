#ifndef  __CD4051BBM96_2_H__
#define  __CD4051BBM96_2_H__
#ifdef __CplusPlus
extern "C" {
#endif
//要用到它里面的结构体  不然同时用两个时候会出现重复定义
#include "CD4051B.h"

#define CD4051B_2_EN_Pin VL_EN_Pin
#define CD4051B_2_EN_GPIO_Port VL_EN_GPIO_Port
#define CD4051B_2_A_Pin VL_A_Pin
#define CD4051B_2_A_GPIO_Port VL_A_GPIO_Port
#define CD4051B_2_B_Pin VL_B_Pin
#define CD4051B_2_B_GPIO_Port VL_B_GPIO_Port
#define CD4051B_2_C_Pin VL_C_Pin
#define CD4051B_2_C_GPIO_Port VL_C_GPIO_Port


//    typedef enum
//    {
//        NONE    = 101,//失能
//        Channel_1    = 102,
//        Channel_2    = 103,
//        Channel_3      = 104,
//        Channel_4     = 105,
//        Channel_5    =  106,
//        Channel_6      = 107,
//        Channel_7     = 108,
//        Channel_8     = 109


//    } CD4051B_Channel_X_TypeDefE;
void  CD4051B_2_init(void);
		//下面的是主要函数 上面的可以不用管	
void CD4051B_2_ChannelSelect(CD4051B_Channel_X_TypeDefE  Channel);



#ifdef CplusPlus
}
#endif

#endif








