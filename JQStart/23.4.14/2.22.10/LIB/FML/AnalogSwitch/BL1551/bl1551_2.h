
#ifndef __BL1551_2__H__
#define  __BL1551_2__H__
#include "JHAL.h"
#ifdef __CplusPlus
extern "C" {
#endif

#define BL1551_2_EN_Pin  Uh_SW1_Pin
#define BL1551_2_EN_GPIO_Port Uh_SW1_GPIO_Port
void bl1551_2_init(void);
void bl1551_2_BConnectedToA1(uint8 x );





#ifdef CplusPlus
}
#endif

#endif


