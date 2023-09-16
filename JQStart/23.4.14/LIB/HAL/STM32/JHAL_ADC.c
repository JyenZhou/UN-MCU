#include "../JHAL_ADC.h"
 
/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*                                                                     *

CUBEMX ����
ADC
1.number of Conversion ѡ��Ҫ����ת����ͨ���������.hһ��(Ϊ�˼�������ĳ��� ֻ��һ���Ļ� ���ԼӸ��¶Ȼ��׼ͨ�� ʣ�µĲ��ù�)
2.continuos Conversion mode ->ENABLE ʹ������ת��ģʽ
3.ɨ��ת�� ����ת�� ��Ҫʹ��
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

extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;
//����12λ��ADC��3.3V��ADCֵΪ0xfff,�¶�Ϊ25��ʱ��Ӧ�ĵ�ѹֵΪ1.43V��0x6EE

#define VREFINT 1.2f
#define V25  0x6EE
    //б�� ÿ���϶�4.3mV ��Ӧÿ���϶�0x05
#define AVG_SLOPE 0x05
uint32_t  *__JHAL_ADC_ConvertedValue= NULL ;


 
#define JHAL_ADC_AVERAGE_TIMES_MAX   11    /**  ADC�˲����г��� Ҫ����5 */
static JHAL_ADCConfig *__adcConfig[JHAL_ADC_Number];


ADC_HandleTypeDef* __JHAL_jadc2adc(JHAL_ADC adc)
{
    if(adc==JHAL_ADC0)
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
*@ �������ܻ���: ��ͨ�����βɼ�ģʽ��ʼ��
 * @�������:
isEN  true��ʼ��  false����ʼ��
channel ��Ӧͨ��������
number  Ҫ��ʼ��ͨ���ĸ���
 * @�� �� ֵ: ��
 * @��ע:
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
      //�ڴ����ʧ�� ;
        while(true)  ;
    }
			    __adcConfig[jadc]=config;
		ADC_HandleTypeDef*  hadc=__JHAL_jadc2adc(  jadc);
	  HAL_ADCEx_Calibration_Start(hadc);
    HAL_ADC_Start_DMA(hadc,__JHAL_ADC_ConvertedValue,config->channelsNumber);
		
 

		
    HAL_Delay(200);//�����ʱ��Ϊ�˵�һ�ζ�ȡadc����ֵ��  �Ǹ���ת��ʱ��ĵȴ�
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







