/**
  ******************************************************************************
  * @file    ������ģ����Գ���
  * @author  dotod
  * @version V1.0
  * @date    10-DEC-2017
  * @brief
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "keyboard4X4.h"
/* ��������------------------------------------------------------------------*/


void scn_Keyboard4X4_init()
{
    GPIO_InitTypeDef GPIO_InitStruct;
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    GPIO_InitStruct.Pin =Key_R1|Key_R2|Key_R3|Key_R4;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOX, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = Key_C1|Key_C2|Key_C3|Key_C4;;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOX, &GPIO_InitStruct);

}
/*************************************************************************************************
* @�������ܣ�ָ�����ŵ��������  
*
* @�����������
* @����˵������
* @����ֵ����
* @�ص�˵������
* @ʾ����˵������
* @����;Jyen Z
* **************@����;Jyen Z*****************************@����;Jyen Z******************@����;Jyen Z*********
*/
void  PXOutpt(uint16_t GPIO_Pin,GPIO_PinState PinState )
{
    HAL_GPIO_WritePin(GPIOX, GPIO_Pin, PinState);
}

uint8  PXInput(uint16_t GPIO_Pin)
{

    return	HAL_GPIO_ReadPin(GPIOX,GPIO_Pin);
}
/*************************************************************************************************
* @�������ܣ���ȡ������̵�ԭ����  ��һ���� ʣ������  Ȼ������͵Ľ��Ƿ�����  ��Ϊ�Ѿ��ⲿ�������Բ���Ҫ����һ�������  ֻҪ��һ������һ��������� ��51��һ�� ����˫���������
             ������  C���� R����
* @�����������
* @����˵������
* @����ֵ����
* @�ص�˵������
* @ʾ����˵������
* @����;Jyen Z
* **************@����;Jyen Z*****************************@����;Jyen Z******************@����;Jyen Z*********
*/
//mode:0,��֧��������;1,֧��������;
uint8 ReadKeyBorad (uint8 mode)
{
    static u8 key0_up=1;//�������ɿ���־
    static u8 key1_up=1;//�������ɿ���־
    static u8 key2_up=1;//�������ɿ���־
    static u8 key3_up=1;//�������ɿ���־
    static u8 key4_up=1;//�������ɿ���־
    static u8 key5_up=1;//�������ɿ���־
    static u8 key6_up=1;//�������ɿ���־
    static u8 key7_up=1;//�������ɿ���־
    static u8 key8_up=1;//�������ɿ���־
    static u8 key9_up=1;//�������ɿ���־
    static u8 keyA_up=1;//�������ɿ���־
    static u8 keyB_up=1;//�������ɿ���־
    static u8 keyC_up=1;//�������ɿ���־
    static u8 keyD_up=1;//�������ɿ���־
    static u8 keyX_up=1;//�������ɿ���־
    static u8 keyJ_up=1;//�������ɿ���־
    if(mode)
    {
        key0_up=1;
        key1_up=1;
        key2_up=1;
        key3_up=1;
        key4_up=1;
        key5_up=1;
        key6_up=1;
        key7_up=1;
        key8_up=1;
        key9_up=1;
        keyA_up=1;
        keyB_up=1;
        keyC_up=1;
        keyD_up=1;
        keyX_up=1;
        keyJ_up=1;
    }

    PXOutpt(Key_R1,GPIO_PIN_RESET);//�ѵ�һ������͵�ƽ
    PXOutpt(Key_R2,GPIO_PIN_SET);
    PXOutpt(Key_R3,GPIO_PIN_SET);
    PXOutpt(Key_R4,GPIO_PIN_SET);

    if(key1_up&&PXInput(Key_C1)==0)
    {
        HAL_Delay(jpys);
        key1_up=0;
        if(PXInput(Key_C1)==1) //�����һ���ǵ͵�ƽ��˵���м������£����û��ֱ���˳�if���
        {
            return 1;
        }
    }
    else if(PXInput(Key_C1==1))key1_up=1;


    if(key2_up&&PXInput(Key_C2)==0)//����ڶ����ǵ͵�ƽ��
    {
        HAL_Delay(jpys);//��ʱ20�룬�������
        key2_up=0;
        if(PXInput(Key_C2)==0)//����ڶ����ǵ͵�ƽ��˵���м������£����û��ֱ���˳�if���
        {
            return 2;
        }
    }
    else if(PXInput(Key_C2)==1)key2_up=1;

		
    if(key3_up&&PXInput(Key_C3)==0)
    {
        HAL_Delay(jpys);
        key3_up=0;
        if(PXInput(Key_C3)==0)
        {
            return 3;
        }
    }
    else if(PXInput(Key_C3)==1)key3_up=1;

    if(keyA_up&&PXInput(Key_C4)==0)//����������ǵ͵�ƽ
    {
        HAL_Delay(jpys);
        keyA_up=0;
        if(PXInput(Key_C4)==0)//����������ǵ͵�ƽ��˵���м������£����û��ֱ���˳�if���
        {
            return 4;
        }
    }
    else if(PXInput(Key_C4)==1)keyA_up=1;











    //�ڶ���

    PXOutpt(Key_R1,GPIO_PIN_SET);
    PXOutpt(Key_R2,GPIO_PIN_RESET); //�ѵ�2������͵�ƽ
    PXOutpt(Key_R3,GPIO_PIN_SET);
    PXOutpt(Key_R4,GPIO_PIN_SET);
    if(key4_up&&PXInput(Key_C1)==0)//�����һ���ǵ͵�ƽ
    {
        HAL_Delay(jpys);
        key4_up=0;
        if(PXInput(Key_C1)==0)//˵���м������£����û��ֱ���˳�if���
        {
            return 5;
        }
    }
    else if(PXInput(Key_C1)==1)key4_up=1;

    if(key5_up&&PXInput(Key_C2)==0)
    {
        HAL_Delay(jpys);
        key5_up=0;
        if(PXInput(Key_C2)==0)
        {
            return 6;
        }
    }
    else if(PXInput(Key_C2)==1)key5_up=1;

    if(key6_up&&PXInput(Key_C3)==0)
    {
        HAL_Delay(jpys);
        key6_up=0;
        if(PXInput(Key_C3)==0)
        {
            return 7;
        }
    }
    else if(PXInput(Key_C3)==1)key6_up=1;

    if(keyB_up&&PXInput(Key_C4)==0)
    {
        HAL_Delay(jpys);
        keyB_up=0;
        if(PXInput(Key_C4)==0)
        {
            return 8;
        }
    }
    else if(PXInput(Key_C4)==1)keyB_up=1;
    //������
    PXOutpt(Key_R1,GPIO_PIN_SET);
    PXOutpt(Key_R2,GPIO_PIN_SET);
    PXOutpt(Key_R3,GPIO_PIN_RESET); //�ѵ������õ�
    PXOutpt(Key_R4,GPIO_PIN_SET);


    if(key7_up&&PXInput(Key_C1)==0) //�����һ���ǵ͵�ƽ
    {
        HAL_Delay(jpys);//��ʱ20��
        key7_up=0;
        if(PXInput(Key_C1)==0)//˵���м������£����û��ֱ���˳�if���
        {
            return 9;
        }
    }
    else if(PXInput(Key_C1)==1)key7_up=1;

    if(key8_up&&PXInput(Key_C2)==0)
    {
        HAL_Delay(jpys);
        key8_up=0;
        if(PXInput(Key_C2)==0)
        {
            return 10;
        }
    }
    else if(PXInput(Key_C2)==1)key8_up=1;

    if(key9_up&&PXInput(Key_C3)==0)
    {
        HAL_Delay(jpys);
        key9_up=0;
        if(PXInput(Key_C3)==0)
        {
            return 11;
        }
    }
    else if(PXInput(Key_C3)==1)key9_up=1;

    if(keyC_up&&PXInput(Key_C4)==0)
    {
        HAL_Delay(jpys);
        keyC_up=0;
        if(PXInput(Key_C4)==0)
        {
            return 12;
        }
    }
    else if(PXInput(Key_C4)==1)keyC_up=1;

    //������
    PXOutpt(Key_R1,GPIO_PIN_SET);
    PXOutpt(Key_R2,GPIO_PIN_SET);
    PXOutpt(Key_R3,GPIO_PIN_SET);
    PXOutpt(Key_R4,GPIO_PIN_RESET); 	//�ѵ������õ�
    if(keyX_up&&PXInput(Key_C1)==0)//�����һ���ǵ͵�ƽ
    {
        HAL_Delay(jpys);
        keyX_up=0;
        if(PXInput(Key_C1)==0)//˵���м������£����û��ֱ���˳�if���
        {
            return 13;
        }
    }
    else if(PXInput(Key_C1)==1)keyX_up=1;

    if(key0_up&&PXInput(Key_C2)==0)
    {
        HAL_Delay(jpys);
        key0_up=0;
        if(PXInput(Key_C2)==0)
        {
            return 14;
        }
    }
    else if(PXInput(Key_C2)==1)key0_up=1;

    if(keyJ_up&&PXInput(Key_C3)==0)//����������ǵ͵�ƽ
    {
        HAL_Delay(20);
        keyJ_up=0;
        if(PXInput(Key_C3)==0)//˵���м������£����û��ֱ���˳�if���
        {
            return 15;
        }
    }
    else if(PXInput(Key_C3)==1)keyJ_up=1;

    if(keyD_up&&PXInput(Key_C4)==0)
    {
        HAL_Delay(20);
        keyD_up=0;
        if(PXInput(Key_C4)==0)
        {
            return 16;
        }
    }
    else if(PXInput(Key_C4)==1)keyD_up=1;

    return 0;
}



