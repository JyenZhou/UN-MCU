/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*                                                                     *
          		大多时候用不上  程序不用控制					 
*                                               
       通过电阻改变 输出电压  这个一般是电路上已经算好了   这个模块主要控制使能失能 甚至可以不控制 让他一直使能 还可能不经过单片机控制
                   
									计算方法0.5*（1+（R1/R2））见TPS7A7001手册10页*
  *                                                                 *
   *                                                              *
*********@作者*Jyen******************@作者*Jyen***********************@作者*Jyen********************/

#include "TPS7A7001.h"

/*-----------------------------------------------------------------------*/
/**
  *@ 函数功能或简介: 使能失能该模块
  * @输入参数:无                 
  * @返 回 值: 无
  * @重点说明：无
  * @作者：Jyen Z
  *
  ******************************************************************************
  *
  *----------------------作者Jyen---------------------作者Jyen--------------------------*/
	void TPS7A7001_EN (uint8 boolean)
	{
		if(boolean==1)
		{
		 HAL_GPIO_WritePin(TPS7A7001_EN_GPIO_Port,TPS7A7001_EN_Pin,GPIO_PIN_SET);
		}
		else{
			HAL_GPIO_WritePin(TPS7A7001_EN_GPIO_Port,TPS7A7001_EN_Pin,GPIO_PIN_RESET);
		}
	}
	
	
void TPS7A7001_Init (void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
  TPS7A7001_EN(0);
  GPIO_InitStruct.Pin = TPS7A7001_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(TPS7A7001_EN_GPIO_Port, &GPIO_InitStruct);
}
