#include "../JHAL_ADC.h"
#include <xl_adc.h>




ADC_Type* __JHAL_jadc2adc(JHAL_ADC adc)
{
    if(adc==JHAL_ADC0)
    {

        return ADC0;
    } else {
//不存在或未实现
        while(true);
    }
}




#define JHAL_ADC_AVERAGE_TIMES_MAX   11    /**  ADC滤波队列长度 要大于5 */

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
// 防止AD采集异常  抛掉2个最大值最小值
    adcValue.minAD=t_adcBuffer[2];
    adcValue.ad=t_adcBuffer[(JHAL_ADC_AVERAGE_TIMES_MAX-4)/2+2];
    adcValue.maxAD=t_adcBuffer[JHAL_ADC_AVERAGE_TIMES_MAX-3];
    return adcValue;
}
/*
  获取电压计算系数
*/




void  __JHAL_adcUpdateVoltageCalculationCoefficient(ADC_Type* adc,JHAL_ADCConfig *config)
{

    switch(config->vref)
    {
    case  JHAL_ADC_ReferVoltage_BandGap:
        config->__otherInfo. calculationCoefficient=1.2f/ __JHAL_adcAqcSingleCollector(adc,ADC_CHANNEL_BANDGAP).ad;
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
        ADC_Type* adc=__JHAL_jadc2adc(jadc);
        JHAL_delayInit(*(JHAL_DealyConfig *)NULL);
        ADC_InitTypeDef ADC_InitStruct= {0};
        ADC_FIFOTypeDef ADC_FIFOStruct= {0};

        SIM_SCGC_Cmd(SIM_SCGC_ADC,ENABLE);
        if(config->triggerSoure==JHAL_ADC_ReferVoltage_PWM)
        {
            __JHAL_jpwm2adcTriggerSoureValue((JHAL_PWM)(config->triggerSoureValue));
        }

        ADC_InitStruct.ADC_Mode=ADC_MODE_12BIT;		//输出宽度
        ADC_InitStruct.ADC_ClockSource=ADC_CLOCK_BUS_CLOCK;	//转换时钟 总线时钟
        ADC_InitStruct.ADC_ClkDivide=ADC_ADIV_DIVIDE_8;	//转换时钟分频
        if(config->vref==JHAL_ADC_ReferVoltage_VDD) {
            ADC_InitStruct.ADC_ReferVoltage=ADC_VREF_VDDA; //参考电压，模拟供电电压VDDA/VSSA
        } else {
            ADC_InitStruct.ADC_ReferVoltage=ADC_VREF_VREFH; //参考电压，模拟供电电压VDDA/VSSA
        }
        ADC_InitStruct.ADC_LongTimeConvert=ENABLE;		//长采样时间
        ADC_InitStruct.ADC_LowPowerConvert=ENABLE;		//低功耗转换使能
        ADC_Init(adc, &ADC_InitStruct);		//

        ADC_SetTriggerMode(adc,ADC_SOFTWARE_TRIGGER);	//触发选择

        ADC_SetConventionMode(adc,ADC_CONVENTION_ONCE);	//单次转换
        if(config->isMultichannelMode) {
//该单片机连续模式最大支持8通道
            while(config->channelsNumber>ADC_FIFO_LEVEL8+1);

            ADC_FIFOStruct.ADC_FIFOLevel=config->channelsNumber-1;
            ADC_FIFOStruct.ADC_ChannelScan=ENABLE;

        } else {
            ADC_FIFOStruct.ADC_FIFOLevel=ADC_FIFO_DISABLE;	//FIFO功能关闭，扫描功能无用--只扫描FIFO_0
            ADC_FIFOStruct.ADC_ChannelScan=DISABLE;		//信道扫描，扫描FIFO所有配置的信道
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
        //第一次不准 初始化时候采集一次
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
    adcInfo.voltage=adcInfo.adcValue.ad*__adcConfig[jadc]->__otherInfo.calculationCoefficient;
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
        __adcConfig[jadc]->adcInfosBuff[i].voltage=ADC_GetConversionValue(adc)*__adcConfig[jadc]->__otherInfo.calculationCoefficient;

    }
    for(u8 i=0; i<__adcConfig[jadc]->channelsNumber; i++)
    {

        ADC_SetChannel(adc,   __adcConfig[jadc]->channels[i]);
    }

    return true;
}







