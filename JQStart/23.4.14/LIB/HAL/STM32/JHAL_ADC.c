#include "../JHAL_ADC.h"

/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*                                                                     *

CUBEMX 配置  注意stm32中默认启用了verf  需要把最后一个通道配置成这个引脚或使用JHAL_ADC_ReferVoltage_NONE
ADC
1.number of Conversion 选择要连续转换的通道数与这儿.h一致(为了兼容这里的连续程序 只有一个的话 可以加个温度或基准通道 剩下的不用管)
2.continuos Conversion mode ->ENABLE 使能连续转换模式
3.扫描转换   需要使能
3.配置转换通道数Rank

DMA
1.DMA-->Mode-->Circular
2.DMA-->Data Width-->word


            16通道ADC1+DMA处理
						 ADC开始后 一定要延时再读取  不然第一次读的是空值  之后读的都是上一次值

						 	 //转换为温度值,实际应用中,可考虑用毫伏为单位,避免浮点运算
						   float  TEMP=	(1.43 - (ad*3.3/4096))/0.0043 + 25 = 55.23

*                                                                     *
  *                                                                 *
   *                                                              *
*********@作者*Jyen******************@作者*Jyen***********************@作者*Jyen********************/
#ifdef HAL_ADC_MODULE_ENABLED
 
//对于12位的ADC，3.3V的ADC值为0xfff,温度为25度时对应的电压值为1.43V即0x6EE

#define VREFINT 1.2f
#define V25  0x6EE
//斜率 每摄氏度4.3mV 对应每摄氏度0x05
#define AVG_SLOPE 0x05



 







/*
  获取电压计算系数
*/




void  __JHAL_adcUpdateVoltageCalculationCoefficient( JHAL_ADC *adc)
{

    switch(adc->vref)
    {
    case  JHAL_ADC_ReferVoltage_BandGap:
        adc->__info. calculationCoefficient=VREFINT/ (((u32 *)(adc-> __info.convertedValue))[(adc->channelsNumber)-1]&0xFFF);
        break;
    case  JHAL_ADC_ReferVoltage_VDD:
    case  JHAL_ADC_ReferVoltage_Vref:
        adc->__info.calculationCoefficient=adc->vrefVoltageValue/4095;
        break;
    case	JHAL_ADC_ReferVoltage_NONE:
        adc->__info.calculationCoefficient=3.3f/4096;
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
 * @备注:如果使用bandgap当做基准 adc ->channelsNumber长度需要包含verf
 *
 *
 ******************************************************************************
 *
 *------------------Jyen-------------------------Jyen-------------------------*/



bool  JHAL_adcOpen(JHAL_ADC *adc )
{


    if((		adc->	__info.convertedValue=(u32*)mymalloc(adc->channelsNumber*sizeof(u32)))==NULL)
    {
        //内存分配失败 ;
        while(true)  ;
    }

    ADC_HandleTypeDef*  hadc=   (ADC_HandleTypeDef *)adc->dev ;
#ifndef STM32F407xx
    HAL_ADCEx_Calibration_Start(hadc);
#endif
    HAL_ADC_Start_DMA(hadc,adc->	__info.convertedValue,adc ->channelsNumber);




    HAL_Delay(200);//这个延时是为了第一次读取adc就有值了  是给他转换时间的等待
    __JHAL_adcUpdateVoltageCalculationCoefficient(adc );
    return true;



}

bool JHAL_adcClose(JHAL_ADC *adc)
{


    myfree(   	adc->		__info.convertedValue);
    return false;
}

//  adc->adcInfosBuff 长度不需要包含verf 即可以比adc ->channelsNumber少一个
bool  JHAL_adcAqcMultiple (JHAL_ADC *adc)
{
    u8 channelsNumber=adc->channelsNumber;
    if(adc->vref==JHAL_ADC_ReferVoltage_BandGap) {
        channelsNumber--;
    }

    if(adc->samplingCount<3&& adc->filteredModel!=JHAL_FilteredModel_Median)
    {
        //主要为了兼容后面二维数组定义不会出现a[x][0]的错误  和增加滤波空间
        adc->samplingCount=3;
    }

    u32 adcBuff[channelsNumber][adc-> samplingCount];




    for(u8 j  =0; j< adc->samplingCount; j++)
    {
        HAL_Delay(10);
        for(u8 i=0; i< channelsNumber; i++)
        {



            adcBuff[i][j]=   (((u32*)adc->	__info.convertedValue)[i])&0xFFF ;

        }
    }



    for(u8 i=0; i< channelsNumber; i++)
    {


        JHAL_sortU32ArrayAsc( adcBuff[i], adc->samplingCount);

        adc->adcInfosBuff[i]. adcValue.minAD=adcBuff[i][0];
        adc->adcInfosBuff[i] .  adcValue.maxAD=adcBuff[i][adc->samplingCount-1];



        if( adc->filteredModel==JHAL_FilteredModel_Median) {

            adc->adcInfosBuff[i]. adcValue.ad= adcBuff[i][( adc->samplingCount-1)/2];
        } else {


            adc->adcInfosBuff[i]. adcValue.ad=adcBuff[i][1] ;

            for(u8 j=2; j< adc->samplingCount-1 ; j++)
            {


                adc->adcInfosBuff[i]. adcValue.ad =     adc->adcInfosBuff-> adcValue.ad + (adcBuff[i][j]-   adc->adcInfosBuff-> adcValue.ad )/adc->samplingCount ;

            }

        }


        adc->adcInfosBuff[i].minVoltage=	 adc->adcInfosBuff[i]. adcValue.minAD	*adc->__info.calculationCoefficient;

        adc->adcInfosBuff[i].voltage=	 adc->adcInfosBuff[i].adcValue.ad	*adc->__info.calculationCoefficient;

        adc->adcInfosBuff[i].maxVoltage=	 adc->adcInfosBuff[i].adcValue.maxAD	*adc->__info.calculationCoefficient;



    }








    return true;
}



#endif



