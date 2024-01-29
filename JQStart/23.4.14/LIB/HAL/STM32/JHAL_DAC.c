#include "../JHAL_DAC.h"

/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*                                                                     *

CUBEMX ����
ADC
1.Scan Conversion Mode ʹ��
2.continuos Conversion mode  ʹ��
3.DMA Conversion Requests    ʹ��(f4������  f1���ǵ���)
4. number of Conversion ѡ��Ҫ����ת����ͨ���������.hһ��(Ϊ�˼�������ĳ��� ֻ��һ���Ļ� ���ԼӸ��¶Ȼ��׼ͨ�� ʣ�µĲ��ù�)
5.����ת��ͨ����Rank

DMA
1.DMA-->Mode-->Circular
2.DMA-->Data Width-->word


            16ͨ��ADC1+DMA����
						 ADC��ʼ�� һ��Ҫ��ʱ�ٶ�ȡ  ��Ȼ��һ�ζ����ǿ�ֵ  ֮����Ķ�����һ��ֵ

						 	 //ת��Ϊ�¶�ֵ,ʵ��Ӧ����,�ɿ����ú���Ϊ��λ,���⸡������
						   float  TEMP=	(1.43 - (ad*3.3/4096))/0.0043 + 25 = 55.23

*                                                                     *
  *                                                                 *
   *                                                              *
*********@����*Jyen******************@����*Jyen***********************@����*Jyen********************/


#ifdef HAL_DAC_MODULE_ENABLED
 



#define DAC_VREF 3.3f




 



bool  JHAL_dacOpen( JHAL_DAC *dac)

{
		for(u8 channelIndex=0;channelIndex<dac->channelsNumber;channelIndex++)
			{
				//TIM_CHANNEL_ALL  ʵ��cubemx 1.8.5��pwm�����κ���� ��֪���ǲ���bug
				if(dac->channels[channelIndex]==DAC_CHANNEL_1 || dac->channels[channelIndex]==DAC_CHANNEL_2    )
				{
					
					 	    HAL_DAC_Start((DAC_HandleTypeDef*)(dac->dev),   dac->channels[channelIndex]);
				}else{
					   JHAL_Fault_Handler("JHAL_dacOpen");
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
