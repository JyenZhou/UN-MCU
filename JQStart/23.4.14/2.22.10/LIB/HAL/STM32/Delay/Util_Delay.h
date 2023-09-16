#ifndef __HAL_DELAYM__H__
#define  __HAL_DELAYM__H__ 
#include <hall_sys_includes.h>



//********************************************************************************
//delay_intnesting改为：delay_osintnesting
////////////////////////////////////////////////////////////////////////////////// 
	 //初始化
void delay_init(u8 SYSCLK);
//n毫秒延时
void delay_ms(u32 nms);
//n微秒延时
void delay_us(u32 nus);

#endif


















