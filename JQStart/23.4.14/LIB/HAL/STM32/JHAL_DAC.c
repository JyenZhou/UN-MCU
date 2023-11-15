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
extern DAC_HandleTypeDef hdac;



#define DAC_VREF 3.3f




DAC_HandleTypeDef* __JHAL_jdac2dac(u8 dev)
{
    if(dev==0)
    {

        return &hdac;
    } else {
//不存在或未实现
        while(true);
    }
}




bool  JHAL_dacOpen( JHAL_DAC *dac)

{

    HAL_DAC_Start(&hdac,   DAC_CHANNEL_1);
    return true;
}




void JHAL_dacSetVoltage (JHAL_DAC *adc,float voltage)
{
    u32 dac=voltage*4096.0f/DAC_VREF;
    if(dac>4095)
    {
        dac=4095;
    }
    HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R,dac );



}

void JHAL_dacSetDA (JHAL_DAC *adc,u16 da)
{
   
    if(da>4095)
    {
        da=4095;
    }
    HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R,da );



}








#endif
