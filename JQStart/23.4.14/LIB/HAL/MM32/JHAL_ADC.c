#include "../JHAL_ADC.h"





bool  JHAL_adcOpen(JHAL_ADC *adc )
{



    if((		adc->	__info.convertedValue=(u32*)mymalloc(adc->channelsNumber*sizeof(u32)))==NULL)
    {
        //�ڴ����ʧ�� ;
        JHAL_Fault_Handler("JHAL_adcOpen");
    }


    ADC_InitTypeDef  ADC_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB1PeriphClockCmd(RCC_APB1ENR_ADC1, ENABLE);

    ADC_StructInit(&ADC_InitStruct);
    ADC_InitStruct.ADC_Resolution       = ADC_Resolution_12b;
    ADC_InitStruct.ADC_PRESCARE         = ADC_PCLK2_PRESCARE_16;
    ADC_InitStruct.ADC_Mode             = ADC_Mode_Scan;
    ADC_InitStruct.ADC_DataAlign        = ADC_DataAlign_Right;
    ADC_Init(ADC1, &ADC_InitStruct);

    ADC_SampleTimeConfig(ADC1, ADC_Samctl_240_5);

    ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
    ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);



    for(u8 index=0; index<adc->channelsNumber; index++)
    {
        u8 ch=adc->channels[ index];
        if(ch==0)
        {

            RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOB, ENABLE);
            GPIO_StructInit(&GPIO_InitStruct);
            GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_1;
            GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AIN;
            GPIO_Init(GPIOB, &GPIO_InitStruct);
        } else if(ch==1)
        {
            RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOB, ENABLE);
            GPIO_StructInit(&GPIO_InitStruct);
            GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_0;
            GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AIN;
            GPIO_Init(GPIOB, &GPIO_InitStruct);

        } else if(ch==2)
        {
            RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA, ENABLE);
            GPIO_StructInit(&GPIO_InitStruct);
            GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_3;
            GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AIN;
            GPIO_Init(GPIOA, &GPIO_InitStruct);

        } else if(ch==3)
        {
            RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA, ENABLE);
            GPIO_StructInit(&GPIO_InitStruct);
            GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_12;
            GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AIN;
            GPIO_Init(GPIOA, &GPIO_InitStruct);

        } else if(ch==4)
        {
            RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA, ENABLE);
            GPIO_StructInit(&GPIO_InitStruct);
            GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_11;
            GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AIN;
            GPIO_Init(GPIOA, &GPIO_InitStruct);

        } else if(ch==5)
        {
            RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA, ENABLE);
            GPIO_StructInit(&GPIO_InitStruct);
            GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_2;
            GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AIN;
            GPIO_Init(GPIOA, &GPIO_InitStruct);

        } else if(ch==6)
        {
            RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA, ENABLE);
            GPIO_StructInit(&GPIO_InitStruct);
            GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_15;
            GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AIN;
            GPIO_Init(GPIOA, &GPIO_InitStruct);

        } else if(ch==7)
        {
            RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA, ENABLE);
            GPIO_StructInit(&GPIO_InitStruct);
            GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_7;
            GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AIN;
            GPIO_Init(GPIOA, &GPIO_InitStruct);

        }

    }

    ADC_Cmd(ADC1, ENABLE);


    return true;


}

bool JHAL_adcClose(JHAL_ADC *adc)
{

    ADC_Cmd(ADC1, DISABLE);
    myfree(   	adc->		__info.convertedValue);

    return false;
}



JHAL_ADCInfo  JHAL_adcAqcSingle(JHAL_ADC *adc,u8 channelIndex)

{
    JHAL_ADCInfo adcInfo;

    ADC_ANY_NUM_Config(ADC1, 0);
    ADC_ANY_CH_Config(ADC1, 0,  (ADCCHANNEL_TypeDef)( adc->channels[ channelIndex]));
    ADC_ANY_Cmd(ADC1, ENABLE);

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    while (RESET == ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
    {
    }

    ADC_ClearFlag(ADC1, ADC_FLAG_EOC);

    adcInfo.adcValue.ad = ADC_GetConversionValue(ADC1);

    ADC_ANY_Cmd(ADC1, DISABLE);


    adcInfo.voltage =   adcInfo.adcValue.ad  * adc->vrefVoltageValue/4096;

    return adcInfo;
}




