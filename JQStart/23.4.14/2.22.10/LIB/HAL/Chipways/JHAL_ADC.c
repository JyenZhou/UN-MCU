#include "../JHAL_ADC.h"
#include <xl_adc.h>




ADC_Type* __JHAL_jadc2adc(JHAL_ADC adc)
{
    if(adc==JHAL_ADC0)
    {

        return ADC0;
    } else {
//�����ڻ�δʵ��
        while(true);
    }
}




#define JHAL_ADC_AVERAGE_TIMES_MAX   11    /**  ADC�˲����г��� Ҫ����5 */

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

static JHAL_ADCConfig *__adcConfig[JHAL_ADC_Number];





JHAL_ADCValue __JHAL_adcAqcSingleCollector(ADC_Type* adc,u8 adc_channel)
{
    uint32_t t_adcValue=0;
    uint32_t t_adcBuffer[JHAL_ADC_AVERAGE_TIMES_MAX] = {0};
    for(uint16_t t_count = 0; t_count < JHAL_ADC_AVERAGE_TIMES_MAX; t_count++)
    {
        t_adcValue =__JHAL_adcAqcSingle(adc,adc_channel);
        t_adcBuffer[t_count] = t_adcValue;
        if(t_adcValue==0)
        {
            t_adcValue=0;
        }
    }

    JHAL_sortU32ArrayAsc( t_adcBuffer,JHAL_ADC_AVERAGE_TIMES_MAX);

    JHAL_ADCValue adcValue;
// ��ֹAD�ɼ��쳣  �׵�2�����ֵ��Сֵ
    adcValue.minAD=t_adcBuffer[2];
    adcValue.middleAD=t_adcBuffer[(JHAL_ADC_AVERAGE_TIMES_MAX-4)/2+2];
    adcValue.maxAD=t_adcBuffer[JHAL_ADC_AVERAGE_TIMES_MAX-3];
    return adcValue;
}
/*
  ��ȡ��ѹ����ϵ��
*/




void  __JHAL_adcUpdateVoltageCalculationCoefficient(ADC_Type* adc,JHAL_ADCConfig *config)
{

    switch(config->vref)
    {
    case  JHAL_ADC_ReferVoltage_BandGap:
        config->__otherInfo. calculationCoefficient=1.2f/ __JHAL_adcAqcSingleCollector(adc,ADC_CHANNEL_BANDGAP).middleAD;
        break;
    case  JHAL_ADC_ReferVoltage_VDD:
    case  JHAL_ADC_ReferVoltage_Vref:
        config->__otherInfo.calculationCoefficient=config->vrefVoltageValue/4095;
        break;
    }

}

bool __JHAL_adcIsENCheak(JHAL_ADC id, bool isEN) {
    static bool sEN[JHAL_ADC_Number]= {false};
    if(sEN[(u8)id]!=isEN)
    {
        sEN[(u8)id]=isEN;
        return true;
    }
    return  false;
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



bool  JHAL_adcInit(JHAL_ADC jadc,JHAL_ADCConfig *config)
{
    if(__JHAL_adcIsENCheak(jadc,true)) {
        ADC_Type* adc=__JHAL_jadc2adc(jadc);
        JHAL_delayInit(*(JHAL_DealyConfig *)NULL);
        ADC_InitTypeDef ADC_InitStruct= {0};
        ADC_FIFOTypeDef ADC_FIFOStruct= {0};

        SIM_SCGC_Cmd(SIM_SCGC_ADC,ENABLE);
        if(config->triggerSoure==JHAL_ADC_ReferVoltage_PWM)
        {
            __JHAL_jpwm2adcTriggerSoureValue((JHAL_PWM)(config->triggerSoureValue));
        }

        ADC_InitStruct.ADC_Mode=ADC_MODE_12BIT;		//������
        ADC_InitStruct.ADC_ClockSource=ADC_CLOCK_BUS_CLOCK;	//ת��ʱ�� ����ʱ��
        ADC_InitStruct.ADC_ClkDivide=ADC_ADIV_DIVIDE_8;	//ת��ʱ�ӷ�Ƶ
        if(config->vref==JHAL_ADC_ReferVoltage_VDD) {
            ADC_InitStruct.ADC_ReferVoltage=ADC_VREF_VDDA; //�ο���ѹ��ģ�⹩���ѹVDDA/VSSA
        } else {
            ADC_InitStruct.ADC_ReferVoltage=ADC_VREF_VREFH; //�ο���ѹ��ģ�⹩���ѹVDDA/VSSA
        }
        ADC_InitStruct.ADC_LongTimeConvert=ENABLE;		//������ʱ��
        ADC_InitStruct.ADC_LowPowerConvert=ENABLE;		//�͹���ת��ʹ��
        ADC_Init(adc, &ADC_InitStruct);		//

        ADC_SetTriggerMode(adc,ADC_SOFTWARE_TRIGGER);	//����ѡ��

        ADC_SetConventionMode(adc,ADC_CONVENTION_ONCE);	//����ת��
        if(config->isMultichannelMode) {
//�õ�Ƭ������ģʽ���֧��8ͨ��
            while(config->channelsNumber>ADC_FIFO_LEVEL8+1);

            ADC_FIFOStruct.ADC_FIFOLevel=config->channelsNumber-1;
            ADC_FIFOStruct.ADC_ChannelScan=ENABLE;

        } else {
            ADC_FIFOStruct.ADC_FIFOLevel=ADC_FIFO_DISABLE;	//FIFO���ܹرգ�ɨ�蹦������--ֻɨ��FIFO_0
            ADC_FIFOStruct.ADC_ChannelScan=DISABLE;		//�ŵ�ɨ�裬ɨ��FIFO�������õ��ŵ�
        }
        ADC_FIFOConfig(adc, &ADC_FIFOStruct);	//
        ADC_EnableCmd(adc,ENABLE);		//ADC enable
        JHAL_delayUs(50);
        for(u8 i=0; i<config->channelsNumber; i++)
        {
            if(config->isMultichannelMode) {
                ADC_SetChannel(adc,config->channels[i]);
            } else {
                __JHAL_adcAqcSingle(adc,config->channels[i]);
            }
        }

        __JHAL_adcUpdateVoltageCalculationCoefficient(adc, config);
        __adcConfig[jadc]=config;
        //��һ�β�׼ ��ʼ��ʱ��ɼ�һ��
        if(config->isMultichannelMode) {
            while( !JHAL_adcAqcMultiple(jadc));
        }
        return true;
    }
    return false;
}
bool JHAL_adcDeInit(JHAL_ADC jadc)
{
    if(__JHAL_adcIsENCheak(jadc,false)) {
        ADC_DeInit(ADC0);
        return false;
    }
    return true;
}

JHAL_ADCInfo JHAL_adcAqcSingle(JHAL_ADC jadc,u8 adc_channel)
{   ADC_Type* adc=__JHAL_jadc2adc(jadc);
    JHAL_ADCInfo adcInfo= {0};
    adcInfo.adcValue=__JHAL_adcAqcSingleCollector(adc,adc_channel);
    adcInfo.minVoltage=adcInfo.adcValue.minAD*__adcConfig[jadc]->__otherInfo.calculationCoefficient;
    adcInfo.middleVoltage=adcInfo.adcValue.middleAD*__adcConfig[jadc]->__otherInfo.calculationCoefficient;
    adcInfo.maxVoltage=adcInfo.adcValue.maxAD*__adcConfig[jadc]->__otherInfo.calculationCoefficient;
    return adcInfo;
}

bool  JHAL_adcAqcMultiple (JHAL_ADC jadc )
{
    ADC_Type* adc=__JHAL_jadc2adc(jadc);


    if(!ADC_GetFlagStatus(adc, ADC_FLAG_CONV_FINISH))
    {
        return false;
    };
    for(u8 i=0; i<__adcConfig[jadc]->channelsNumber; i++)
    {
        __adcConfig[jadc]->adcInfosBuff[i].middleVoltage=ADC_GetConversionValue(adc)*__adcConfig[jadc]->__otherInfo.calculationCoefficient;

    }
    for(u8 i=0; i<__adcConfig[jadc]->channelsNumber; i++)
    {

        ADC_SetChannel(adc,   __adcConfig[jadc]->channels[i]);
    }

    return true;
}







