#include "../JHAL_DAC.h"

/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*                                                                     *

CUBEMX 配置
ADC
1.Scan Conversion Mode 使能
2.continuos Conversion mode  使能
3.DMA Conversion Requests    使能(f4上面有  f1不记得了)
4. number of Conversion 选择要连续转换的通道数与这儿.h一致(为了兼容这里的程序 只有一个的话 可以加个温度或基准通道 剩下的不用管)
5.配置转换通道数Rank

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


#ifdef HAL_DAC_MODULE_ENABLED




#define DAC_VREF 3.3f








bool  JHAL_dacOpen( JHAL_DAC *dac)

{
    for(u8 channelIndex=0; channelIndex<dac->channelsNumber; channelIndex++)
    {
        //TIM_CHANNEL_ALL  实测cubemx 1.8.5在pwm中无任何输出 不知道是不是bug
        if(dac->channels[channelIndex]==DAC_CHANNEL_1 || dac->channels[channelIndex]==DAC_CHANNEL_2    )
        {

            HAL_DAC_Start((DAC_HandleTypeDef*)(dac->dev),   dac->channels[channelIndex]);
        } else {
            JHAL_error("JHAL_dacOpen");
            return false;
        }

    }



    return true;
}



void JHAL_dacSetVoltage (JHAL_DAC *dac,u8 channelIndex,float voltage)
{
    u32 dacValue=voltage*4096.0f/DAC_VREF;
    if(dacValue>4095)
    {
        dacValue=4095;
    }
    HAL_DAC_SetValue((DAC_HandleTypeDef*)(dac->dev),  dac->channels[channelIndex], DAC_ALIGN_12B_R,dacValue );



}

void JHAL_dacSetDA (JHAL_DAC *dac,u8 channelIndex,u16 da)
{

    if(da>4095)
    {
        da=4095;
    }
    HAL_DAC_SetValue((DAC_HandleTypeDef*)(dac->dev),  dac->channels[channelIndex], DAC_ALIGN_12B_R,da );



}








#endif
