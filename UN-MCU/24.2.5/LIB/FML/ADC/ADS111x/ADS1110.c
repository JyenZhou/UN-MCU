
/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*                                                                     *


					��  5V����ʱ����Ҫ��IIC�ĵ�ƽת�� ����������ֵ




					 ����ѹ����
���ڲ�������ѹ������ѹ��8000~ffff,����ѹ��������0�����ƶԳƹ�ϵ����λȡ����+1��ͬ
if(data_get>=0x8000)
dianya=((float)(0xffff-data_get)/32768.0)*4.096;//0.000125
else
dianya=((float)data_get/32768.0)*4.096;

return dianya;
*                                                                     *
  *                                                                 *
   *                                                              *
*********@����*Jyen******************@����*Jyen***********************@����*Jyen********************/
#include "ads1110.h"





// ����ת����ʼ
#define ads1110_REG_CONFIG_OS_START                     (0x1U << 7)//�豸����ת������ ���ֽڵ����λ
#define ads1110_REG_CONFIG_OS_NULL                      (0x0U << 7)
//�Ƚ���ģʽ
#define ads1110_REG_CONFIG_COMP_MODE_TRADITIONAL    (0x0U << 4)//Ĭ��
#define ads1110_REG_CONFIG_COMP_MODE_WINDOW         (0x1U << 4)
#define ads1110_REG_CONFIG_COMP_POL_LOW         (0x0U << 3)//Ĭ��
#define ads1110_REG_CONFIG_COMP_POL_HIG         (0x1U << 3)
#define ads1110_REG_CONFIG_COMP_LAT_NONLATCH        (0x0U << 2)
#define ads1110_REG_CONFIG_COMP_LAT_LATCH           (0x1U << 2)
#define ads1110_REG_CONFIG_COMP_QUE_ONE         (0x0U << 0)
#define ads1110_REG_CONFIG_COMP_QUE_TWO         (0x1U << 0)
#define ads1110_REG_CONFIG_COMP_QUE_THR         (0x2U << 0)
#define ads1110_REG_CONFIG_COMP_QUE_DIS         (0x3U << 0)




 

bool  __ads1110Init( ADS1110  *config )
{



    u8 regData  =ads1110_REG_CONFIG_OS_START  | config->isSingleConversion <<4|config->speed <<2|config->pga;
  

    u8 timeoutCount=10;

    while(!JHAL_i2csfTransmit(&config->i2c,&regData,1))
    {
        if(timeoutCount--==0) {
            return false;
        }
    }

    return true;
}

bool  ads1110Open( ADS1110  *config)
{
    JHAL_i2csfOpen(	&config->i2c);


    return     true;
}

bool __ads1110GetAD(ADS1110  *config, int32 *conv_data )

{
    /*ָ��ads1110ָ��Ĵ�������׼����ȡ����*/
    uint8_t	readbuff[3];
	  u8 timeoutCount=10;


//ͨ��SC�Ĵ����ж��Ƿ�ת����� ���Է�ֹ��ʱ�����Ĳ�׼ 
  u8 regData  =ads1110_REG_CONFIG_OS_START  | config->isSingleConversion <<4|config->speed <<2|config->pga;
	 u32 timeoutCount2=0xFFFFFFF;
    while(1) {
			
			  if(  !JHAL_i2csfReceice(&config->i2c,(u8*)NULL,0,true,readbuff,3))
				{
					  if(timeoutCount--==0) {
            return false;
        }
					
				}
	 
					//0x20 SC�Ĵ���λ��
					  if(readbuff[2]== (0x7F&regData))
        {
            break;
        }else{
					
					 if(timeoutCount2--==0) {
            return false;
        }
				}
 
					
				}
			
    
  

    /*��ȡ����**/

 
 
    *conv_data=(readbuff[0]<<8)|readbuff[1];
 
    if(*conv_data > 0x8000)//����ѹʱ��
    {
        *conv_data =-(0xffff-*conv_data);
			
    }
 
		
		 
    return true ;
 
}


bool ads1110GetVoltage(ADS1110  *config,  JHAL_ADCInfo  *adcInfo  )

{
 
    if(config->isSingleConversion)
    {
        __ads1110Init(config );

    }
 
    u8 samplingCount=config->samplingCount4filter;
    if(samplingCount>1)
    {

        int32  adcBuff[ samplingCount];

        for(u8 i=0; i< samplingCount; i++)
        {
            if(! __ads1110GetAD(config,&adcBuff[i]))
            {
                return false;
            }
        }


        JHAL_sort32ArrayAsc( adcBuff, samplingCount);

        adcInfo->adcValue.minAD=adcBuff[0];
        adcInfo->adcValue.maxAD=adcBuff[samplingCount-1];



        if(config->filteredModel==JHAL_FilteredModel_Median) {

            adcInfo->adcValue.ad= adcBuff[( samplingCount-1)/2];
        } else {
            samplingCount-=1;

            adcInfo->adcValue.ad=adcBuff[1] ;

            for(u8 i=2; i< samplingCount ; i++)
            {

                adcInfo->adcValue.ad =    adcInfo->adcValue.ad + (adcBuff[i]-  adcInfo->adcValue.ad )/samplingCount ;

            }

        }

    } else {



        if(! __ads1110GetAD(config,& adcInfo->adcValue.ad))
        {
            return false;
        }

    }
 
    if(config->pga==ADS1110_PGA1_2V048) {
        adcInfo->voltage=adcInfo->adcValue.ad*0.0000625;
        adcInfo->maxVoltage=adcInfo->adcValue.maxAD*0.0000625;
        adcInfo->minVoltage=adcInfo->adcValue.minAD*0.0000625;

    } else if(config->pga==ADS1110_PGA2_1V024) {
        adcInfo->voltage=adcInfo->adcValue.ad*0.00003125;
        adcInfo->maxVoltage=adcInfo->adcValue.maxAD*0.00003125;
        adcInfo->minVoltage=adcInfo->adcValue.minAD*0.00003125;
    } else if(config->pga==ADS1110_PGA4_0V512) {
        adcInfo->voltage=adcInfo->adcValue.ad*0.000015625;
        adcInfo->maxVoltage=adcInfo->adcValue.maxAD*0.000015625;
        adcInfo->minVoltage=adcInfo->adcValue.minAD*0.000015625;
    } else {
        adcInfo->voltage=adcInfo->adcValue.ad*0.0000078125;
        adcInfo->maxVoltage=adcInfo->adcValue.maxAD*0.0000078125;
        adcInfo->minVoltage=adcInfo->adcValue.minAD*0.0000078125;
    }
 
    return true;
 
}







