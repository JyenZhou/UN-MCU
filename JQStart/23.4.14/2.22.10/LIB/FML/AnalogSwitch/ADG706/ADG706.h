#ifndef __ADG706__H__
#define  __ADG706__H__
#include "JHAL.h"
#ifdef __CplusPlus
extern "C" {
#endif

#define ADG706_A0_Pin RL1_A0_Pin
#define ADG706_A0_GPIO_Port RL1_A0_GPIO_Port
#define ADG706_A1_Pin RL1_A1_Pin
#define ADG706_A1_GPIO_Port RL1_A1_GPIO_Port
#define ADG706_A2_Pin RL1_A2_Pin
#define ADG706_A2_GPIO_Port RL1_A2_GPIO_Port
#define ADG706_A3_Pin RL1_A3_Pin
#define ADG706_A3_GPIO_Port RL1_A3_GPIO_Port
#define ADG706_EN_Pin RL1_EN_Pin
#define ADG706_EN_GPIO_Port RL1_EN_GPIO_Port

    typedef enum
    {
        NONE         = 0x00,//Ê§ÄÜ
        ADG706_S1    = 0x01,
        ADG706_S2    = 0x02,
        ADG706_S3    = 0x03,
        ADG706_S4    = 0x04,
        ADG706_S5    = 0x05,
        ADG706_S6    = 0x06,
        ADG706_S7    = 0x07,
        ADG706_S8    = 0x08,
        ADG706_S9    = 0x09,
        ADG706_S10    = 0x0A,
        ADG706_S11    = 0x0B,
        ADG706_S12    = 0x0C,
        ADG706_S13    = 0x0D,
        ADG706_S14    = 0x0E,
        ADG706_S15    = 0x0F,
        ADG706_S16    = 0x10,
    } ADG706_X_TypeDefE;

    void ADG706_SEL(ADG706_X_TypeDefE ADG706_SX);
    void  ADG706_init(void);





#ifdef CplusPlus
}
#endif

#endif


