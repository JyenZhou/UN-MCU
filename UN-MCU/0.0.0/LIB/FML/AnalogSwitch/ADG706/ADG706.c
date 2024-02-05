/*****************************************************************************************************
   *
	 *
 *
 *
*                                                                     *
            ��ADG706�л�ͨ���ĺ���
          ��ֵ��
A3 A2 A1  A0   EN   ON Switch
X		X		X   X		0			NONE
0		0		0		0		1				s1
0		0		0		1		1				s2
0		0		1		0		1				s3
0		0		1		1		1				s4
0		1		0		0		1				s5
0		1		0		1		1				s6
0		1		1		0		1				s7
0		1		1		1		1				s8
1		0		0		0		1				s9
1		0		0		1		1				s10
1		0		1		0		1				s11
1		0		1		1		1				s12
1		1		0		0		1				s13
1		1		0		1		1				s14
1		1		1		0		1				s15
1		1		1		1		1				s16
*                                                                     *
  ͨ������ A1 A0 A2 A3��� �������                                                                *

*
*********@����*Jyen******************@����*Jyen***********************@����*Jyen********************/
#include "ADG706.h"



/*ʹ�ܸ�ģ��*/
static void ADG706_EN(uint8 boolean)
{
    if(boolean==1)
    {
        HAL_GPIO_WritePin(ADG706_EN_GPIO_Port,ADG706_EN_Pin,GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(ADG706_EN_GPIO_Port,ADG706_EN_Pin,GPIO_PIN_RESET);
    }
}
/*�򿪿���S1*/
static 	void openADG706_S1()
{
    ADG706_EN(1);
    HAL_GPIO_WritePin(ADG706_A0_GPIO_Port,ADG706_A0_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ADG706_A1_GPIO_Port,ADG706_A1_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ADG706_A2_GPIO_Port,ADG706_A2_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ADG706_A3_GPIO_Port,ADG706_A3_Pin,GPIO_PIN_RESET);
}
static void openADG706_S2()
{
    ADG706_EN(1);
    HAL_GPIO_WritePin(ADG706_A0_GPIO_Port,ADG706_A0_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(ADG706_A1_GPIO_Port,ADG706_A1_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ADG706_A2_GPIO_Port,ADG706_A2_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ADG706_A3_GPIO_Port,ADG706_A3_Pin,GPIO_PIN_RESET);

}
static	void openADG706_S3()
{
    ADG706_EN(1);
    HAL_GPIO_WritePin(ADG706_A0_GPIO_Port,ADG706_A0_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ADG706_A1_GPIO_Port,ADG706_A1_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(ADG706_A2_GPIO_Port,ADG706_A2_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ADG706_A3_GPIO_Port,ADG706_A3_Pin,GPIO_PIN_RESET);
}
static	void openADG706_S4()
{
    ADG706_EN(1);
    HAL_GPIO_WritePin(ADG706_A0_GPIO_Port,ADG706_A0_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(ADG706_A1_GPIO_Port,ADG706_A1_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(ADG706_A2_GPIO_Port,ADG706_A2_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ADG706_A3_GPIO_Port,ADG706_A3_Pin,GPIO_PIN_RESET);
}
static 	void openADG706_S5()
{
    ADG706_EN(1);
    HAL_GPIO_WritePin(ADG706_A0_GPIO_Port,ADG706_A0_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ADG706_A1_GPIO_Port,ADG706_A1_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ADG706_A2_GPIO_Port,ADG706_A2_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(ADG706_A3_GPIO_Port,ADG706_A3_Pin,GPIO_PIN_RESET);
}
static void openADG706_S6()
{
    ADG706_EN(1);
    HAL_GPIO_WritePin(ADG706_A0_GPIO_Port,ADG706_A0_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(ADG706_A1_GPIO_Port,ADG706_A1_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ADG706_A2_GPIO_Port,ADG706_A2_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(ADG706_A3_GPIO_Port,ADG706_A3_Pin,GPIO_PIN_RESET);
}
static	void openADG706_S7()
{

    ADG706_EN(1);
    HAL_GPIO_WritePin(ADG706_A0_GPIO_Port,ADG706_A0_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ADG706_A1_GPIO_Port,ADG706_A1_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(ADG706_A2_GPIO_Port,ADG706_A2_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(ADG706_A3_GPIO_Port,ADG706_A3_Pin,GPIO_PIN_RESET);


}
static	void openADG706_S8()
{
    ADG706_EN(1);
    HAL_GPIO_WritePin(ADG706_A0_GPIO_Port,ADG706_A0_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(ADG706_A1_GPIO_Port,ADG706_A1_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(ADG706_A2_GPIO_Port,ADG706_A2_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(ADG706_A3_GPIO_Port,ADG706_A3_Pin,GPIO_PIN_RESET);
}
static 	void openADG706_S9()
{
    ADG706_EN(1);
    HAL_GPIO_WritePin(ADG706_A0_GPIO_Port,ADG706_A0_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ADG706_A1_GPIO_Port,ADG706_A1_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ADG706_A2_GPIO_Port,ADG706_A2_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ADG706_A3_GPIO_Port,ADG706_A3_Pin,GPIO_PIN_SET);
}
static void openADG706_S10()
{
    ADG706_EN(1);
    HAL_GPIO_WritePin(ADG706_A0_GPIO_Port,ADG706_A0_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(ADG706_A1_GPIO_Port,ADG706_A1_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ADG706_A2_GPIO_Port,ADG706_A2_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ADG706_A3_GPIO_Port,ADG706_A3_Pin,GPIO_PIN_SET);
}
static	void openADG706_S11()
{


    ADG706_EN(1);
    HAL_GPIO_WritePin(ADG706_A0_GPIO_Port,ADG706_A0_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ADG706_A1_GPIO_Port,ADG706_A1_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(ADG706_A2_GPIO_Port,ADG706_A2_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ADG706_A3_GPIO_Port,ADG706_A3_Pin,GPIO_PIN_SET);
}
static	void openADG706_S12()
{
    ADG706_EN(1);
    HAL_GPIO_WritePin(ADG706_A0_GPIO_Port,ADG706_A0_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(ADG706_A1_GPIO_Port,ADG706_A1_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(ADG706_A2_GPIO_Port,ADG706_A2_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ADG706_A3_GPIO_Port,ADG706_A3_Pin,GPIO_PIN_SET);
}
static 	void openADG706_S13()
{
    ADG706_EN(1);
    HAL_GPIO_WritePin(ADG706_A0_GPIO_Port,ADG706_A0_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ADG706_A1_GPIO_Port,ADG706_A1_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ADG706_A2_GPIO_Port,ADG706_A2_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(ADG706_A3_GPIO_Port,ADG706_A3_Pin,GPIO_PIN_SET);
}
static void openADG706_S14()
{
    ADG706_EN(1);
    HAL_GPIO_WritePin(ADG706_A0_GPIO_Port,ADG706_A0_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(ADG706_A1_GPIO_Port,ADG706_A1_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ADG706_A2_GPIO_Port,ADG706_A2_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(ADG706_A3_GPIO_Port,ADG706_A3_Pin,GPIO_PIN_SET);

}
static	void openADG706_S15()
{
    ADG706_EN(1);
    HAL_GPIO_WritePin(ADG706_A0_GPIO_Port,ADG706_A0_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ADG706_A1_GPIO_Port,ADG706_A1_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(ADG706_A2_GPIO_Port,ADG706_A2_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(ADG706_A3_GPIO_Port,ADG706_A3_Pin,GPIO_PIN_SET);
}
static	void openADG706_S16()
{
    ADG706_EN(1);
    HAL_GPIO_WritePin(ADG706_A0_GPIO_Port,ADG706_A0_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(ADG706_A1_GPIO_Port,ADG706_A1_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(ADG706_A2_GPIO_Port,ADG706_A2_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(ADG706_A3_GPIO_Port,ADG706_A3_Pin,GPIO_PIN_SET);
}

/*-----------------------------------------------------------------------*/
/**
  *@ �������ܻ���: ģ�⿪�ص��л�
  * @�������:��
  * @�� �� ֵ: ��
  * @�ص�˵������
  * @���ߣ�Jyen Z
  *
  ******************************************************************************
  *
  *----------------------����Jyen---------------------����Jyen--------------------------*/
void   ADG706_SEL(ADG706_X_TypeDefE ADG706_SX)
{
    switch (ADG706_SX)
    {
    case NONE:
        ADG706_EN(0);
        break;

    case ADG706_S1  :
        openADG706_S1();
        break;

    case ADG706_S2  :
        openADG706_S2();
        break;


    case ADG706_S3  :
        openADG706_S3();
        break;


    case ADG706_S4  :
        openADG706_S4();

        break;
    case ADG706_S5  :
        openADG706_S5();
        break;

    case ADG706_S6  :
        openADG706_S6();
        break;


    case ADG706_S7  :
        openADG706_S7();
        break;


    case ADG706_S8  :
        openADG706_S8();
        break;

    case ADG706_S9  :
        openADG706_S9();
        break;

    case ADG706_S10  :
        openADG706_S10();
        break;


    case ADG706_S11  :
        openADG706_S11();
        break;


    case ADG706_S12  :
        openADG706_S12();
        break;

    case ADG706_S13  :
        openADG706_S13();
        break;

    case ADG706_S14  :
        openADG706_S14();
        break;


    case ADG706_S15  :
        openADG706_S15();
        break;


    case ADG706_S16  :
        openADG706_S16();
        break;
    default:
        ADG706_EN(0);
        break;
    }
}




void  ADG706_init()
{
#ifndef ADG706
    δ��ʼ��ADG706
#endif
    ADG706_EN(0);
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin = ADG706_EN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ADG706_EN_GPIO_Port, &GPIO_InitStruct);


    GPIO_InitStruct.Pin = ADG706_A0_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ADG706_A0_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = ADG706_A1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ADG706_A1_GPIO_Port, &GPIO_InitStruct);


    GPIO_InitStruct.Pin = ADG706_A2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ADG706_A2_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = ADG706_A3_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ADG706_A3_GPIO_Port, &GPIO_InitStruct);


}


