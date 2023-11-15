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
extern DAC_HandleTypeDef hdac;



#define DAC_VREF 3.3f




DAC_HandleTypeDef* __JHAL_jdac2dac(u8 dev)
{
    if(dev==0)
    {

        return &hdac;
    } else {
//�����ڻ�δʵ��
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
