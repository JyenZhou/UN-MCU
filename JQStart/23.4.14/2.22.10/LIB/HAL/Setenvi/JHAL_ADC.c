#include "../JHAL_ADC.h"
 #include "asm32lx003_adc.h"
ADC_TypeDef* __JHAL_jadc2adc(JHAL_ADC adc)
{
    if(adc==JHAL_ADC0)
    {

        return ADC;
    } else {
//�����ڻ�δʵ��
        while(true);
    }
}
u8 __JHAL_jadc2id(JHAL_ADC adc)
{
    if(adc==JHAL_ADC0)
    {
        return 0;

    } else {

//�����ڻ�δʵ��
        while(true);
    }

}

//���������
#define __ADC_Number  1


#define JHAL_ADC_AVERAGE_TIMES_MAX   11    /**  ADC�˲����г��� Ҫ����5 */

/*
//BandGap���СADֵ
const int SYS_BandGapAd_Min = 950, SYS_BandGapAd_Max = 1150;
// ��׼��ѹ����λ��MV
const float ADC_BANDGAP_VOLTAGE	=	1.2f  ;
*/

//ADC��ͨ���ɼ�
u32  __JHAL_adcAqcSingle(ADC_TypeDef* adc,u8 channel)
{

 
  ADC_SoftwareStartConv(adc);  
//�ȴ�ת�����
	while (ADC_GetRISFlagStatus(ADC, ADC_TICONT_FLAG) == RESET);   
	u32 adcV= ADC_GetChannelConversionValue(adc, 1<<channel);
		ADC_ClearFlag(ADC, ADC_TICONT_FLAG);  
    return adcV ;
}

static JHAL_ADCConfig *__adcConfig[__ADC_Number];





JHAL_ADCValue __JHAL_adcAqcSingleCollector(ADC_TypeDef* adc,u8 adc_channel)
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




void  __JHAL_adcUpdateVoltageCalculationCoefficient(ADC_TypeDef* adc,JHAL_ADCConfig *config)
{

    switch(config->vref)
    {
    case  JHAL_ADC_ReferVoltage_BandGap:
			//��mcu��֧��
        while(1);
       
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
        ADC_TypeDef* adc=__JHAL_jadc2adc(jadc);
        
      
      
       
        if(config->triggerSoure==JHAL_ADC_ReferVoltage_PWM)
        {
					//��֧�ָù���
          while(true);
        }

				
			/* GPIO �ܽŹ���ѡ�� */
	GPIO_InitTypeDef GPIO_InitStructor={0};
  GPIO_InitStructor.GPIO_Pin   = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6;
  GPIO_InitStructor.GPIO_Mode  = GPIO_Mode_AN;       // ģ������
	GPIO_InitStructor.GPIO_OType = GPIO_OType_PP;      
	GPIO_InitStructor.GPIO_Speed = GPIO_Speed_2MHz;    // �ٶ�����
	GPIO_InitStructor.GPIO_PuPd  = GPIO_PuPd_NOPULL;   // ��������
	GPIO_Init(GPIOD, &GPIO_InitStructor);
	
	GPIO_InitStructor.GPIO_Pin   =  GPIO_Pin_6;
	GPIO_Init(GPIOC, &GPIO_InitStructor);
    
		  /* ��ʼ��ADCͨ�Žṹ�� */
					ADC_CommonInitTypeDef ADC_CommonInitStruct={0};
					ADC_CommonInitStruct.ADC_CircleMode = ADC_Mode_Independent;   // ����ADCת��Ϊ��ѭ��ģʽ
	ADC_CommonInitStruct.ADC_CtMode     = ADC_Ct_Contine;         // ����ADCת��ģʽ
	ADC_CommonInitStruct.ADC_SetAdcCnt  = (uint32_t)9<<8;         // ��������ת������
	ADC_CommonInitStruct.ADC_Trigs0Sel  = ADC_Trigs0_TIM1;     // �ⲿ����ADת������
	ADC_CommonInitStruct.ADC_Trigs1Sel  = ADC_Trigs1_TIM1;     // �ⲿ����ADת������
	ADC_CommonInit(ADC, &ADC_CommonInitStruct);
		/* ��ʼ��ADC�ṹ�� */  
		  ADC_InitTypeDef ADC_InitStruct= {0};
				 
					ADC_InitStruct.ADC_ChannelSel = ADC_Channel0;     
	ADC_InitStruct.ADC_ClkSel = ADC_Prescaler_Div1;   // ����ADCʱ�ӷ�Ƶ��1, @Fs=24MHZʱ��Ƶ����24MHz
	ADC_InitStruct.ADC_SamSel = ADC_SAMPLE_16CYCLE;   // ����ADC��������, ���޸�
	ADC_Init(ADC, &ADC_InitStruct);
	
				//ADC�ж�����
				    NVIC_EnableIRQ(ADC0_IRQn);
    NVIC_SetPriority(ADC0_IRQn, 2);
			/* ���жϱ�־ */
    ADC_ClearFlag(ADC, ADC_TICONT_FLAG);    
					/* ʹ������ת���ж� */
   // ADC_ITCmd(ADC, ADC_TICONT_FLAG, ENABLE);  

	/* ��λADC����ת��״̬ */
	ADC_ContinuousConverRstCmd(ADC, ENABLE);
	ADC_ContinuousConverRstCmd(ADC, DISABLE);
	
		/* ADC����ת��ͨ��ʹ�� */
	 
     //�õ�Ƭ������ģʽ���֧��8ͨ��
            while(config->channelsNumber>8);
   
 
        for(u8 i=0; i<config->channelsNumber; i++)
        {
            if(config->isMultichannelMode) {
							//ͨ�����ֵΪ7
							 while(config->channels[i]>7);
								ADC_ContinuousConverChannelCmd(ADC, 1<<config->channels[i], ENABLE);  // 
            
            } else {
                __JHAL_adcAqcSingle(adc,config->channels[i]);
            }
        }
				
     __JHAL_adcUpdateVoltageCalculationCoefficient(adc, config);
        __adcConfig[__JHAL_jadc2id(jadc)]=config;
	
	/* ʹ��ADC���� */
	ADC_Cmd(ADC, ENABLE);
    
    /* ADC��ʼת�� */
    ADC_SoftwareStartConv(ADC); 
		
		
		

   
      
return true; 
    }
		return false;
}
bool JHAL_adcDeInit(JHAL_ADC jadc)
{
    if(__JHAL_adcIsENCheak(jadc,false)) {
        ADC_DeInit();
			return true;
    }
			return false;
}
/**-------------------------------------------------------
ADC�жϷ������
--------------------------------------------------------*/
void ADC0_IRQHandler(void)
{   
 
}


JHAL_ADCInfo JHAL_adcAqcSingle(JHAL_ADC jadc,u8 adc_channel)
{   ADC_TypeDef* adc=__JHAL_jadc2adc(jadc);
u8 id=	__JHAL_jadc2id(jadc);
    JHAL_ADCInfo adcInfo= {0};
    adcInfo.adcValue=__JHAL_adcAqcSingleCollector(adc,adc_channel);
    adcInfo.minVoltage=adcInfo.adcValue.minAD*__adcConfig[id]->__otherInfo.calculationCoefficient;
    adcInfo.middleVoltage=adcInfo.adcValue.middleAD*__adcConfig[id]->__otherInfo.calculationCoefficient;
    adcInfo.maxVoltage=adcInfo.adcValue.maxAD*__adcConfig[id]->__otherInfo.calculationCoefficient;
    return adcInfo;
}

bool  JHAL_adcAqcMultiple (JHAL_ADC jadc )
{
    ADC_TypeDef* adc=__JHAL_jadc2adc(jadc);
u8 id=	__JHAL_jadc2id(jadc);

    if(ADC_GetRISFlagStatus(adc, ADC_TICONT_FLAG) == RESET)
    {
        return false;
    };
    for(u8 i=0; i<__adcConfig[id]->channelsNumber; i++)
    {
        __adcConfig[id]->adcInfosBuff[i].middleVoltage=ADC_GetChannelConversionValue(adc, 1<<(__adcConfig[id]->channels[i]))*__adcConfig[id]->__otherInfo.calculationCoefficient;

    }
   
   	ADC_ClearFlag(adc, ADC_TICONT_FLAG);    
    return true;
}







