#include "../JHAL_ADC.h"

 //BandGap���СADֵ
const int SYS_BandGapAd_Min = 1750, SYS_BandGapAd_Max = 1950;
/**  ��׼��ѹ����λ��MV */
const float ADC_BANDGAP_VOLTAGE	=	1.5f  ;
void JHAL_adchannelGPIOCModeInit(u8 channel)
{
#if defined(KF32A151MQV)



#elif defined (KF32A140INP)

  switch(channel)
  {
  case ADC_CHANNEL_7:
  GPIO_Write_Mode_Bits(GPIOC_SFR,GPIO_PIN_MASK_7,GPIO_MODE_AN);
  break;
  case ADC_CHANNEL_8:
   GPIO_Write_Mode_Bits(GPIOC_SFR,GPIO_PIN_MASK_8,GPIO_MODE_AN);
   break;
  case ADC_CHANNEL_9:
    GPIO_Write_Mode_Bits(GPIOC_SFR,GPIO_PIN_MASK_9,GPIO_MODE_AN);
    break;
  case ADC_CHANNEL_13:
   GPIO_Write_Mode_Bits(GPIOC_SFR,GPIO_PIN_MASK_13,GPIO_MODE_AN);
   break;
  case ADC_CHANNEL_36:
   GPIO_Write_Mode_Bits(GPIOE_SFR,GPIO_PIN_MASK_15,GPIO_MODE_AN);
   break;
  case ADC_CHANNEL_37:
   GPIO_Write_Mode_Bits(GPIOA_SFR,GPIO_PIN_MASK_0,GPIO_MODE_AN);
   break;
  case ADC_CHANNEL_38:
   GPIO_Write_Mode_Bits(GPIOA_SFR,GPIO_PIN_MASK_1,GPIO_MODE_AN);
   break;
  case ADC_CHANNEL_39:
   GPIO_Write_Mode_Bits(GPIOA_SFR,GPIO_PIN_MASK_2,GPIO_MODE_AN);
   break;
  case ADC_CHANNEL_40:
   GPIO_Write_Mode_Bits(GPIOA_SFR,GPIO_PIN_MASK_3,GPIO_MODE_AN);
   break;
  case ADC_CHANNEL_41:
   GPIO_Write_Mode_Bits(GPIOA_SFR,GPIO_PIN_MASK_5,GPIO_MODE_AN);
   break;
  case ADC_CHANNEL_42:
   GPIO_Write_Mode_Bits(GPIOA_SFR,GPIO_PIN_MASK_6,GPIO_MODE_AN);
   break;
   //53���ڲ���׼ͨ��
  case ADC_CHANNEL_53:
	  break;
  default:
    //�����ڵ�ADC
    while(1);

  }


#else
  ��Ҫ��������ͺ�����ӳ��
#endif



}



/*MSP��س�ʼ��*/
void JHAL_adcMSPInit(u8* channel,u8 number)
{
for(u8 i=0;i<number;i++)
  {
	JHAL_adchannelGPIOCModeInit(channel[i]);
  }



}

/*�����е���ʱ����--������ADC��ʼ��*/
void JHAL_adcInitDelay(volatile uint32_t cnt) {
        while (cnt--)
                ;
}


void  _JHAL_adcSingleModeInit(u8* channel,u8 number)
{



        ADC_InitTypeDef adcStruct;

        ADC_Reset (ADC0_SFR); //��λADC���貢��ADC��ʱ��

        /* ��ʼ�� ADCʱ��Դѡ�� */
        adcStruct.m_Clock = ADC_HFCLK;
        /* ��ʼ�� ADCʱ�ӷ�Ƶ */
        adcStruct.m_ClockDiv = ADC_CLK_DIV_32; //ѡ��32��Ƶ
        /* ��ʼ�� ADCɨ��ģʽʹ�� */
        adcStruct.m_ScanMode = FALSE;  //��ʹ��ɨ��ģʽ��һ�δ���ֻת����һ��ͨ��
        /* ��ʼ�� ADC����ת��ģʽ */
        adcStruct.m_ContinuousMode = ADC_SINGLE_MODE; //��ʹ������ת�����ܣ�ֻ�����õ�ͨ������һ��
        /* ��ʼ�� ADCת����������ʽ */
        adcStruct.m_DataAlign = ADC_DATAALIGN_RIGHT; //ת���Ľ���Ҷ���
        /* ��ʼ�� ADC����ͨ���ⲿ����ת��ģʽʹ�� */
        adcStruct.m_ExternalTrig_EN = FALSE; //����ͨ��ʧ���ⲿ��������ADCת����
        /* ��ʼ�� ADC����ͨ���ⲿ�����¼� */
        adcStruct.m_ExternalTrig = ADC_EXTERNALTRIG_T1TRGO; //����ͨ���ⲿ�����ź�Դ����ΪT1TRGO
        /* ��ʼ�� ADC�����ȼ�ͨ���ⲿ����ת��ģʽʹ�� */
        adcStruct.m_HPExternalTrig_EN = FALSE; //�����ȼ�ͨ��ʧ���ⲿ��������ADCת����
        /* ��ʼ�� �����ȼ�ͨ���ⲿ�����¼� */
        adcStruct.m_HPExternalTrig = ADC_HPEXTERNALTRIG_CCP1_CH1; //�����ȼ�ͨ�������ź�Դ����ΪCCP1��ͨ��1
        /* �ο���ѹѡ��ȡֵΪ�ꡰADC�ο���ѹѡ���е�һ���� */
        adcStruct.m_VoltageRef=ADC_REF_AVDD; //ת���Ĳο���ѹԴ����ΪVDDA
        /* ��ʼ�� ADC����ͨ��ɨ�賤�� */
        adcStruct.m_NumOfConv = 1; //ɨ��ĳ�������Ϊ1��ͨ��
        /* ��ʼ�� ADC�����ȼ�ͨ��ɨ�賤�� */
        adcStruct.m_NumOfHPConv = 1; //�����ȼ�ͨ����ɨ�賤��
        ADC_Configuration (ADC0_SFR, &adcStruct);

        ADC_Cmd (ADC0_SFR, TRUE); //ADCʹ��

        JHAL_adcInitDelay(0xFFF);  //ADCʹ�ܺ���Ҫһ�ζ��ݵ�ʱ�䣬�ȴ���·�����Ϻ󣬲���ȥ���������߿��ܴ��ڽϴ�Ĳ������

        JHAL_adcMSPInit(channel,number);

}


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
void  JHAL_adcSingleModeInit(bool  isEN,u8* channel,u8 number)
{


 static bool sEN= {false};
 if(sEN!=isEN)
 {
     sEN=isEN;

     if(isEN)
     {
    		//�ο���ѹ����
    	    PM_VREF_Software_Enable(TRUE);
    		PM_VREF_SELECT(PM_VREF_VOLTAGE_1P5V);
    	    ADC_Reference_Voltage_Config(ADC0_SFR,ADC_REF_INTERNAL);
    	 _JHAL_adcSingleModeInit( channel, number);
     } else {

        //TODO  ADC_Reset�����ᵽ��λʱ�� ���ۿ��Թرն�Ӧʱ��
     }
 }

}







//��ͨ���ɼ�
u32  JHAL_adcAqcSingle(u8 channel)

{
  //ԭ�Ͳο� ADC_Regular_Channel_Config (ADC0_SFR, ADC_CHANNEL_2, 0x01); //ADC0����ͨ��1�Ĳ���Դ����ΪADC_CHANNEL_2
  ADC_Regular_Channel_Config (ADC0_SFR, channel, 0x01); //ADC0����ͨ��1�Ĳ���Դ����ΪADC_CHANNEL_1
  ADC_Software_Start_Conv(ADC0_SFR);              //�������ת��
  while(!ADC_Get_INT_Flag(ADC0_SFR,ADC_INT_EOC)); //�ȴ�ת��������
  return ADC_Get_Conversion_Value(ADC0_SFR); //��ȡADC���ݼĴ���,ͬʱӲ�����Զ�����EOCIF

}

