#include "../JHAL_ADC.h"
#include <xl_adc.h>




ADC_Type* __JHAL_id2adc(u8 adc)
{
    if(adc==0)
    {

        return ADC0;
    } else {
//�����ڻ�δʵ��
        while(true);
    }
}




 

/*
//BandGap���СADֵ
const int SYS_BandGapAd_Min = 950, SYS_BandGapAd_Max = 1150;
// ��׼��ѹ����λ��MV
const float ADC_BANDGAP_VOLTAGE	=	1.2f  ;
*/

//ADC��ͨ���ɼ�
u32  __JHAL_adcAqcSingle(ADC_Type* adc,u8 channel)
{

//ѡ��ͨ��
    ADC_SetChannel(adc,channel);
//�ȴ�ת�����
    while(!ADC_GetFlagStatus(adc, ADC_FLAG_CONV_FINISH));
    return  ADC_GetConversionValue(adc);
}

 


JHAL_ADCValue __JHAL_adcAqcSingleCollector(JHAL_ADC* adc,u8 adc_channel)
{
    uint32_t t_adcValue ;
    uint32_t t_adcBuffer[  adc->samplingCount]  ;
    for(uint16_t t_count = 0; t_count <  adc->samplingCount; t_count++)
    {
        t_adcValue =__JHAL_adcAqcSingle(   __JHAL_id2adc( adc->dev) ,adc->channelsNumber);
        t_adcBuffer[t_count] = t_adcValue;
        if(t_adcValue==0)
        {
            t_adcValue=0;
        }
    }

    JHAL_sortU32ArrayAsc( t_adcBuffer,adc->samplingCount);

    JHAL_ADCValue adcValue;
// ��ֹAD�ɼ��쳣  �׵�2�����ֵ��Сֵ
    adcValue.minAD=t_adcBuffer[2];
    adcValue.ad=t_adcBuffer[(adc->samplingCount-4)/2+2];
    adcValue.maxAD=t_adcBuffer[adc->samplingCount-3];
    return adcValue;
}
/*
  ��ȡ��ѹ����ϵ��
*/




void  __JHAL_adcUpdateVoltageCalculationCoefficient( JHAL_ADC  *jadc)
{
 
    switch(jadc->vref)
    {
    case  JHAL_ADC_ReferVoltage_BandGap:
        jadc->__info. calculationCoefficient=1.2f/ __JHAL_adcAqcSingleCollector(jadc,ADC_CHANNEL_BANDGAP).ad;
        break;
    case  JHAL_ADC_ReferVoltage_VDD:
    case  JHAL_ADC_ReferVoltage_Vref:
        jadc->__info.calculationCoefficient=jadc->vrefVoltageValue/4095;
        break;
    }

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



bool  JHAL_adcOpen(JHAL_ADC *jadc )
{
   if(   !jadc->__info.isOpen ) 
	 {
		 jadc->__info.isOpen=true;
        ADC_Type* adc=__JHAL_id2adc(jadc->dev);
        JHAL_delayOpen(*(JHAL_Delay  *)NULL);
        ADC_InitTypeDef ADC_InitStruct= {0};
        ADC_FIFOTypeDef ADC_FIFOStruct= {0};

        SIM_SCGC_Cmd(SIM_SCGC_ADC,ENABLE);
        if(jadc->triggerSoure==JHAL_ADC_ReferVoltage_PWM)
        {
            __JHAL_jpwm2adcTriggerSoureValue( (jadc->triggerSoureValue));
        }

        ADC_InitStruct.ADC_Mode=ADC_MODE_12BIT;		//������
        ADC_InitStruct.ADC_ClockSource=ADC_CLOCK_BUS_CLOCK;	//ת��ʱ�� ����ʱ��
        ADC_InitStruct.ADC_ClkDivide=ADC_ADIV_DIVIDE_8;	//ת��ʱ�ӷ�Ƶ
        if(jadc->vref==JHAL_ADC_ReferVoltage_VDD) {
            ADC_InitStruct.ADC_ReferVoltage=ADC_VREF_VDDA; //�ο���ѹ��ģ�⹩���ѹVDDA/VSSA
        } else {
            ADC_InitStruct.ADC_ReferVoltage=ADC_VREF_VREFH; //�ο���ѹ��ģ�⹩���ѹVDDA/VSSA
        }
        ADC_InitStruct.ADC_LongTimeConvert=ENABLE;		//������ʱ��
        ADC_InitStruct.ADC_LowPowerConvert=ENABLE;		//�͹���ת��ʹ��
        ADC_Init(adc, &ADC_InitStruct);		//

        ADC_SetTriggerMode(adc,ADC_SOFTWARE_TRIGGER);	//����ѡ��

        ADC_SetConventionMode(adc,ADC_CONVENTION_ONCE);	//����ת��
        if(jadc->isMultichannelMode) {
//�õ�Ƭ������ģʽ���֧��8ͨ��
            while(jadc->channelsNumber>ADC_FIFO_LEVEL8+1);

            ADC_FIFOStruct.ADC_FIFOLevel=jadc->channelsNumber-1;
            ADC_FIFOStruct.ADC_ChannelScan=ENABLE;

        } else {
            ADC_FIFOStruct.ADC_FIFOLevel=ADC_FIFO_DISABLE;	//FIFO���ܹرգ�ɨ�蹦������--ֻɨ��FIFO_0
            ADC_FIFOStruct.ADC_ChannelScan=DISABLE;		//�ŵ�ɨ�裬ɨ��FIFO�������õ��ŵ�
        }
        ADC_FIFOConfig(adc, &ADC_FIFOStruct);	//
        ADC_EnableCmd(adc,ENABLE);		//ADC enable
        JHAL_delayUs(50);
        for(u8 i=0; i<jadc->channelsNumber; i++)
        {
            if(jadc->isMultichannelMode) {
                ADC_SetChannel(adc,jadc->channels[i]);
            } else {
                __JHAL_adcAqcSingle(adc,jadc->channels[i]);
            }
        }

        __JHAL_adcUpdateVoltageCalculationCoefficient(  jadc);
        
        //��һ�β�׼ ��ʼ��ʱ��ɼ�һ��
        if(jadc->isMultichannelMode) {
            while( !JHAL_adcAqcMultiple(jadc));
        }
				
				 return  jadc->__info.isOpen=true;
			}
       return false; 
    
}
bool JHAL_adcClose(JHAL_ADC *jadc)
{
   if(   jadc->__info.isOpen ) 
	 {
	 
	
        ADC_DeInit(__JHAL_id2adc(jadc->dev));
     
		 jadc->__info.isOpen=false ;
    return true; 
 }
	 return false; 
}

JHAL_ADCInfo JHAL_adcAqcSingle(JHAL_ADC *jadc,u8 adc_channel)
{   ADC_Type* adc=__JHAL_id2adc(jadc->dev);
    JHAL_ADCInfo adcInfo= {0};
    adcInfo.adcValue=__JHAL_adcAqcSingleCollector(jadc,adc_channel);
    adcInfo.minVoltage=adcInfo.adcValue.minAD*jadc->__info.calculationCoefficient;
    adcInfo.voltage=adcInfo.adcValue.ad*jadc->__info.calculationCoefficient;
    adcInfo.maxVoltage=adcInfo.adcValue.maxAD*jadc->__info.calculationCoefficient;
    return adcInfo;
}

bool  JHAL_adcAqcMultiple (JHAL_ADC *jadc )
{
    ADC_Type* adc=__JHAL_id2adc(jadc->dev);


    if(!ADC_GetFlagStatus(adc, ADC_FLAG_CONV_FINISH))
    {
        return false;
    };
    for(u8 i=0; i<jadc->channelsNumber; i++)
    {
        jadc->adcInfosBuff[i].voltage=ADC_GetConversionValue(adc)*jadc->__info.calculationCoefficient;

    }
    for(u8 i=0; i<jadc->channelsNumber; i++)
    {

        ADC_SetChannel(adc,   jadc->channels[i]);
    }

    return true;
}







