
/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*                                                                     *
               TPS7A4700_2_RGW 在开漏模式下控制固定的电压输出  1.4-20.5 台阶1.0v
*                                                                     *
  *                                                                 *
   *                                                              *
*********@作者*Jyen******************@作者*Jyen***********************@作者*Jyen********************/



#include "TPS7A4700_RGWR_2.h"


/*函数封装*/


 

static 	void TPS7A4700_2_SEL_3V2_ENABLE(void)
{
    HAL_GPIO_WritePin(TPS7A4700_2_RWG_3V2_EN_GPIO_Port,TPS7A4700_2_RWG_3V2_EN_Pin,GPIO_PIN_RESET);
}
static 	void TPS7A4700_2_SEL_50mV_DISABLE(void)
{
    HAL_GPIO_WritePin(TPS7A4700_2_RWG_3V2_EN_GPIO_Port,TPS7A4700_2_RWG_3V2_EN_Pin,GPIO_PIN_SET);
}

static 	void TPS7A4700_2_SEL_1V6_ENABLE(void)
{
    HAL_GPIO_WritePin(TPS7A4700_2_RWG_1V6_EN_GPIO_Port,TPS7A4700_2_RWG_1V6_EN_Pin,GPIO_PIN_RESET);
}
static 	void TPS7A4700_2_SEL_1V6_DISABLE(void)
{
    HAL_GPIO_WritePin(TPS7A4700_2_RWG_1V6_EN_GPIO_Port,TPS7A4700_2_RWG_1V6_EN_Pin,GPIO_PIN_SET);
}

static 	void TPS7A4700_2_SEL_800mV_ENABLE(void)
{
    HAL_GPIO_WritePin(TPS7A4700_2_RWG_mVfor800_EN_GPIO_Port,TPS7A4700_2_RWG_mVfor800_EN_Pin,GPIO_PIN_RESET);
}
static 	void TPS7A4700_2_SEL_800mV_DISABLE(void)
{
    HAL_GPIO_WritePin(TPS7A4700_2_RWG_mVfor800_EN_GPIO_Port,TPS7A4700_2_RWG_mVfor800_EN_Pin,GPIO_PIN_SET);
}
static 	void TPS7A4700_2_SEL_400mV_ENABLE(void)
{
    HAL_GPIO_WritePin(TPS7A4700_2_RWG_mVfor400_EN_GPIO_Port,TPS7A4700_2_RWG_mVfor400_EN_Pin,GPIO_PIN_RESET);
}
static 	void TPS7A4700_2_SEL_400mV_DISABLE(void)
{
    HAL_GPIO_WritePin(TPS7A4700_2_RWG_mVfor400_EN_GPIO_Port,TPS7A4700_2_RWG_mVfor400_EN_Pin,GPIO_PIN_SET);
}

static 	void TPS7A4700_2_SEL_200mV_ENABLE(void)
{
    HAL_GPIO_WritePin(TPS7A4700_2_RWG_mVfor200_EN_GPIO_Port,TPS7A4700_2_RWG_mVfor200_EN_Pin,GPIO_PIN_RESET);
}
static 	void TPS7A4700_2_SEL_200mV_DISABLE(void)
{
    HAL_GPIO_WritePin(TPS7A4700_2_RWG_mVfor200_EN_GPIO_Port,TPS7A4700_2_RWG_mVfor200_EN_Pin,GPIO_PIN_SET);
}


static 	void TPS7A4700_2_SEL_100mV_ENABLE(void)
{
    HAL_GPIO_WritePin(TPS7A4700_2_RWG_mVfor100_EN_GPIO_Port,TPS7A4700_2_RWG_mVfor100_EN_Pin,GPIO_PIN_RESET);
}
static 	void TPS7A4700_2_SEL_100mV_DISABLE(void)
{
    HAL_GPIO_WritePin(TPS7A4700_2_RWG_mVfor100_EN_GPIO_Port,TPS7A4700_2_RWG_mVfor100_EN_Pin,GPIO_PIN_SET);
}




void TPS7A4700_2_RWG_init()
{

    GPIO_InitTypeDef GPIO_InitStruct;

    TPS7A4700_2_RWG_DISABLE;




    GPIO_InitStruct.Pin = TPS7A4700_2_RWG_mVfor100_EN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(TPS7A4700_2_RWG_mVfor100_EN_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = TPS7A4700_2_RWG_mVfor200_EN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(TPS7A4700_2_RWG_mVfor200_EN_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = TPS7A4700_2_RWG_mVfor400_EN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(TPS7A4700_2_RWG_mVfor400_EN_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = TPS7A4700_2_RWG_mVfor800_EN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(TPS7A4700_2_RWG_mVfor800_EN_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = TPS7A4700_2_RWG_1V6_EN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(TPS7A4700_2_RWG_1V6_EN_GPIO_Port, &GPIO_InitStruct);
		
		
    GPIO_InitStruct.Pin = TPS7A4700_2_RWG_3V2_EN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(TPS7A4700_2_RWG_3V2_EN_GPIO_Port, &GPIO_InitStruct);


    /*Configure GPIO pin : 使能 */

    GPIO_InitStruct.Pin = TPS7A4700_2_RWG_EN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(TPS7A4700_2_RWG_EN_Port, &GPIO_InitStruct);



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
static void TPS7A4700_2SelectReset()
{
    TPS7A4700_2_SEL_1V6_DISABLE();
    TPS7A4700_2_SEL_800mV_DISABLE();
    TPS7A4700_2_SEL_400mV_DISABLE();
    TPS7A4700_2_SEL_200mV_DISABLE();
    TPS7A4700_2_SEL_100mV_DISABLE();
    TPS7A4700_2_SEL_50mV_DISABLE();
    TPS7A4700_2_RWG_ENABLE;
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
uint8_t TPS7A4700_2SelectFunc(TPS7A4700_SELECT_TypeDef TPS7A4700_2_select)
{


    switch (TPS7A4700_2_select)
    {
    case TPS7A4700_SELECT_0V0  :
        TPS7A4700_2_RWG_DISABLE;

        break;
    case TPS7A4700_SELECT_1V4  :
        TPS7A4700_2SelectReset();

        break;
    case TPS7A4700_SELECT_1V5 :
        TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_100mV_ENABLE();

        break;
    case TPS7A4700_SELECT_1V6  :
        TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_200mV_ENABLE();
        break;
    case TPS7A4700_SELECT_1V7 :
        TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_200mV_ENABLE();
        TPS7A4700_2_SEL_100mV_ENABLE();
        break;
    case TPS7A4700_SELECT_1V8  :
        TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_400mV_ENABLE();

        break;
    case TPS7A4700_SELECT_1V9 :
        TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_400mV_ENABLE();
        TPS7A4700_2_SEL_100mV_ENABLE();

        break;
    case TPS7A4700_SELECT_2V0  :
        TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_400mV_ENABLE();
        TPS7A4700_2_SEL_200mV_ENABLE();

        break;
    case TPS7A4700_SELECT_2V1 :
        TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_400mV_ENABLE();
        TPS7A4700_2_SEL_200mV_ENABLE();
        TPS7A4700_2_SEL_100mV_ENABLE();

        break;
    case TPS7A4700_SELECT_2V2  :
        TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_800mV_ENABLE();
        break;
    case TPS7A4700_SELECT_2V3 :
        TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_800mV_ENABLE();
        TPS7A4700_2_SEL_100mV_ENABLE();
        break;
    case TPS7A4700_SELECT_2V4  :
        TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_800mV_ENABLE();
        TPS7A4700_2_SEL_200mV_ENABLE();
        break;
    case TPS7A4700_SELECT_2V5 :
        TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_800mV_ENABLE();
        TPS7A4700_2_SEL_200mV_ENABLE();
        TPS7A4700_2_SEL_100mV_ENABLE();
        break;
    case TPS7A4700_SELECT_2V6  :
        TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_800mV_ENABLE();
        TPS7A4700_2_SEL_400mV_ENABLE();
        break;
    case TPS7A4700_SELECT_2V7 :
        TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_800mV_ENABLE();
        TPS7A4700_2_SEL_400mV_ENABLE();
        TPS7A4700_2_SEL_100mV_ENABLE();
        break;
    case TPS7A4700_SELECT_2V8  :
        TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_800mV_ENABLE();
        TPS7A4700_2_SEL_400mV_ENABLE();
        TPS7A4700_2_SEL_200mV_ENABLE();
        break;
    case TPS7A4700_SELECT_2V9 :
        TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_800mV_ENABLE();
        TPS7A4700_2_SEL_200mV_ENABLE();
        TPS7A4700_2_SEL_100mV_ENABLE();

        break;
    case TPS7A4700_SELECT_3V0  :
        TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_1V6_ENABLE();
        break;
    case TPS7A4700_SELECT_3V1 :
        TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_1V6_ENABLE();
        TPS7A4700_2_SEL_100mV_ENABLE();
        break;
    case TPS7A4700_SELECT_3V2  :
        TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_1V6_ENABLE();
        TPS7A4700_2_SEL_200mV_ENABLE();

        break;
    case TPS7A4700_SELECT_3V3 :
        TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_1V6_ENABLE();
        TPS7A4700_2_SEL_200mV_ENABLE();
        TPS7A4700_2_SEL_100mV_ENABLE();
        break;
    case TPS7A4700_SELECT_3V4  :
        TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_1V6_ENABLE();
        TPS7A4700_2_SEL_400mV_ENABLE();

        break;
    case TPS7A4700_SELECT_3V5 :
        TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_1V6_ENABLE();

        TPS7A4700_2_SEL_400mV_ENABLE();

        TPS7A4700_2_SEL_100mV_ENABLE();
        break;
    case TPS7A4700_SELECT_3V6  :
        TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_1V6_ENABLE();
        TPS7A4700_2_SEL_400mV_ENABLE();
        TPS7A4700_2_SEL_200mV_ENABLE();

        break;
    case TPS7A4700_SELECT_3V7 :
        TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_1V6_ENABLE();
        TPS7A4700_2_SEL_400mV_ENABLE();
        TPS7A4700_2_SEL_200mV_ENABLE();
        TPS7A4700_2_SEL_100mV_ENABLE();
        break;
    case TPS7A4700_SELECT_3V8  :
			 TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_1V6_ENABLE();
        TPS7A4700_2_SEL_800mV_ENABLE();
        break;
    case TPS7A4700_SELECT_3V9 :
        TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_1V6_ENABLE();
        TPS7A4700_2_SEL_800mV_ENABLE();
        TPS7A4700_2_SEL_100mV_ENABLE();
        break;
    case TPS7A4700_SELECT_4V0  :
        TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_1V6_ENABLE();
        TPS7A4700_2_SEL_800mV_ENABLE();
        TPS7A4700_2_SEL_200mV_ENABLE();

        break;
    case TPS7A4700_SELECT_4V1 :
        TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_1V6_ENABLE();
        TPS7A4700_2_SEL_800mV_ENABLE();
        TPS7A4700_2_SEL_200mV_ENABLE();
        TPS7A4700_2_SEL_100mV_ENABLE();
        break;
    case TPS7A4700_SELECT_4V2  :
        TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_1V6_ENABLE();
        TPS7A4700_2_SEL_800mV_ENABLE();
        TPS7A4700_2_SEL_400mV_ENABLE();

        break;
    case TPS7A4700_SELECT_4V3 :
        TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_1V6_ENABLE();
        TPS7A4700_2_SEL_800mV_ENABLE();
        TPS7A4700_2_SEL_400mV_ENABLE();
        TPS7A4700_2_SEL_100mV_ENABLE();
        break;
    case TPS7A4700_SELECT_4V4  :
        TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_1V6_ENABLE();
        TPS7A4700_2_SEL_800mV_ENABLE();
        TPS7A4700_2_SEL_400mV_ENABLE();
        TPS7A4700_2_SEL_200mV_ENABLE();
		
        break;
    case TPS7A4700_SELECT_4V5 :
        TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_1V6_ENABLE();
        TPS7A4700_2_SEL_800mV_ENABLE();
        TPS7A4700_2_SEL_400mV_ENABLE();
        TPS7A4700_2_SEL_200mV_ENABLE();
        TPS7A4700_2_SEL_100mV_ENABLE();
        break;
    case TPS7A4700_SELECT_4V6  :
        TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_3V2_ENABLE();

        break;
    case TPS7A4700_SELECT_4V7 :
        TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_3V2_ENABLE();

        TPS7A4700_2_SEL_100mV_ENABLE();
        break;
    case TPS7A4700_SELECT_4V8  :
        TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_3V2_ENABLE();
        TPS7A4700_2_SEL_200mV_ENABLE();

        break;
    case TPS7A4700_SELECT_4V9 :
        TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_3V2_ENABLE();
        TPS7A4700_2_SEL_200mV_ENABLE();
        TPS7A4700_2_SEL_100mV_ENABLE();
        break;
    case TPS7A4700_SELECT_5V0  :
        TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_3V2_ENABLE();
        TPS7A4700_2_SEL_400mV_ENABLE();

        break;
    case TPS7A4700_SELECT_5V1 :
        TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_3V2_ENABLE();
        TPS7A4700_2_SEL_400mV_ENABLE();
        TPS7A4700_2_SEL_100mV_ENABLE();
        break;
    case TPS7A4700_SELECT_5V2  :
        TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_3V2_ENABLE();
        TPS7A4700_2_SEL_400mV_ENABLE();
        TPS7A4700_2_SEL_200mV_ENABLE();

        break;
    case TPS7A4700_SELECT_5V3 :
        TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_3V2_ENABLE();
        TPS7A4700_2_SEL_400mV_ENABLE();
        TPS7A4700_2_SEL_200mV_ENABLE();
        TPS7A4700_2_SEL_100mV_ENABLE();
        break;
    case TPS7A4700_SELECT_5V4  :
        TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_3V2_ENABLE();
        TPS7A4700_2_SEL_800mV_ENABLE();

        break;
    case TPS7A4700_SELECT_5V5 :
        TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_3V2_ENABLE();

        TPS7A4700_2_SEL_800mV_ENABLE();

        TPS7A4700_2_SEL_100mV_ENABLE();
        break;
    case TPS7A4700_SELECT_5V6  :
        TPS7A4700_2SelectReset();
        TPS7A4700_2_SEL_3V2_ENABLE();

        TPS7A4700_2_SEL_800mV_ENABLE();

        TPS7A4700_2_SEL_200mV_ENABLE();

        break;
    //用不上暂时就不写那么多了
    default:
        TPS7A4700_2_RWG_DISABLE;
        return 0;

    }
    if(TPS7A4700_2_select==TPS7A4700_SELECT_0V0)
        return 0;
    return 1;
}

































