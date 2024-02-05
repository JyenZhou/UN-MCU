/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*              
*
\
       ��CD4051BBͨ���л��Ĳ������˸�����
                 CD4051BB ��ֵ��


	           C    B   A   ����ͨ��
	           0    0   0  		 0
		         0    0   1  		 1
					   0    1		0	 		2
					   0		1		1		  3
					   1		0		0	  	4
					   1		0		1		  5
					   1		1		0	   	6
					   1		1		1	  	7

	                   ʹ��λ1 ʧ������


          �����ǿ��ƴ�����ƥ�����
*                                                                     *
  *                                                                 *
   *                                                              *
*********@����*Jyen******************@����*Jyen***********************@����*Jyen********************/
#include "CD4051B_3.h"

static 	void CD4051B_EN(uint8 boolean)
{
	if(boolean==1)
	{
		 HAL_GPIO_WritePin(CD4051B_3_EN_GPIO_Port,CD4051B_3_EN_Pin,GPIO_PIN_RESET);
	}else{
			  HAL_GPIO_WritePin(CD4051B_3_EN_GPIO_Port,CD4051B_3_EN_Pin,GPIO_PIN_SET);
	}
}


static 	void openCD4051B_Channel1()
{CD4051B_EN(1);
			HAL_GPIO_WritePin(CD4051B_3_A_GPIO_Port,CD4051B_3_A_Pin,GPIO_PIN_RESET);
HAL_GPIO_WritePin(CD4051B_3_B_GPIO_Port,CD4051B_3_B_Pin,GPIO_PIN_RESET);
HAL_GPIO_WritePin(CD4051B_3_C_GPIO_Port,CD4051B_3_C_Pin,GPIO_PIN_RESET);
}
static 	void openCD4051B_Channel2()
{CD4051B_EN(1);
			HAL_GPIO_WritePin(CD4051B_3_A_GPIO_Port,CD4051B_3_A_Pin,GPIO_PIN_SET);
HAL_GPIO_WritePin(CD4051B_3_B_GPIO_Port,CD4051B_3_B_Pin,GPIO_PIN_RESET);
HAL_GPIO_WritePin(CD4051B_3_C_GPIO_Port,CD4051B_3_C_Pin,GPIO_PIN_RESET);
}
static 	void openCD4051B_Channel3()
{CD4051B_EN(1);
	HAL_GPIO_WritePin(CD4051B_3_A_GPIO_Port,CD4051B_3_A_Pin,GPIO_PIN_RESET);
HAL_GPIO_WritePin(CD4051B_3_B_GPIO_Port,CD4051B_3_B_Pin,GPIO_PIN_SET);	
HAL_GPIO_WritePin(CD4051B_3_C_GPIO_Port,CD4051B_3_C_Pin,GPIO_PIN_RESET);
}
static 	void openCD4051B_Channel4()
{
	CD4051B_EN(1);
			HAL_GPIO_WritePin(CD4051B_3_A_GPIO_Port,CD4051B_3_A_Pin,GPIO_PIN_SET);
HAL_GPIO_WritePin(CD4051B_3_B_GPIO_Port,CD4051B_3_B_Pin,GPIO_PIN_SET);
HAL_GPIO_WritePin(CD4051B_3_C_GPIO_Port,CD4051B_3_C_Pin,GPIO_PIN_RESET);
}static 	void openCD4051B_Channel5()
{CD4051B_EN(1);
		HAL_GPIO_WritePin(CD4051B_3_A_GPIO_Port,CD4051B_3_A_Pin,GPIO_PIN_RESET);
HAL_GPIO_WritePin(CD4051B_3_B_GPIO_Port,CD4051B_3_B_Pin,GPIO_PIN_RESET);
HAL_GPIO_WritePin(CD4051B_3_C_GPIO_Port,CD4051B_3_C_Pin,GPIO_PIN_SET);

}
static 	void openCD4051B_Channel6()
{CD4051B_EN(1);
	
		HAL_GPIO_WritePin(CD4051B_3_A_GPIO_Port,CD4051B_3_A_Pin,GPIO_PIN_SET);
HAL_GPIO_WritePin(CD4051B_3_B_GPIO_Port,CD4051B_3_B_Pin,GPIO_PIN_RESET);
HAL_GPIO_WritePin(CD4051B_3_C_GPIO_Port,CD4051B_3_C_Pin,GPIO_PIN_SET);
}
static 	void openCD4051B_Channel7()
{
	
  	  CD4051B_EN(1);
		HAL_GPIO_WritePin(CD4051B_3_A_GPIO_Port,CD4051B_3_A_Pin,GPIO_PIN_RESET);
HAL_GPIO_WritePin(CD4051B_3_B_GPIO_Port,CD4051B_3_B_Pin,GPIO_PIN_SET);
HAL_GPIO_WritePin(CD4051B_3_C_GPIO_Port,CD4051B_3_C_Pin,GPIO_PIN_SET);
}
static 	void openCD4051B_Channel8()
{
	  CD4051B_EN(1);
   	HAL_GPIO_WritePin(CD4051B_3_A_GPIO_Port,CD4051B_3_A_Pin,GPIO_PIN_SET);
HAL_GPIO_WritePin(CD4051B_3_B_GPIO_Port,CD4051B_3_B_Pin,GPIO_PIN_SET);
HAL_GPIO_WritePin(CD4051B_3_C_GPIO_Port,CD4051B_3_C_Pin,GPIO_PIN_SET);


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
void CD4051B_3_ChannelSelect(CD4051B_Channel_X_TypeDefE  Channel)
{
    switch (Channel)
    {
    case NONE:
        CD4051B_EN(0);
        break;
    case Channel_1:
			openCD4051B_Channel1();
        break;
    case Channel_2:
				openCD4051B_Channel2();
        break;
    case Channel_3:
				openCD4051B_Channel3();
        break;
		
    case Channel_4:
				openCD4051B_Channel4();
        break;
    case Channel_5:
			openCD4051B_Channel5();
        break;
    case Channel_6:
				openCD4051B_Channel6();
        break;
    case Channel_7:
	openCD4051B_Channel7();
        break;
    case Channel_8:
			openCD4051B_Channel8();
        break;
    default:
        break;
    }
}
void  CD4051B_3_init()
{
#ifndef CD4051B_3
  δ��ʼ��CD4051B_3
#endif
	 CD4051B_EN(0);
	GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = CD4051B_3_EN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(CD4051B_3_EN_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = CD4051B_3_A_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(CD4051B_3_A_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = CD4051B_3_B_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(CD4051B_3_B_GPIO_Port, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = CD4051B_3_C_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(CD4051B_3_C_GPIO_Port, &GPIO_InitStruct);
	
	
}

