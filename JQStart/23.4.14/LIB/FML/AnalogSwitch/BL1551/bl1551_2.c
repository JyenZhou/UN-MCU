
/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*               	
 		ENB 1 得到   A1连接到公共端B
		ENB 0 得到   A2连接到公共端B
*                                                                     *
  *                                                                 *
   *                                                              *
*********@作者*Jyen******************@作者*Jyen***********************@作者*Jyen********************/




 /*------------------作者Jyen--------------------------作者Jyen-----------------------*/
/*******************************************************************************
*@ 函数功能或简介: 
  * @输入参数:   无
  * @返 回 值: 无
  * @重点说明: 无
  * @作者：Jyen Z
  *
  ******************************************************************************
  *
  *------------------作者Jyen--------------------------作者Jyen-------------------------*/
#include "bl1551_2.h"
void bl1551_2_init(void)
{
	#ifndef BL1551_2
    未初始化bl1551_2
#endif
 	 HAL_GPIO_WritePin(BL1551_2_EN_GPIO_Port,BL1551_2_EN_Pin,GPIO_PIN_SET);
	
	  GPIO_InitTypeDef GPIO_InitStruct;   
    GPIO_InitStruct.Pin = BL1551_2_EN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(BL1551_2_EN_GPIO_Port, &GPIO_InitStruct);
}

/*
*传进来1的时候 A1连接到公共端 否则A2连接到公共端
*/
void bl1551_2_BConnectedToA1(uint8 x )
{
	if(x==1)
	{
		 HAL_GPIO_WritePin(BL1551_2_EN_GPIO_Port,BL1551_2_EN_Pin,GPIO_PIN_SET);
	}else{
		 HAL_GPIO_WritePin(BL1551_2_EN_GPIO_Port,BL1551_2_EN_Pin,GPIO_PIN_RESET);
	}
	
}
