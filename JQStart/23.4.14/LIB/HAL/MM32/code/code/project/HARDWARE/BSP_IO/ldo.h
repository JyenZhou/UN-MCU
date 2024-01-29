#ifndef __LDO_H
#define __LDO_H

#include "main.h"

#define LDO_Port  GPIOA
#define LDO_Pin   GPIO_Pin_0

#define LDO_OFF  			GPIO_ResetBits(LDO_Port,LDO_Pin)
#define LDO_ON  			GPIO_SetBits(LDO_Port,LDO_Pin)

typedef	enum {LDO_ENABLE = 0, LDO_DISABLE}ldo_sta_t;

typedef struct
{
	ldo_sta_t ldo_sta;
}	ldo_t;

extern ldo_t ldo_sw;

void 			Ldo_Init(void);
uint32_t  ldo_task(void);

#endif
