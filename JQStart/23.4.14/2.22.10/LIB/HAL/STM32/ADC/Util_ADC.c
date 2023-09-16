/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*                                                                     *

CUBEMX ����
ADC
1.number of Conversion ѡ��Ҫ����ת����ͨ���������.hһֱ(Ϊ�˼�������ĳ��� ֻ��һ���Ļ� ���ԼӸ��¶Ȼ��׼ͨ�� ʣ�µĲ��ù�)
2.continuos Conversion mode ->ENABLE ʹ������ת��ģʽ
3.����Rank

DMA
1.DMA-->Mode-->Circular
2.DMA-->Data Width-->word


            16ͨ��ADC1+DMA����
						 ADC��ʼ�� һ��Ҫ��ʱ�ٶ�ȡ  ��Ȼ��һ�ζ����ǿ�ֵ  ֮����Ķ�����һ��ֵ
						 
						 	 //ת��Ϊ�¶�ֵ,ʵ��Ӧ����,�ɿ����ú���Ϊ��λ,���⸡������
						   float  TEMP=	(1.43 - (ad*3.3/4096))/0.0043 + 25 = 55.23
					
*                                                                     *
  *                                                                 *
   *                                                              *
*********@����*Jyen******************@����*Jyen***********************@����*Jyen********************/

#include "Util_ADC.h"
// ADת�����ֵ
uint32_t  ADC_ConvertedValue[ADC_NUMOFCHANNEL];
uint32_t DMA_Transfer_Complete_Count=0;


/*-----------------------------------------------------------------------*/
/**
  *@ �������ܻ���: ��ʼ��DMA����ADC��ת��
  * @�������:��
  * @�� �� ֵ: ��
  * @�ص�˵������
  * @���ߣ�Jyen Z
  *
  ******************************************************************************
  *
  *----------------------����Jyen---------------------����Jyen--------------------------*/
void utilADC_Init (void)
{
 
  //  HAL_ADCEx_Calibration_Start(&hadc1);
  //  HAL_ADCEx_Calibration_Start(&hadc1);
    /* ����ADת����ʹ��DMA������ж� */
    HAL_ADC_Start_DMA(&hadc1,ADC_ConvertedValue,ADC_NUMOFCHANNEL);
    HAL_Delay(200);//�����ʱ��Ϊ�˵�һ�ζ�ȡadc����ֵ��  �Ǹ���ת��ʱ��ĵȴ�
}

/**
  * ��������: ADCת����ɻص�����
  * �������: hadc��ADC�����豸���
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void  HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    DMA_Transfer_Complete_Count++;
	
 
}





