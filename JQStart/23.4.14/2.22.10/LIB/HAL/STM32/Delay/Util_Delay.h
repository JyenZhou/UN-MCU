#ifndef __HAL_DELAYM__H__
#define  __HAL_DELAYM__H__ 
#include <hall_sys_includes.h>



//********************************************************************************
//delay_intnesting��Ϊ��delay_osintnesting
////////////////////////////////////////////////////////////////////////////////// 
	 //��ʼ��
void delay_init(u8 SYSCLK);
//n������ʱ
void delay_ms(u32 nms);
//n΢����ʱ
void delay_us(u32 nus);

#endif


















