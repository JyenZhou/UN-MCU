#include "../JHAL_ADC.h"
#include <xl_adc.h>




ADC_Type* __JHAL_id2adc(u8 adc)
{
    if(adc==0)
    {

        return ADC0;
    } else {
//不存在或未实现
        while(true);
    }
}




 

/*
//BandGap最大、小AD值
const int SYS_BandGapAd_Min = 950, SYS_BandGapAd_Max = 1150;
// 基准电压，单位：MV
const float ADC_BANDGAP_VOLTAGE	=	1.2f  ;
*/

//ADC单通道采集
u32  __JHAL_adcAqcSingle(ADC_Type* adc,u8 channel)
{

//选择通道
    ADC_SetChannel(adc,channel);
//等待转化完成
    while(!ADC_GetFlagStatus(adc, ADC_FLAG_CONV_FINISH));
    return  ADC_GetConversionValue(adc);
}

 


JHAL_ADCValue __JHAL_adcAqcSingleCollector(JHAL_ADC* adc,u8 adc_channel)
{
    uint32_t t_adcValue ;
    uint32_t t_adcBuffer[  adc->samplingCount]  ;
    for(uint16_t t_count = 0; t_count <  adc->samplingCount; t_count++)
    {
        t_adcValue =__JHAL_adcAqcSingle(   __JHAL_id2adc( adc->id) ,adc_channel);
        t_adcBuffer[t_count] = t_adcValue;
        if(t_adcValue==0)
        {
            t_adcValue=0;
        }
    }

    JHAL_sortU32ArrayAsc( t_adcBuffer,adc->samplingCount);

    JHAL_ADCValue adcValue;
// 防止AD采集异常  抛掉2个最大值最小值
    adcValue.minAD=t_adcBuffer[2];
    adcValue.ad=t_adcBuffer[(adc->samplingCount-4)/2+2];
    adcValue.maxAD=t_adcBuffer[adc->samplingCount-3];
    return adcValue;
}
/*
  获取电压计算系数
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



bool  JHAL_adcOpen(JHAL_ADC *jadc )
{
   if(   !jadc->__info.isOpen ) 
	 {
		 jadc->__info.isOpen=true;
		 if(jadc->samplingCount==0)
		 {
			 jadc->samplingCount=5;
		 }
        ADC_Type* adc=__JHAL_id2adc(jadc->id);
        JHAL_delayOpen(*(JHAL_Delay  *)NULL);
        ADC_InitTypeDef ADC_InitStruct= {0};
        ADC_FIFOTypeDef ADC_FIFOStruct= {0};

        SIM_SCGC_Cmd(SIM_SCGC_ADC,ENABLE);
        if(jadc->triggerSoure==JHAL_ADC_ReferVoltage_PWM)
        {
            __JHAL_jpwm2adcTriggerSoureValue( (jadc->triggerSoureValue));
        }

        ADC_InitStruct.ADC_Mode=ADC_MODE_12BIT;		//输出宽度
        ADC_InitStruct.ADC_ClockSource=ADC_CLOCK_BUS_CLOCK;	//转换时钟 总线时钟
        ADC_InitStruct.ADC_ClkDivide=ADC_ADIV_DIVIDE_8;	//转换时钟分频
        if(jadc->vref==JHAL_ADC_ReferVoltage_VDD) {
            ADC_InitStruct.ADC_ReferVoltage=ADC_VREF_VDDA; //参考电压，模拟供电电压VDDA/VSSA
        } else {
            ADC_InitStruct.ADC_ReferVoltage=ADC_VREF_VREFH; //参考电压，模拟供电电压VDDA/VSSA
        }
        ADC_InitStruct.ADC_LongTimeConvert=ENABLE;		//长采样时间
        ADC_InitStruct.ADC_LowPowerConvert=ENABLE;		//低功耗转换使能
        ADC_Init(adc, &ADC_InitStruct);		//

        ADC_SetTriggerMode(adc,ADC_SOFTWARE_TRIGGER);	//触发选择

        ADC_SetConventionMode(adc,ADC_CONVENTION_ONCE);	//单次转换
        if(jadc->isMultichannelMode) {
//该单片机连续模式最大支持8通道
            while(jadc->channelsNumber>ADC_FIFO_LEVEL8+1);

            ADC_FIFOStruct.ADC_FIFOLevel=jadc->channelsNumber-1;
            ADC_FIFOStruct.ADC_ChannelScan=ENABLE;

        } else {
            ADC_FIFOStruct.ADC_FIFOLevel=ADC_FIFO_DISABLE;	//FIFO功能关闭，扫描功能无用--只扫描FIFO_0
            ADC_FIFOStruct.ADC_ChannelScan=DISABLE;		//信道扫描，扫描FIFO所有配置的信道
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
        
        //第一次不准 初始化时候采集一次
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
	 
	
        ADC_DeInit(__JHAL_id2adc(jadc->id));
     
		 jadc->__info.isOpen=false ;
    return true; 
 }
	 return false; 
}

JHAL_ADCInfo JHAL_adcAqcSingle(JHAL_ADC *jadc,u8 channelIndex)
{  
	 ADC_Type* adc=__JHAL_id2adc(jadc->id);
	
	
    JHAL_ADCInfo adcInfo= {0};
    adcInfo.adcValue=__JHAL_adcAqcSingleCollector(jadc, jadc->channels[channelIndex]);
    adcInfo.minVoltage=adcInfo.adcValue.minAD*jadc->__info.calculationCoefficient;
    adcInfo.voltage=adcInfo.adcValue.ad*jadc->__info.calculationCoefficient;
    adcInfo.maxVoltage=adcInfo.adcValue.maxAD*jadc->__info.calculationCoefficient;
    return adcInfo;
}

bool  JHAL_adcAqcMultiple (JHAL_ADC *jadc )
{
    ADC_Type* adc=__JHAL_id2adc(jadc->id);


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







