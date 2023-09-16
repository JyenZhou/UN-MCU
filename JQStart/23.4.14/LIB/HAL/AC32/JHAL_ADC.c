#include "../JHAL_ADC.h"

 
#include "ac780x_adc_reg.h"

 //BandGap最大、小AD值
const int SYS_BandGapAd_Min = 900, SYS_BandGapAd_Max = 1100;
/**  基准电压，单位：MV */
const float ADC_BANDGAP_VOLTAGE	=	1.2f  ;  
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

//有自动生成器所以初始化就不做通用配置  这样引脚的差异也无需处理
/* 
   void  JHAL_adcSingleModeInit(bool  isEN,u8* channel,u8 number)
{


    static bool sEN= {true};
    if(sEN!=isEN)
    {
        sEN=isEN;

        if(isEN)
        {
           ATC_ADC_Init();
        } else {

            ADC_DeInit(ADC0);
        }
    }

}
*/






//单通道采集
u32  JHAL_adcAqcSingle(u8 channel)

{
    /**  每次都需要软件触发采集数据 */
    ADC_SetLowPassFilter(ADC0,(ADC_ChannelType)channel,ADC_LPF_1200KHZ);
    ADC_SetRegularGroupChannel(ADC0, (ADC_ChannelType)channel, ADC_SPT_CLK_64, 0);
    ADC_SoftwareStartRegularConvert(ADC0);

    while(!ADC_GetIdleFlag(ADC0))
    {
        udelay(1);
    }
    return ADC_GetRegularData(ADC0);

}

