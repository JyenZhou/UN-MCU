#include "ADG1204_3.h"

/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*              
*
\
       ��ADG1204Bͨ���л��Ĳ������˸�����
                 ADG1204B ��ֵ��


	               A1   A0   ����ͨ��
	               0   0  	 0
		             0   1  	 1
					       1	0	 		 	2
								1		1		    3

	                   ʹ��λ1 ʧ������


           
*                                                                     *
  *                                                                 *
   *                                                              *
*********@����*Jyen******************@����*Jyen***********************@����*Jyen********************/
 

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
  *@ �������ܻ���: ʹ��GAS_X��ĳ��ͨ��
  * @�������:��      ö���� GAS1_X GAS1_0-GAS1_8
  * @�� �� ֵ: ��
  * @�ص�˵����GAS1_0����ʧ�ܹر�
  * @���ߣ�Jyen Z
  *
  ******************************************************************************
  *
  *----------------------����Jyen---------------------����Jyen--------------------------*/
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
  //δ��ʼ��ADG1204
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

