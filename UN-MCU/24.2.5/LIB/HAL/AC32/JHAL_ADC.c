#include "../JHAL_ADC.h"

 
#include "ac780x_adc_reg.h"

 //BandGap���СADֵ
const int SYS_BandGapAd_Min = 900, SYS_BandGapAd_Max = 1100;
/**  ��׼��ѹ����λ��MV */
const float ADC_BANDGAP_VOLTAGE	=	1.2f  ;  
 /*------------------Jyen--------------------------Jyen-----------------------*/
/*******************************************************************************
*@ �������ܻ���: ��ͨ�����βɼ�ģʽ��ʼ��
  * @�������:  
 isEN  true��ʼ��  false����ʼ��
 channel ��Ӧͨ�������� 
number  Ҫ��ʼ��ͨ���ĸ���
  * @�� �� ֵ: ��
  * @��ע: 
  * 
  *
  ******************************************************************************
  *
  *------------------Jyen-------------------------Jyen-------------------------*/

//���Զ����������Գ�ʼ���Ͳ���ͨ������  �������ŵĲ���Ҳ���账��
/* 
   void  JHAL_adcSingleModeInit(bool  isEN,u8* channel,u8 number)
{


    static bool sEN= {true};
    if(sEN!=isEN)
    {
        sEN=isEN;

        if(isEN)
        {
           ATC_ADC_Init();
        } else {

            ADC_DeInit(ADC0);
        }
    }

}
*/






//��ͨ���ɼ�
u32  JHAL_adcAqcSingle(u8 channel)

{
    /**  ÿ�ζ���Ҫ��������ɼ����� */
    ADC_SetLowPassFilter(ADC0,(ADC_ChannelType)channel,ADC_LPF_1200KHZ);
    ADC_SetRegularGroupChannel(ADC0, (ADC_ChannelType)channel, ADC_SPT_CLK_64, 0);
    ADC_SoftwareStartRegularConvert(ADC0);

    while(!ADC_GetIdleFlag(ADC0))
    {
        udelay(1);
    }
    return ADC_GetRegularData(ADC0);

}

