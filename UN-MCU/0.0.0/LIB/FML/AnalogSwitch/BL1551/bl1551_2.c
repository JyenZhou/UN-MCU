
/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*               	
 		ENB 1 �õ�   A1���ӵ�������B
		ENB 0 �õ�   A2���ӵ�������B
*                                                                     *
  *                                                                 *
   *                                                              *
*********@����*Jyen******************@����*Jyen***********************@����*Jyen********************/




 /*------------------����Jyen--------------------------����Jyen-----------------------*/
/*******************************************************************************
*@ �������ܻ���: 
  * @�������:   ��
  * @�� �� ֵ: ��
  * @�ص�˵��: ��
  * @���ߣ�Jyen Z
  *
  ******************************************************************************
  *
  *------------------����Jyen--------------------------����Jyen-------------------------*/
#include "bl1551_2.h"
void bl1551_2_init(void)
{
	#ifndef BL1551_2
    δ��ʼ��bl1551_2
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
*������1��ʱ�� A1���ӵ������� ����A2���ӵ�������
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
