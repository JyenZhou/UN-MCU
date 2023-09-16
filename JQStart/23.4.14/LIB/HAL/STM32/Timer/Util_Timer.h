#ifdef UTTIL_TIMER
#ifndef __UTIL_TIMER_H__
#define __UTIL_TIMER_H__

#include "Hall_includes.h"
 
/***
使能指定定时器
***/
#define UTIL_TIMER1_EN
/****************************/





#ifdef    UTIL_TIMER1_EN
extern TIM_HandleTypeDef htim1;
#endif

void utilTimer_Init(void);




#endif
#endif

