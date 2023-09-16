#ifndef  __TPS7A7001__H__
#define  __TPS7A7001__H__
#include "JHAL.h"
#ifdef __CplusPlus
extern "C" {
#endif

#define TPS7A7001_EN_Pin GPIO_PIN_12
#define TPS7A7001_EN_GPIO_Port GPIOC


void TPS7A7001_Init (void);
	void TPS7A7001_EN (uint8 boolean);

#ifdef CplusPlus
}
#endif

#endif


