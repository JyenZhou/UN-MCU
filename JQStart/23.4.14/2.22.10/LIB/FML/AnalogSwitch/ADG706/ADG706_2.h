#ifndef __ADG706_2_2__H__
#define  __ADG706_2_2__H__
//要用到它里面的结构体  不然同时用两个时候会出现重复定义
#include "ADG706.h"
#ifdef __CplusPlus
extern "C" {
#endif

#define ADG706_2_A0_Pin RL2_A0_Pin
#define ADG706_2_A0_GPIO_Port RL2_A0_GPIO_Port
#define ADG706_2_A1_Pin RL2_A1_Pin
#define ADG706_2_A1_GPIO_Port RL2_A1_GPIO_Port
#define ADG706_2_A2_Pin RL2_A2_Pin
#define ADG706_2_A2_GPIO_Port RL2_A2_GPIO_Port
#define ADG706_2_A3_Pin RL2_A3_Pin
#define ADG706_2_A3_GPIO_Port RL2_A3_GPIO_Port
#define ADG706_2_EN_Pin RL2_EN_Pin
#define ADG706_2_EN_GPIO_Port RL2_EN_GPIO_Port


    void ADG706_2_SEL(ADG706_X_TypeDefE ADG706_2_SX);
    void  ADG706_2_init(void);





#ifdef CplusPlus
}
#endif

#endif


