
/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*                                                                     *
               TPS7A7100_2_RGW 在开漏模式下控制固定的电压输出  0.9-3.5   台阶0.5v
							  此芯片选择端在开漏模式下控制  高阻态时无输出 低电平时输出           （开漏模式下1为高阻态）
						总输出0.9V到3.5V 见datasheet 15页
						此芯片使能端 高电平使能 低电平失能 见datasheet 6页
*                                                                     *
  *                                                                 *
   *                                                              *
*********@作者*Jyen******************@作者*Jyen***********************@作者*Jyen********************/



#include "TPS7A7100_RGW_2.h"



/*函数封装*/




static void TPS7A7100_2_SEL_1V6_ENABLE(void)
{
    HAL_GPIO_WritePin( TPS7A7100_2_for_1V6_GPIO_Port, TPS7A7100_2_for_1V6_Pin,GPIO_PIN_RESET);
}


static void TPS7A7100_2_SEL_1V6_DISABLE(void)
{
    HAL_GPIO_WritePin( TPS7A7100_2_for_1V6_GPIO_Port, TPS7A7100_2_for_1V6_Pin,GPIO_PIN_SET);
}


static void TPS7A7100_2_SEL_800mV_ENABLE(void)
{
    HAL_GPIO_WritePin(TPS7A7100_2_mVfor800_EN_GPIO_Port,TPS7A7100_2_mVfor800_EN_Pin,GPIO_PIN_RESET);
}


static void TPS7A7100_2_SEL_800mV_DISABLE(void)
{
    HAL_GPIO_WritePin(TPS7A7100_2_mVfor800_EN_GPIO_Port,TPS7A7100_2_mVfor800_EN_Pin,GPIO_PIN_SET);
}


static void TPS7A7100_2_SEL_400mV_ENABLE(void)
{
    HAL_GPIO_WritePin(TPS7A7100_2_mVfor400_EN_GPIO_Port,TPS7A7100_2_mVfor400_EN_Pin,GPIO_PIN_RESET);
}


static void TPS7A7100_2_SEL_400mV_DISABLE(void)
{
    HAL_GPIO_WritePin(TPS7A7100_2_mVfor400_EN_GPIO_Port,TPS7A7100_2_mVfor400_EN_Pin,GPIO_PIN_SET);
}


static void TPS7A7100_2_SEL_200mV_ENABLE(void)
{
    HAL_GPIO_WritePin(TPS7A7100_2_mVfor200_EN_GPIO_Port,TPS7A7100_2_mVfor200_EN_Pin,GPIO_PIN_RESET);
}


static void TPS7A7100_2_SEL_200mV_DISABLE(void)
{
    HAL_GPIO_WritePin(TPS7A7100_2_mVfor200_EN_GPIO_Port,TPS7A7100_2_mVfor200_EN_Pin,GPIO_PIN_SET);
}


static void TPS7A7100_2_SEL_100mV_ENABLE(void)
{
    HAL_GPIO_WritePin(TPS7A7100_2_mVfor100_EN_GPIO_Port,TPS7A7100_2_mVfor100_EN_Pin,GPIO_PIN_RESET);
}


static void TPS7A7100_2_SEL_100mV_DISABLE(void)
{
    HAL_GPIO_WritePin(TPS7A7100_2_mVfor100_EN_GPIO_Port,TPS7A7100_2_mVfor100_EN_Pin,GPIO_PIN_SET);
}

static void TPS7A7100_2_SEL_50mV_ENABLE(void)
{
    HAL_GPIO_WritePin(TPS7A7100_2_mVfor50_EN_GPIO_Port,TPS7A7100_2_mVfor50_EN_Pin,GPIO_PIN_RESET);
}
static void TPS7A7100_2_SEL_50mV_DISABLE(void)
{
    HAL_GPIO_WritePin(TPS7A7100_2_mVfor50_EN_GPIO_Port,TPS7A7100_2_mVfor50_EN_Pin,GPIO_PIN_SET);
}








void TPS7A7100_2_RWG_init()
{
 TPS7A7100_2_RWG_DISABLE;

#ifndef TPS7A7100_RGW_2
    未初始化 TPS7A7100_2_RGW
#endif
#ifdef USE_HAL_DRIVER
    GPIO_InitTypeDef GPIO_InitStruct;
   


    GPIO_InitStruct.Pin = TPS7A7100_2_mVfor50_EN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(TPS7A7100_2_mVfor50_EN_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = TPS7A7100_2_mVfor100_EN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(TPS7A7100_2_mVfor100_EN_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = TPS7A7100_2_mVfor200_EN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(TPS7A7100_2_mVfor200_EN_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = TPS7A7100_2_mVfor400_EN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(TPS7A7100_2_mVfor400_EN_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = TPS7A7100_2_mVfor800_EN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(TPS7A7100_2_mVfor800_EN_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin =  TPS7A7100_2_EN_1_6V_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init( TPS7A7100_2_EN_1_6V_GPIO_Port, &GPIO_InitStruct);


    /*Configure GPIO pin : 使能 */

    GPIO_InitStruct.Pin = TPS7A7100_2_RWG_EN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(TPS7A7100_2_RWG_EN_Port, &GPIO_InitStruct);

#endif
#ifdef HC32_MODULE_ENABLED
 Gpio_InitIOExt(TPS7A7100_2_mVfor50_EN_GPIO_Port, TPS7A7100_2_mVfor50_EN_Pin, GpioDirOut, FALSE, FALSE, TRUE, FALSE);
 Gpio_InitIOExt(TPS7A7100_2_mVfor100_EN_GPIO_Port, TPS7A7100_2_mVfor100_EN_Pin, GpioDirOut, FALSE, FALSE, TRUE, FALSE);
 Gpio_InitIOExt(TPS7A7100_2_mVfor200_EN_GPIO_Port, TPS7A7100_2_mVfor200_EN_Pin, GpioDirOut, FALSE, FALSE, TRUE, FALSE);
 Gpio_InitIOExt(TPS7A7100_2_mVfor400_EN_GPIO_Port, TPS7A7100_2_mVfor400_EN_Pin, GpioDirOut, FALSE, FALSE, TRUE, FALSE);
 Gpio_InitIOExt(TPS7A7100_2_mVfor800_EN_GPIO_Port, TPS7A7100_2_mVfor800_EN_Pin, GpioDirOut, FALSE, FALSE, TRUE, FALSE);
 Gpio_InitIOExt(TPS7A7100_2_for_1V6_GPIO_Port,TPS7A7100_2_for_1V6_Pin, GpioDirOut, FALSE, FALSE, TRUE, FALSE);


    /*Configure GPIO pin : 使能 */
 Gpio_InitIOExt(TPS7A7100_2_RWG_EN_Port, TPS7A7100_2_RWG_EN_Pin, GpioDirOut, FALSE, FALSE, FALSE, FALSE);


#endif





}




/*-----------------------------------------------------------------------*/
/**
  *@ 函数功能或简介: 用来重置选择的
  * @输入参数:无
  * @返 回 值:
  * @重点说明：无
  * @作者：Jyen Z
  *
  ******************************************************************************
  *
  *----------------------作者Jyen---------------------作者Jyen--------------------------*/
static void TPS7A7100_2SelectReset()
{
    TPS7A7100_2_SEL_1V6_DISABLE();
    TPS7A7100_2_SEL_800mV_DISABLE();
    TPS7A7100_2_SEL_400mV_DISABLE();
    TPS7A7100_2_SEL_200mV_DISABLE();
    TPS7A7100_2_SEL_100mV_DISABLE();
    TPS7A7100_2_SEL_50mV_DISABLE();
    TPS7A7100_2_RWG_ENABLE;
}
/*-----------------------------------------------------------------------*/
/**
  *@ 函数功能或简介: 电压选择输出
  * @输入参数:枚举
  * @返 回 值:0无电压输出    1代表有电压输出
  * @重点说明：无
  * @作者：Jyen Z
  *
  ******************************************************************************
  *
  *----------------------作者Jyen---------------------作者Jyen--------------------------*/
uint8_t TPS7A7100_2SelectFunc(TPS7A7100_SELECT_TypeDef TPS7A7100_2_select)
{


    switch (TPS7A7100_2_select)
    {
case TPS7A7100_SELECT_0V0:
	  TPS7A7100_2SelectReset();
		TPS7A7100_2_RWG_DISABLE;
		break;

	case TPS7A7100_SELECT_0V5:
		TPS7A7100_2SelectReset();
		break;

	case TPS7A7100_SELECT_0V55:
		TPS7A7100_2SelectReset();
		TPS7A7100_2_SEL_50mV_ENABLE();

		break;

	case TPS7A7100_SELECT_0V6:
		TPS7A7100_2SelectReset();
		TPS7A7100_2_SEL_100mV_ENABLE();
		break;

	case TPS7A7100_SELECT_0V65:
		TPS7A7100_2SelectReset();
		TPS7A7100_2_SEL_100mV_ENABLE();
		TPS7A7100_2_SEL_50mV_ENABLE();
		break;

	case TPS7A7100_SELECT_0V7:
		TPS7A7100_2SelectReset();
		TPS7A7100_2_SEL_200mV_ENABLE();
		break;

	case TPS7A7100_SELECT_0V75:
		TPS7A7100_2SelectReset();
		TPS7A7100_2_SEL_50mV_ENABLE();
		TPS7A7100_2_SEL_200mV_ENABLE();
		break;

	case TPS7A7100_SELECT_0V8:
		TPS7A7100_2SelectReset();
		TPS7A7100_2_SEL_100mV_ENABLE();
		TPS7A7100_2_SEL_200mV_ENABLE();
		break;
	case TPS7A7100_SELECT_0V85:
		TPS7A7100_2SelectReset();
		TPS7A7100_2_SEL_50mV_ENABLE();
		TPS7A7100_2_SEL_100mV_ENABLE();
		TPS7A7100_2_SEL_200mV_ENABLE();
		break;
       
    case TPS7A7100_SELECT_0V9  :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_400mV_ENABLE();
        break;
    case TPS7A7100_SELECT_0V95 :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_400mV_ENABLE();
        TPS7A7100_2_SEL_50mV_ENABLE();
        break;
    case TPS7A7100_SELECT_1V0  :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_400mV_ENABLE();
        TPS7A7100_2_SEL_100mV_ENABLE();
        break;
    case TPS7A7100_SELECT_1V05 :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_400mV_ENABLE();
        TPS7A7100_2_SEL_100mV_ENABLE();
        TPS7A7100_2_SEL_50mV_ENABLE();
        break;
    case TPS7A7100_SELECT_1V1  :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_400mV_ENABLE();
        TPS7A7100_2_SEL_200mV_ENABLE();
        break;
    case TPS7A7100_SELECT_1V15 :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_400mV_ENABLE();
        TPS7A7100_2_SEL_200mV_ENABLE();
        TPS7A7100_2_SEL_50mV_ENABLE();
        break;
    case TPS7A7100_SELECT_1V2  :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_400mV_ENABLE();
        TPS7A7100_2_SEL_200mV_ENABLE();
        TPS7A7100_2_SEL_100mV_ENABLE();
        break;
    case TPS7A7100_SELECT_1V25 :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_400mV_ENABLE();
        TPS7A7100_2_SEL_200mV_ENABLE();
        TPS7A7100_2_SEL_100mV_ENABLE();
        TPS7A7100_2_SEL_50mV_ENABLE();
        break;
    case TPS7A7100_SELECT_1V3  :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_800mV_ENABLE();
        break;
    case TPS7A7100_SELECT_1V35 :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_800mV_ENABLE();
        TPS7A7100_2_SEL_50mV_ENABLE();
        break;
    case TPS7A7100_SELECT_1V4  :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_800mV_ENABLE();
        TPS7A7100_2_SEL_100mV_ENABLE();
        break;
    case TPS7A7100_SELECT_1V45 :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_800mV_ENABLE();
        TPS7A7100_2_SEL_100mV_ENABLE();
        TPS7A7100_2_SEL_50mV_ENABLE();
        break;
    case TPS7A7100_SELECT_1V5  :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_800mV_ENABLE();
        TPS7A7100_2_SEL_200mV_ENABLE();
        break;
    case TPS7A7100_SELECT_1V55 :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_800mV_ENABLE();
        TPS7A7100_2_SEL_200mV_ENABLE();
        TPS7A7100_2_SEL_50mV_ENABLE();
        break;
    case TPS7A7100_SELECT_1V6  :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_800mV_ENABLE();
        TPS7A7100_2_SEL_200mV_ENABLE();
        TPS7A7100_2_SEL_100mV_ENABLE();
        break;
    case TPS7A7100_SELECT_1V65 :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_800mV_ENABLE();
        TPS7A7100_2_SEL_200mV_ENABLE();
        TPS7A7100_2_SEL_100mV_ENABLE();
        TPS7A7100_2_SEL_50mV_ENABLE();
        break;
    case TPS7A7100_SELECT_1V7  :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_800mV_ENABLE();
        TPS7A7100_2_SEL_400mV_ENABLE();
        break;
    case TPS7A7100_SELECT_1V75 :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_800mV_ENABLE();
        TPS7A7100_2_SEL_400mV_ENABLE();
        TPS7A7100_2_SEL_50mV_ENABLE();
        break;
    case TPS7A7100_SELECT_1V8  :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_800mV_ENABLE();
        TPS7A7100_2_SEL_400mV_ENABLE();
        TPS7A7100_2_SEL_100mV_ENABLE();
        break;
    case TPS7A7100_SELECT_1V85 :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_800mV_ENABLE();
        TPS7A7100_2_SEL_400mV_ENABLE();
        TPS7A7100_2_SEL_100mV_ENABLE();
        TPS7A7100_2_SEL_50mV_ENABLE();
        break;
    case TPS7A7100_SELECT_1V9  :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_800mV_ENABLE();
        TPS7A7100_2_SEL_400mV_ENABLE();
        TPS7A7100_2_SEL_200mV_ENABLE();
        break;
    case TPS7A7100_SELECT_1V95 :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_800mV_ENABLE();
        TPS7A7100_2_SEL_400mV_ENABLE();
        TPS7A7100_2_SEL_200mV_ENABLE();
        TPS7A7100_2_SEL_50mV_ENABLE();
        break;
    case TPS7A7100_SELECT_2V0  :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_800mV_ENABLE();
        TPS7A7100_2_SEL_400mV_ENABLE();
        TPS7A7100_2_SEL_200mV_ENABLE();
        TPS7A7100_2_SEL_100mV_ENABLE();
        break;
    case TPS7A7100_SELECT_2V05 :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_800mV_ENABLE();
        TPS7A7100_2_SEL_400mV_ENABLE();
        TPS7A7100_2_SEL_200mV_ENABLE();
        TPS7A7100_2_SEL_100mV_ENABLE();
        TPS7A7100_2_SEL_50mV_ENABLE();
        break;
    case TPS7A7100_SELECT_2V1  :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_1V6_ENABLE();
        break;
    case TPS7A7100_SELECT_2V15 :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_1V6_ENABLE();
        TPS7A7100_2_SEL_50mV_ENABLE();
        break;
    case TPS7A7100_SELECT_2V2  :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_1V6_ENABLE();
        TPS7A7100_2_SEL_100mV_ENABLE();
        break;
    case TPS7A7100_SELECT_2V25 :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_1V6_ENABLE();
        TPS7A7100_2_SEL_100mV_ENABLE();
        TPS7A7100_2_SEL_50mV_ENABLE();
        break;
    case TPS7A7100_SELECT_2V3  :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_1V6_ENABLE();
        TPS7A7100_2_SEL_200mV_ENABLE();
        break;
    case TPS7A7100_SELECT_2V35 :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_1V6_ENABLE();
        TPS7A7100_2_SEL_200mV_ENABLE();
        TPS7A7100_2_SEL_50mV_ENABLE();
        break;
    case TPS7A7100_SELECT_2V4  :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_1V6_ENABLE();
        TPS7A7100_2_SEL_200mV_ENABLE();
        TPS7A7100_2_SEL_100mV_ENABLE();
        break;
    case TPS7A7100_SELECT_2V45 :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_1V6_ENABLE();
        TPS7A7100_2_SEL_200mV_ENABLE();
        TPS7A7100_2_SEL_100mV_ENABLE();
        TPS7A7100_2_SEL_50mV_ENABLE();
        break;
    case TPS7A7100_SELECT_2V5  :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_1V6_ENABLE();
        TPS7A7100_2_SEL_400mV_ENABLE();
        break;
    case TPS7A7100_SELECT_2V55 :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_1V6_ENABLE();
        TPS7A7100_2_SEL_400mV_ENABLE();
        TPS7A7100_2_SEL_50mV_ENABLE();
        break;
    case TPS7A7100_SELECT_2V6  :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_1V6_ENABLE();
        TPS7A7100_2_SEL_400mV_ENABLE();
        TPS7A7100_2_SEL_100mV_ENABLE();
        break;
    case TPS7A7100_SELECT_2V65 :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_1V6_ENABLE();
        TPS7A7100_2_SEL_400mV_ENABLE();
        TPS7A7100_2_SEL_100mV_ENABLE();
        TPS7A7100_2_SEL_50mV_ENABLE();
        break;
    case TPS7A7100_SELECT_2V7  :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_1V6_ENABLE();
        TPS7A7100_2_SEL_400mV_ENABLE();
        TPS7A7100_2_SEL_200mV_ENABLE();
        break;
    case TPS7A7100_SELECT_2V75 :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_1V6_ENABLE();
        TPS7A7100_2_SEL_400mV_ENABLE();
        TPS7A7100_2_SEL_200mV_ENABLE();
        TPS7A7100_2_SEL_50mV_ENABLE();
        break;
    case TPS7A7100_SELECT_2V8  :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_1V6_ENABLE();
        TPS7A7100_2_SEL_400mV_ENABLE();
        TPS7A7100_2_SEL_200mV_ENABLE();
        TPS7A7100_2_SEL_100mV_ENABLE();
        break;
    case TPS7A7100_SELECT_2V85 :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_1V6_ENABLE();
        TPS7A7100_2_SEL_400mV_ENABLE();
        TPS7A7100_2_SEL_200mV_ENABLE();
        TPS7A7100_2_SEL_100mV_ENABLE();
        TPS7A7100_2_SEL_50mV_ENABLE();
        break;
    case TPS7A7100_SELECT_2V9  :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_1V6_ENABLE();
        TPS7A7100_2_SEL_800mV_ENABLE();
        break;
    case TPS7A7100_SELECT_2V95 :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_1V6_ENABLE();
        TPS7A7100_2_SEL_800mV_ENABLE();
        TPS7A7100_2_SEL_50mV_ENABLE();
        break;
    case TPS7A7100_SELECT_3V0  :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_1V6_ENABLE();
        TPS7A7100_2_SEL_800mV_ENABLE();
        TPS7A7100_2_SEL_100mV_ENABLE();
        break;
    case TPS7A7100_SELECT_3V05 :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_1V6_ENABLE();
        TPS7A7100_2_SEL_800mV_ENABLE();
        TPS7A7100_2_SEL_100mV_ENABLE();
        TPS7A7100_2_SEL_50mV_ENABLE();
        break;
    case TPS7A7100_SELECT_3V1  :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_1V6_ENABLE();
        TPS7A7100_2_SEL_800mV_ENABLE();
        TPS7A7100_2_SEL_200mV_ENABLE();
        break;
    case TPS7A7100_SELECT_3V15 :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_1V6_ENABLE();
        TPS7A7100_2_SEL_800mV_ENABLE();
        TPS7A7100_2_SEL_200mV_ENABLE();
        TPS7A7100_2_SEL_50mV_ENABLE();
        break;
    case TPS7A7100_SELECT_3V2  :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_1V6_ENABLE();
        TPS7A7100_2_SEL_800mV_ENABLE();
        TPS7A7100_2_SEL_200mV_ENABLE();
        TPS7A7100_2_SEL_100mV_ENABLE();
        break;
    case TPS7A7100_SELECT_3V25 :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_1V6_ENABLE();
        TPS7A7100_2_SEL_800mV_ENABLE();
        TPS7A7100_2_SEL_200mV_ENABLE();
        TPS7A7100_2_SEL_100mV_ENABLE();
        TPS7A7100_2_SEL_50mV_ENABLE();
        break;
    case TPS7A7100_SELECT_3V3  :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_1V6_ENABLE();
        TPS7A7100_2_SEL_800mV_ENABLE();
        TPS7A7100_2_SEL_400mV_ENABLE();
        break;
    case TPS7A7100_SELECT_3V35 :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_1V6_ENABLE();
        TPS7A7100_2_SEL_800mV_ENABLE();
        TPS7A7100_2_SEL_400mV_ENABLE();
        TPS7A7100_2_SEL_50mV_ENABLE();
        break;
    case TPS7A7100_SELECT_3V4  :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_1V6_ENABLE();
        TPS7A7100_2_SEL_800mV_ENABLE();
        TPS7A7100_2_SEL_400mV_ENABLE();
        TPS7A7100_2_SEL_100mV_ENABLE();
        break;
    case TPS7A7100_SELECT_3V45 :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_1V6_ENABLE();
        TPS7A7100_2_SEL_800mV_ENABLE();
        TPS7A7100_2_SEL_400mV_ENABLE();
        TPS7A7100_2_SEL_100mV_ENABLE();
        TPS7A7100_2_SEL_50mV_ENABLE();
        break;
    case TPS7A7100_SELECT_3V5  :
        TPS7A7100_2SelectReset();
        TPS7A7100_2_SEL_1V6_ENABLE();
        TPS7A7100_2_SEL_800mV_ENABLE();
        TPS7A7100_2_SEL_400mV_ENABLE();
        TPS7A7100_2_SEL_200mV_ENABLE();
        break;
    default:
        TPS7A7100_2_RWG_DISABLE;
        return 0;

    }
    if(TPS7A7100_2_select==TPS7A7100_SELECT_0V0)
        return 0;
    return 1;
}

































