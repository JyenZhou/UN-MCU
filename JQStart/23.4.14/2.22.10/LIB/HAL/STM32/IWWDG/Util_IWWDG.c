/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*                                                                     *

1. Ҫ�� stm32f1xx_hal_wwdg.c ���ӽ���

	�Ѿ��ѳ�ʼ����ι����װ��   ���������ֱ�������·������Ƶ��ü���

	 MX_WWDG_Init(0x7F, 0X5F,WWDG_PRESCALER_8);//��ʼ����

//Ĭ�Ͽ������жϱ���ι�� ֻҪ�ڳ�ʼ��ʱ����������ʼ������������
	 Wwdog();//����ι��
	 ע���ι���ĵط�����while���� ��ʱ��
*                                                                     *
  *                                                                 *
   *                                                              *
*********@����*Jyen******************@����*Jyen***********************@����*Jyen********************/
#include "WWdog.h"
WWDG_HandleTypeDef hwwdg;
uint8_t wwdg_tr, wwdg_wr;
/**
  * ��������: WWDG��ʼ������
  * �������: tr ���ݼ���ʱ����ֵ�� ȡֵ��ΧΪ��0x7f~0x40
  *           wr ������ֵ��ȡֵ��ΧΪ��0x7f~0x40
  *           prv��Ԥ��Ƶ��ֵ��ȡֵ������
  *              ���� WWDG_PRESCALER_1: WWDG counter clock = (PCLK1/4096)/1
  *              ���� WWDG_PRESCALER_2: WWDG counter clock = (PCLK1/4096)/2
  *              ���� WWDG_PRESCALER_4: WWDG counter clock = (PCLK1/4096)/4
  *              ���� WWDG_PRESCALER_8: WWDG counter clock = (PCLK1/4096)/8
  * �� �� ֵ: ��
  * ˵    ������
  */
void MX_WWDG_Init(uint8_t tr, uint8_t wr, uint32_t WWDG_PRESCALER_X)
{
    // ���ֵ  ����ֵ  ��Ƶ

#ifndef HAL_WWDG_MODULE_ENABLED
    δ��ʼ�����Ź�
#endif
    hwwdg.Instance = WWDG;
    hwwdg.Init.Prescaler = WWDG_PRESCALER_X;
    hwwdg.Init.Window = wr;
    hwwdg.Init.Counter = tr;
    hwwdg.Init.EWIMode = WWDG_EWI_ENABLE;
    if (HAL_WWDG_Init(&hwwdg) != HAL_OK)
    {
        //  Error_Handler();
    }
    /* ����ֵ�����ڳ�ʼ����ʱ�����ó�0X5F�����ֵ����ı� */
    wwdg_wr = WWDG->CFR & 0X7F;//��ֵ wwdg_wr
}
/**
  * ��������: WWDG����
  * �������: hwwdg�����ڿ��Ź����ָ��
  * �� �� ֵ: ��
  * ˵    ����Hal���ڲ�����
  */
void HAL_WWDG_MspInit(WWDG_HandleTypeDef* hwwdg)
{

    if(hwwdg->Instance==WWDG)
    {
        /* ���ڿ��Ź�ʱ��ʹ�� */
        __HAL_RCC_WWDG_CLK_ENABLE();
        /* �����ж����ȼ����ü�ʹ�� */
        HAL_NVIC_SetPriority(WWDG_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(WWDG_IRQn);
    }
}

void HAL_WWDG_MspDeInit(WWDG_HandleTypeDef* hwwdg)
{
    if(hwwdg->Instance==WWDG)
    {
        /* ���ڿ��Ź�ʱ�ӽ��� */
        __HAL_RCC_WWDG_CLK_DISABLE();

        /* �����жϽ��� */
        HAL_NVIC_DisableIRQ(WWDG_IRQn);
    }

}

/*-----------------------------------------------------------------------*/
/**
  *@ �������ܻ���:��������ι�� ������������ ��������ʱʱ�ͻ����ι����ʱ
  * @�������:��
  * @�� �� ֵ: ��
  * @�ص�˵������
  * @���ߣ�Jyen Z
  *
  ******************************************************************************
  *
  *----------------------����Jyen---------------------����Jyen--------------------------*/
void Wwdog (void)
{
    wwdg_tr = WWDG->CR & 0X7F;
    if( wwdg_tr < wwdg_wr )
    {
        /* ι�����������ü�������ֵΪ���0X7F */
        HAL_WWDG_Refresh(&hwwdg);
    }
}
//���ڿ��Ź��жϷ�����
void WWDG_IRQHandler(void)
{
//Wwdog();
    //HAL_WWDG_Refresh(&hwwdg);//����ι��ֻ�ܷ�������  ���涼����
    HAL_WWDG_IRQHandler(&hwwdg);//����WWDG�����жϴ�������  �����ͻ���ص����� ��������ι��


}

//�жϷ�������������
//�˺����ᱻHAL_WWDG_IRQHandler()����
/***�÷��������ѷ����� ��֪��ʲôʱ��ִ��**/
void HAL_WWDG_WakeupCallback(WWDG_HandleTypeDef* hwwdg)
{
    //Wwdog();
    // HAL_WWDG_Refresh(hwwdg);//���´��ڿ��Ź�ֵ

}


//ι�������÷���� �����ں���ֻ���ж�ι����Żص�  ��Ȼ�͸�λ
//����˵���п������ܵ���ӡ��Ӱ�� ˵����Ȼ��ǰ�ж� ��ʱ��ܶ�
/*���Ź��жϻص����� ����ι�� �ڿ�����ǰ�жϻ�����ι��*/
void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef* hwwdg)
{


    HAL_WWDG_Refresh(hwwdg);
    //HAL_WWDG_Refresh(hwwdg);//���´��ڿ��Ź�ֵ
}