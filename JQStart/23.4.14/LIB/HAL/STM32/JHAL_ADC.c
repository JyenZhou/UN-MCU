#include "../JHAL_ADC.h"

/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*                                                                     *

CUBEMX ����
ADC
1.number of Conversion ѡ��Ҫ����ת����ͨ���������.hһ��(Ϊ�˼�������ĳ��� ֻ��һ���Ļ� ���ԼӸ��¶Ȼ��׼ͨ�� ʣ�µĲ��ù�)
2.continuos Conversion mode ->ENABLE ʹ������ת��ģʽ
3.ɨ��ת��   ��Ҫʹ��
3.����ת��ͨ����Rank

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
#ifdef HAL_ADC_MODULE_ENABLED
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;
//����12λ��ADC��3.3V��ADCֵΪ0xfff,�¶�Ϊ25��ʱ��Ӧ�ĵ�ѹֵΪ1.43V��0x6EE

#define VREFINT 1.2f
#define V25  0x6EE
//б�� ÿ���϶�4.3mV ��Ӧÿ���϶�0x05
#define AVG_SLOPE 0x05





ADC_HandleTypeDef* __JHAL_jadc2adc(u8 dev)
{
    if(dev==0)
    {

        return &hadc1;
    } else {
//�����ڻ�δʵ��
        while(true);
    }
}









/*
  ��ȡ��ѹ����ϵ��
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
        adc->__info.calculationCoefficient=1;
        break;
    }

}




/*------------------Jyen--------------------------Jyen-----------------------*/
/*******************************************************************************
*@ �������ܻ���: ��ͨ�����βɼ�ģʽ��ʼ��
 * @�������:
isEN  true��ʼ��  false����ʼ��
channel ��Ӧͨ��������
number  Ҫ��ʼ��ͨ���ĸ���
 * @�� �� ֵ: ��
 * @��ע:���ʹ��bandgap������׼ adc ->channelsNumber������Ҫ����verf
 *
 *
 ******************************************************************************
 *
 *------------------Jyen-------------------------Jyen-------------------------*/



bool  JHAL_adcOpen(JHAL_ADC *adc )
{


    if((		adc->	__info.convertedValue=(u32*)mymalloc(adc->channelsNumber*sizeof(u32)))==NULL)
    {
        //�ڴ����ʧ�� ;
        while(true)  ;
    }

    ADC_HandleTypeDef*  hadc=__JHAL_jadc2adc(  adc->dev);
#ifndef STM32F407xx
    HAL_ADCEx_Calibration_Start(hadc);
#endif
    HAL_ADC_Start_DMA(hadc,adc->	__info.convertedValue,adc ->channelsNumber);




    HAL_Delay(200);//�����ʱ��Ϊ�˵�һ�ζ�ȡadc����ֵ��  �Ǹ���ת��ʱ��ĵȴ�
    __JHAL_adcUpdateVoltageCalculationCoefficient(adc );
    return true;



}

bool JHAL_adcClose(JHAL_ADC *adc)
{


    myfree(   	adc->		__info.convertedValue);
    return false;
}

//  adc->adcInfosBuff ���Ȳ���Ҫ����verf �����Ա�adc ->channelsNumber��һ��
bool  JHAL_adcAqcMultiple (JHAL_ADC *adc)
{
    u8 channelsNumber=adc->channelsNumber;
    if(adc->vref==JHAL_ADC_ReferVoltage_BandGap) {
        channelsNumber--;
    }

    if(adc->samplingCount<3&& adc->filteredModel!=JHAL_FilteredModel_Median)
    {
        //��ҪΪ�˼��ݺ����ά���鶨�岻�����a[x][0]�Ĵ���  �������˲��ռ�
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



