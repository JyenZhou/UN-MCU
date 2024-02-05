/*****************************************************************************************************
   *
	 *
 *
 *
*                      按照电路上设计  配合 TPS7A7001.C文件控制                                                  *
            用ADG704切换通道的函数
          真值表
   A1  A0   EN   ON Switch
		X   X		0			NONE
		0		0		1				s1
		0		1		1				s2
		1		0		1				s3
		1		1		1				s4
*                                                                     *
  通过控制 A1 A0 组合 控制输出                                                                *

*
*********@作者*Jyen******************@作者*Jyen***********************@作者*Jyen********************/
#include "ADG704.h"



/*使能该模块*/
static void ADG704_EN(uint8 boolean)
{
    if(boolean==1)
    {
        HAL_GPIO_WritePin(ADG704_EN_GPIO_Port,ADG704_EN_Pin,GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(ADG704_EN_GPIO_Port,ADG704_EN_Pin,GPIO_PIN_RESET);
    }
}
/*打开开关S1*/
static 	void openADG704_S1()
{
    ADG704_EN(1);
    HAL_GPIO_WritePin(ADG704_A0_GPIO_Port,ADG704_A0_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ADG704_A1_GPIO_Port,ADG704_A1_Pin,GPIO_PIN_RESET);
}
static void openADG704_S2()
{
    ADG704_EN(1);
    HAL_GPIO_WritePin(ADG704_A0_GPIO_Port,ADG704_A0_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ADG704_A1_GPIO_Port,ADG704_A1_Pin,GPIO_PIN_SET);
}
static	void openADG704_S3()
{
    ADG704_EN(1);
    HAL_GPIO_WritePin(ADG704_A0_GPIO_Port,ADG704_A0_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(ADG704_A1_GPIO_Port,ADG704_A1_Pin,GPIO_PIN_RESET);
}
static	void openADG704_S4()
{
    ADG704_EN(1);
    HAL_GPIO_WritePin(ADG704_A0_GPIO_Port,ADG704_A0_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(ADG704_A1_GPIO_Port,ADG704_A1_Pin,GPIO_PIN_SET);
}

/*-----------------------------------------------------------------------*/
/**
  *@ 函数功能或简介: 传感器1的电压设置
  * @输入参数:无
  * @返 回 值: 无
  * @重点说明：无
  * @作者：Jyen Z
  *
  ******************************************************************************
  *
  *----------------------作者Jyen---------------------作者Jyen--------------------------*/
void   ADG704_SEL(ADG704_X_TypeDefE ADG704_SX)
{
    switch (ADG704_SX)
    {
    case NONE:
        ADG704_EN(0);
        break;

    case ADG704_S1  :
        openADG704_S1();
        break;

    case ADG704_S2  :
        openADG704_S2();
        break;


    case ADG704_S3  :
        openADG704_S3();
        break;


    case ADG704_S4  :
        openADG704_S4();
        break;

    default:
			  ADG704_EN(0);
        break;
    }
}




void  ADG704_init()
{
#ifndef ADG704
  未初始化ADG704
#endif
    ADG704_EN(0);
    GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = ADG704_EN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(ADG704_EN_GPIO_Port, &GPIO_InitStruct);
	
    GPIO_InitStruct.Pin = ADG704_A0_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(ADG704_A0_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = ADG704_A1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(ADG704_A1_GPIO_Port, &GPIO_InitStruct);


}


