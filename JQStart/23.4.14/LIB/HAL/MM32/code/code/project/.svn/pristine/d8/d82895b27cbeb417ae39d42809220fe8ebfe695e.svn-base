#define _LDO_C_

/*
LDO说明：
使用方法同LED文件
*/

#include "ldo.h"

ldo_t ldo_sw;

void Ldo_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA, ENABLE);
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  =  LDO_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LDO_Port, &GPIO_InitStructure);

    LDO_OFF;
	
		ldo_sw.ldo_sta = LDO_DISABLE;
}

uint32_t  ldo_task(void){
_SS
  while(UERROR){
		if(ldo_sw.ldo_sta == LDO_ENABLE)
		{
			LDO_ON;
			WaitX(100); 
		}
		else
		{
			LDO_OFF;
			WaitX(100); 
		}
  }
_EE
}


