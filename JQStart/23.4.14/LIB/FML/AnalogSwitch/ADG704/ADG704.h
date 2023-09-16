#ifndef __ADG704__H__
#define  __ADG704__H__
#include "JHAL.h"
#ifdef __CplusPlus
extern "C" {
#endif

#define ADG704_A0_Pin Uhot_A0_Pin
#define ADG704_A0_GPIO_Port Uhot_A0_GPIO_Port
#define ADG704_A1_Pin Uhot_A0D2_Pin
#define ADG704_A1_GPIO_Port Uhot_A0D2_GPIO_Port
#define ADG704_EN_Pin Uhot_EN_Pin
#define ADG704_EN_GPIO_Port Uhot_EN_GPIO_Port

    typedef enum
    {
        NONE         = 0x00,//Ê§ÄÜ
        ADG704_S1    = 0x01,
        ADG704_S2    = 0x02,
        ADG704_S3    = 0x03,
        ADG704_S4    = 0x04,

    } ADG704_X_TypeDefE;

    void ADG704_SEL(ADG704_X_TypeDefE Voltage);
    void  ADG704_init(void);





#ifdef CplusPlus
}
#endif

#endif


