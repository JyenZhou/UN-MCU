#ifndef __ADG704_2__H__
#define  __ADG704_2__H__

//要用到它里面的结构体  不然同时用两个时候会出现重复定义
#include "ADG704.h"
#ifdef __CplusPlus
extern "C" {
#endif

#define ADG704_2_A0_Pin U_test_A0_Pin
#define ADG704_2_A0_GPIO_Port U_test_A0_GPIO_Port
#define ADG704_2_A1_Pin U_test_A1_Pin
#define ADG704_2_A1_GPIO_Port U_test_A1_GPIO_Port
#define ADG704_2_EN_Pin U_test_EN_Pin
#define ADG704_2_EN_GPIO_Port U_test_EN_GPIO_Port

//    typedef enum
//    {
//        NONE         = 0x00,//失能
//        ADG704_S1    = 0x01,
//        ADG704_S2    = 0x02,
//        ADG704_S3    = 0x03,
//        ADG704_S4    = 0x04,

//    } ADG704_X_TypeDefE;

    void ADG704_2_SEL(ADG704_X_TypeDefE Voltage);
    void  ADG704_2_init(void);





#ifdef CplusPlus
}
#endif

#endif


