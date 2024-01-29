#ifndef __LED_H
#define __LED_H

#include "main.h"

#define LED_SYS_Port  GPIOA
#define LED_SYS_Pin   GPIO_Pin_11

#define LED_SYS_ON  			GPIO_ResetBits(LED_SYS_Port,LED_SYS_Pin)
#define LED_SYS_OFF  			GPIO_SetBits(LED_SYS_Port,LED_SYS_Pin)

#define LED_SYS_TOGGLE  	(GPIO_ReadOutputDataBit(LED_SYS_Port,LED_SYS_Pin))?(GPIO_ResetBits(LED_SYS_Port,LED_SYS_Pin)):(GPIO_SetBits(LED_SYS_Port,LED_SYS_Pin))

typedef	enum {LED_OFF = 0, LED_ON,	LED_FLASH_1HZ,	LED_FLASH_5HZ}led_sta_t;

typedef struct
{
	led_sta_t led_sta;
}	led_t;

extern led_t led_sys;

void 			Led_Init(void);
uint32_t  led_sys_task(void);

#endif
