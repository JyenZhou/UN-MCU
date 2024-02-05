#include "../JHAL_ADC.h"
 #include "asm32lx003_adc.h"
ADC_TypeDef* __JHAL_jadc2adc(JHAL_ADC adc)
{
    if(adc==JHAL_ADC0)
    {

        return ADC;
    } else {
//不存在或未实现
        while(true);
    }
}
u8 __JHAL_jadc2id(JHAL_ADC adc)
{
    if(adc==JHAL_ADC0)
    {
        return 0;

    } else {

//不存在或未实现
        while(true);
    }

}

//该外设个数
#define __ADC_Number  1


#define JHAL_ADC_AVERAGE_TIMES_MAX   11    /**  ADC滤波队列长度 要大于5 */

/*
//BandGap最大、小AD值
const int SYS_BandGapAd_Min = 950, SYS_BandGapAd_Max = 1150;
// 基准电压，单位：MV
const float ADC_BANDGAP_VOLTAGE	=	1.2f  ;
*/

//ADC单通道采集
u32  __JHAL_adcAqcSingle(ADC_TypeDef* adc,u8 channel)
{

 
  ADC_SoftwareStartConv(adc);  
//等待转化完成
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
// 防止AD采集异常  抛掉2个最大值最小值
    adcValue.minAD=t_adcBuffer[2];
    adcValue.middleAD=t_adcBuffer[(JHAL_ADC_AVERAGE_TIMES_MAX-4)/2+2];
    adcValue.maxAD=t_adcBuffer[JHAL_ADC_AVERAGE_TIMES_MAX-3];
    return adcValue;
}
/*
  获取电压计算系数
*/




void  __JHAL_adcUpdateVoltageCalculationCoefficient(ADC_TypeDef* adc,JHAL_ADCConfig *config)
{

    switch(config->vref)
    {
    case  JHAL_ADC_ReferVoltage_BandGap:
			//该mcu不支持
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
*@ 函数功能或简介: 单通道单次采集模式初始化
 * @输入参数:
isEN  true初始化  false反初始化
channel 对应通道的数组
number  要初始化通道的个数
 * @返 回 值: 无
 * @备注:
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
					//不支持该功能
          while(true);
        }

				
			/* GPIO 管脚功能选择 */
	GPIO_InitTypeDef GPIO_InitStructor={0};
  GPIO_InitStructor.GPIO_Pin   = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6;
  GPIO_InitStructor.GPIO_Mode  = GPIO_Mode_AN;       // 模拟输入
	GPIO_InitStructor.GPIO_OType = GPIO_OType_PP;      
	GPIO_InitStructor.GPIO_Speed = GPIO_Speed_2MHz;    // 速度配置
	GPIO_InitStructor.GPIO_PuPd  = GPIO_PuPd_NOPULL;   // 无上下拉
	GPIO_Init(GPIOD, &GPIO_InitStructor);
	
	GPIO_InitStructor.GPIO_Pin   =  GPIO_Pin_6;
	GPIO_Init(GPIOC, &GPIO_InitStructor);
    
		  /* 初始化ADC通信结构体 */
					ADC_CommonInitTypeDef ADC_CommonInitStruct={0};
					ADC_CommonInitStruct.ADC_CircleMode = ADC_Mode_Independent;   // 设置ADC转换为非循环模式
	ADC_CommonInitStruct.ADC_CtMode     = ADC_Ct_Contine;         // 设置ADC转换模式
	ADC_CommonInitStruct.ADC_SetAdcCnt  = (uint32_t)9<<8;         // 设置连续转换次数
	ADC_CommonInitStruct.ADC_Trigs0Sel  = ADC_Trigs0_TIM1;     // 外部触发AD转换设置
	ADC_CommonInitStruct.ADC_Trigs1Sel  = ADC_Trigs1_TIM1;     // 外部触发AD转换设置
	ADC_CommonInit(ADC, &ADC_CommonInitStruct);
		/* 初始化ADC结构体 */  
		  ADC_InitTypeDef ADC_InitStruct= {0};
				 
					ADC_InitStruct.ADC_ChannelSel = ADC_Channel0;     
	ADC_InitStruct.ADC_ClkSel = ADC_Prescaler_Div1;   // 设置ADC时钟分频比1, @Fs=24MHZ时，频率是24MHz
	ADC_InitStruct.ADC_SamSel = ADC_SAMPLE_16CYCLE;   // 设置ADC采样周期, 可修改
	ADC_Init(ADC, &ADC_InitStruct);
	
				//ADC中断配置
				    NVIC_EnableIRQ(ADC0_IRQn);
    NVIC_SetPriority(ADC0_IRQn, 2);
			/* 清中断标志 */
    ADC_ClearFlag(ADC, ADC_TICONT_FLAG);    
					/* 使能连续转换中断 */
   // ADC_ITCmd(ADC, ADC_TICONT_FLAG, ENABLE);  

	/* 复位ADC连续转换状态 */
	ADC_ContinuousConverRstCmd(ADC, ENABLE);
	ADC_ContinuousConverRstCmd(ADC, DISABLE);
	
		/* ADC连续转换通道使能 */
	 
     //该单片机连续模式最大支持8通道
            while(config->channelsNumber>8);
   
 
        for(u8 i=0; i<config->channelsNumber; i++)
        {
            if(config->isMultichannelMode) {
							//通道最大值为7
							 while(config->channels[i]>7);
								ADC_ContinuousConverChannelCmd(ADC, 1<<config->channels[i], ENABLE);  // 
            
            } else {
                __JHAL_adcAqcSingle(adc,config->channels[i]);
            }
        }
				
     __JHAL_adcUpdateVoltageCalculationCoefficient(adc, config);
        __adcConfig[__JHAL_jadc2id(jadc)]=config;
	
	/* 使能ADC外设 */
	ADC_Cmd(ADC, ENABLE);
    
    /* ADC开始转换 */
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
ADC中断服务程序
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







