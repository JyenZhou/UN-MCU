#include "ADG1204_3.h"

/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*              
*
\
       对ADG1204B通道切换的操作做了个整合
                 ADG1204B 真值表


	               A1   A0   所开通道
	               0   0  	 0
		             0   1  	 1
					       1	0	 		 	2
								1		1		    3

	                   使能位1 失能所有


           
*                                                                     *
  *                                                                 *
   *                                                              *
*********@作者*Jyen******************@作者*Jyen***********************@作者*Jyen********************/
 

static 	void ADG1204_EN_IO(uint8 boolean)
{
	if(boolean==1)
	{
		 HAL_GPIO_WritePin(ADG1204_3_EN_GPIO_Port,ADG1204_3_EN_Pin,GPIO_PIN_SET);
	}else{
			  HAL_GPIO_WritePin(ADG1204_3_EN_GPIO_Port,ADG1204_3_EN_Pin,GPIO_PIN_RESET);
	}
}


static 	void openADG1204_3_Channel1()
{ADG1204_EN_IO(1);
			HAL_GPIO_WritePin(ADG1204_3_A0_GPIO_Port,ADG1204_3_A0_Pin,GPIO_PIN_RESET);
HAL_GPIO_WritePin(ADG1204_3_A1_GPIO_Port,ADG1204_3_A1_Pin,GPIO_PIN_RESET);
 
}
static 	void openADG1204_3_Channel2()
{ADG1204_EN_IO(1);
			HAL_GPIO_WritePin(ADG1204_3_A0_GPIO_Port,ADG1204_3_A0_Pin,GPIO_PIN_SET);
HAL_GPIO_WritePin(ADG1204_3_A1_GPIO_Port,ADG1204_3_A1_Pin,GPIO_PIN_RESET);

}
static 	void openADG1204_3_Channel3()
{ADG1204_EN_IO(1);
	HAL_GPIO_WritePin(ADG1204_3_A0_GPIO_Port,ADG1204_3_A0_Pin,GPIO_PIN_RESET);
HAL_GPIO_WritePin(ADG1204_3_A1_GPIO_Port,ADG1204_3_A1_Pin,GPIO_PIN_SET);	

}
static 	void openADG1204_3_Channel4()
{
	ADG1204_EN_IO(1);
			HAL_GPIO_WritePin(ADG1204_3_A0_GPIO_Port,ADG1204_3_A0_Pin,GPIO_PIN_SET);
HAL_GPIO_WritePin(ADG1204_3_A1_GPIO_Port,ADG1204_3_A1_Pin,GPIO_PIN_SET);

}
/*-----------------------------------------------------------------------*/
/**
  *@ 函数功能或简介: 使能GAS_X中某个通道
  * @输入参数:无      枚举类 GAS1_X GAS1_0-GAS1_8
  * @返 回 值: 无
  * @重点说明：GAS1_0代表失能关闭
  * @作者：Jyen Z
  *
  ******************************************************************************
  *
  *----------------------作者Jyen---------------------作者Jyen--------------------------*/
void ADG1204_3_ChannelSelect(ADG1204_Channel_X_TypeDefE  Channel)
{
    switch (Channel)
    {
    case ADG1204_Channel_NONE:
        ADG1204_EN_IO(0);
        break;
    case ADG1204_Channel_1:
			openADG1204_3_Channel1();
        break;
    case ADG1204_Channel_2:
				openADG1204_3_Channel2();
        break;
    case ADG1204_Channel_3:
				openADG1204_3_Channel3();
        break;
		
    case ADG1204_Channel_4:
				openADG1204_3_Channel4();
        break;
    
    default:
        break;
    }
}
void  ADG1204_3_init()
{
#ifndef ADG1204
  //未初始化ADG1204
#endif
	 ADG1204_EN_IO(0);
	GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = ADG1204_3_EN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(ADG1204_3_EN_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = ADG1204_3_A0_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(ADG1204_3_A0_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = ADG1204_3_A1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(ADG1204_3_A1_GPIO_Port, &GPIO_InitStruct);
	

	
}

