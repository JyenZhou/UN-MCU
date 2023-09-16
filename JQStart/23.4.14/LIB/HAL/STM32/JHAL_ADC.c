#include "../JHAL_ADC.h"
 
/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*                                                                     *

CUBEMX 配置
ADC
1.number of Conversion 选择要连续转换的通道数与这儿.h一致(为了兼容这里的程序 只有一个的话 可以加个温度或基准通道 剩下的不用管)
2.continuos Conversion mode ->ENABLE 使能连续转换模式
3.扫描转换 连续转换 需要使能
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

extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;
//对于12位的ADC，3.3V的ADC值为0xfff,温度为25度时对应的电压值为1.43V即0x6EE

#define VREFINT 1.2f
#define V25  0x6EE
    //斜率 每摄氏度4.3mV 对应每摄氏度0x05
#define AVG_SLOPE 0x05
uint32_t  *__JHAL_ADC_ConvertedValue= NULL ;


 
#define JHAL_ADC_AVERAGE_TIMES_MAX   11    /**  ADC滤波队列长度 要大于5 */
static JHAL_ADCConfig *__adcConfig[JHAL_ADC_Number];


ADC_HandleTypeDef* __JHAL_jadc2adc(JHAL_ADC adc)
{
    if(adc==JHAL_ADC0)
    {

        return &hadc1;
    } else {
//不存在或未实现
        while(true);
    }
}








 
/*
  获取电压计算系数
*/




void  __JHAL_adcUpdateVoltageCalculationCoefficient( JHAL_ADCConfig *config)
{
 
    switch(config->vref)
    {
    case  JHAL_ADC_ReferVoltage_BandGap:
        config->__otherInfo. calculationCoefficient=VREFINT/ __JHAL_ADC_ConvertedValue[(config->channelsNumber)-1];
        break;
    case  JHAL_ADC_ReferVoltage_VDD:
    case  JHAL_ADC_ReferVoltage_Vref:
        config->__otherInfo.calculationCoefficient=config->vrefVoltageValue/4095;
        break;
		case	JHAL_ADC_ReferVoltage_NONE:
			 config->__otherInfo.calculationCoefficient=1;
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



bool  JHAL_adcInit(JHAL_ADC jadc,JHAL_ADCConfig *config )
{
    if(__JHAL_adcIsENCheak(jadc,true)) {
      
			__JHAL_ADC_ConvertedValue=(u32*)mymalloc(config->channelsNumber*sizeof(u32));
			 if (__JHAL_ADC_ConvertedValue == NULL) {
      //内存分配失败 ;
        while(true)  ;
    }
			    __adcConfig[jadc]=config;
		ADC_HandleTypeDef*  hadc=__JHAL_jadc2adc(  jadc);
	  HAL_ADCEx_Calibration_Start(hadc);
    HAL_ADC_Start_DMA(hadc,__JHAL_ADC_ConvertedValue,config->channelsNumber);
		
 

		
    HAL_Delay(200);//这个延时是为了第一次读取adc就有值了  是给他转换时间的等待
         __JHAL_adcUpdateVoltageCalculationCoefficient(config);
        return true;
    }
		
    return false;
}
   
bool JHAL_adcDeInit(JHAL_ADC jadc)
{
 
	
    myfree(__JHAL_ADC_ConvertedValue);
    return false;
}

 

bool  JHAL_adcAqcMultiple (JHAL_ADC jadc )
{
 u8 channelsNumber=__adcConfig[jadc]->channelsNumber;
	if(__adcConfig[jadc]->vref==JHAL_ADC_ReferVoltage_BandGap){
		channelsNumber--;
	}
   
    for(u8 i=0; i< channelsNumber; i++)
    {
       
			
			  u8 samplingCount=  __adcConfig[jadc]->  samplingCount;
    if(samplingCount>1)
    {

        u32 adcBuff[channelsNumber][ samplingCount];

        for(u8 j  =0; j< samplingCount; j++)
        {
       adcBuff[i][j]=__JHAL_ADC_ConvertedValue[i] ;
		  HAL_Delay(1);
        }


        JHAL_sortU32ArrayAsc( adcBuff[i], samplingCount);

         __adcConfig[jadc]->adcInfosBuff[i]. adcValue.minAD=adcBuff[i][0];
         __adcConfig[jadc]->adcInfosBuff[i] .  adcValue.maxAD=adcBuff[i][samplingCount-1];



        if( __adcConfig[jadc]->filteredModel==JHAL_FilteredModel_Median) {

             __adcConfig[jadc]->adcInfosBuff->  adcValue.ad= adcBuff[i][( samplingCount+1)/2];
        } else {
            samplingCount-=1;

            __adcConfig[jadc]->adcInfosBuff-> adcValue.ad=adcBuff[i][1] ;

            for(u8 j=2; j< samplingCount ; j++)
            {


                __adcConfig[jadc]->adcInfosBuff-> adcValue.ad =     __adcConfig[jadc]->adcInfosBuff-> adcValue.ad + (adcBuff[i][j]-   __adcConfig[jadc]->adcInfosBuff-> adcValue.ad )/samplingCount ;

            }

        }
			
						 __adcConfig[jadc]->adcInfosBuff->minVoltage=	 __adcConfig[jadc]->adcInfosBuff-> adcValue.minAD	*__adcConfig[jadc]->__otherInfo.calculationCoefficient;

						 __adcConfig[jadc]->adcInfosBuff->maxVoltage=	 __adcConfig[jadc]->adcInfosBuff-> adcValue.maxAD	*__adcConfig[jadc]->__otherInfo.calculationCoefficient;


    }else{
		 __adcConfig[jadc]->adcInfosBuff-> adcValue.ad=__JHAL_ADC_ConvertedValue[i] ;
			
		}
				 __adcConfig[jadc]->adcInfosBuff->voltage=	 __adcConfig[jadc]->adcInfosBuff-> adcValue.ad	*__adcConfig[jadc]->__otherInfo.calculationCoefficient;

		
	}
     
		
    return true;
}







