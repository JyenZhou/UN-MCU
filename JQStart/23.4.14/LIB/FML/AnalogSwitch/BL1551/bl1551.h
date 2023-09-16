
#ifndef __BL1551__H__
#define  __BL1551__H__
#include "JHAL.h"
#ifdef __CplusPlus
extern "C" {
#endif


#define BL1551_EN_Pin  Uc_SW1_Pin
#define BL1551_EN_GPIO_Port Uc_SW1_GPIO_Port
void bl1551_init(void);
void bl1551_BConnectedToA1(uint8 x);





#ifdef CplusPlus
}
#endif

#endif


