/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*                                                                     *
          		���ʱ���ò���  �����ÿ���					 
*                                               
       ͨ������ı� �����ѹ  ���һ���ǵ�·���Ѿ������   ���ģ����Ҫ����ʹ��ʧ�� �������Բ����� ����һֱʹ�� �����ܲ�������Ƭ������
                   
									���㷽��0.5*��1+��R1/R2������TPS7A7001�ֲ�10ҳ*
  *                                                                 *
   *                                                              *
*********@����*Jyen******************@����*Jyen***********************@����*Jyen********************/

#include "TPS7A7001.h"

/*-----------------------------------------------------------------------*/
/**
  *@ �������ܻ���: ʹ��ʧ�ܸ�ģ��
  * @�������:��                 
  * @�� �� ֵ: ��
  * @�ص�˵������
  * @���ߣ�Jyen Z
  *
  ******************************************************************************
  *
  *----------------------����Jyen---------------------����Jyen--------------------------*/
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
