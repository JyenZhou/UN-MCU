#ifndef __RADIO_KEY_H__
#define __RADIO_KEY_H__
#include "JHAL.h"
#ifdef __cplusplus
extern "C" {
#endif


#define K_S1_Pin GPIO_PIN_0
#define K_S1_GPIO_GPIO_Port GPIOC



#define K_S2_Pin GPIO_PIN_1
#define K_S2GPIO_Port GPIOC
#define K_S2_EXTI_IRQn EXTI1_IRQn

#define K_S3_Pin GPIO_PIN_2
#define K_S3_GPIO_Port GPIOC
#define K_S3_EXTI_IRQn EXTI2_IRQn

#define K_S4_Pin GPIO_PIN_3
#define K_S4_GPIO_Port GPIOC
#define K_S4_EXTI_IRQn EXTI3_IRQn


 
void radioKey_GPIOInt (void);

#ifdef __cplusplus
}
#endif

#endif

