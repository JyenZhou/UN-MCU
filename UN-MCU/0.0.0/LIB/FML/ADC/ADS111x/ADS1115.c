
/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*                                                                     *

��������� sgm58031

					��  5V����ʱ����Ҫ��IIC�ĵ�ƽת�� ����������ֵ(Ӧ���ǰ弶�������  ��iicӦ�ü���3.3vΪ�ߵ�ƽ)




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
#include "ads1115.h"





// ����ת����ʼ
#define ADS1115_REG_CONFIG_OS_START                     (0x1U << 7)//�豸����ת������ ���ֽڵ����λ
#define ADS1115_REG_CONFIG_OS_NULL                      (0x0U << 7)
//�Ƚ���ģʽ
#define ADS1115_REG_CONFIG_COMP_MODE_TRADITIONAL    (0x0U << 4)//Ĭ��
#define ADS1115_REG_CONFIG_COMP_MODE_WINDOW         (0x1U << 4)
#define ADS1115_REG_CONFIG_COMP_POL_LOW         (0x0U << 3)//Ĭ��
#define ADS1115_REG_CONFIG_COMP_POL_HIG         (0x1U << 3)
#define ADS1115_REG_CONFIG_COMP_LAT_NONLATCH        (0x0U << 2)
#define ADS1115_REG_CONFIG_COMP_LAT_LATCH           (0x1U << 2)
#define ADS1115_REG_CONFIG_COMP_QUE_ONE         (0x0U << 0)
#define ADS1115_REG_CONFIG_COMP_QUE_TWO         (0x1U << 0)
#define ADS1115_REG_CONFIG_COMP_QUE_THR         (0x2U << 0)
#define ADS1115_REG_CONFIG_COMP_QUE_DIS         (0x3U << 0)



 

bool  __ads115Init( ADS1115  *config,  ADS1115Channel ch )
{



    u8 regData[2];
    regData[0]=ADS1115_REG_CONFIG_OS_START| ch|config->pga|config->isSingleConversion;
    regData 	[1]= config->speed <<5|ADS1115_REG_CONFIG_COMP_MODE_TRADITIONAL|ADS1115_REG_CONFIG_COMP_POL_LOW|ADS1115_REG_CONFIG_COMP_LAT_NONLATCH|ADS1115_REG_CONFIG_COMP_QUE_DIS;

    u8 timeoutCount=10;

    while(!JHAL_i2csfMemWrite(&config->i2c,1,false,regData,2))
    {
        if(timeoutCount--==0) {
            return false;
        }
    }

    return true;
}

bool  ads1115Open( ADS1115  *config)
{
    JHAL_i2csfOpen(	&config->i2c);


    return     true;
}

bool __ads1115GetAD(ADS1115  *config, int32 *conv_data )

{
    /*ָ��ADS1115ָ��Ĵ�������׼����ȡ����*/
    uint8_t	readbuff[2];
	   u32 timeoutCount=0X7FFFFF ;

//ͨ��OS�Ĵ����ж��Ƿ�ת����� ���Է�ֹ��ʱ�����Ĳ�׼ ��Ҫ�Ǽ���sgm58031 ʹ��1115��������ʱ����

    while(1) {
		 
        if(  !JHAL_i2csfMemRead(&config->i2c,1,false,readbuff,1))
        {
            return false ;
        }
        JHAL_delayOsMs(1);
        if(readbuff[0]&0x80)
        {
            break;
        }
				if(timeoutCount--==0)
				{
					return false ;
				}
 
    }

    /*��ȡ����**/

    if(  !JHAL_i2csfMemRead(&config->i2c,0,false,readbuff,2))
    {
        return false ;
    }
 
    *conv_data=(readbuff[0]<<8)|readbuff[1];
 
    if(*conv_data > 0x8000)//����ѹʱ��
    {
        *conv_data =-(0xffff-*conv_data);
			
    }
 
    return true ;
 
}


bool ads1115GetVoltage(ADS1115  *config,ADS1115Channel ch, JHAL_ADCInfo  *adcInfo  )

{
 
    if(config->isSingleConversion)
    {
        __ads115Init(config,ch);

    }
 
    u8 samplingCount=config->samplingCount4filter;
    if(samplingCount>1)
    {

        int32  adcBuff[ samplingCount];

        for(u8 i=0; i< samplingCount; i++)
        {
            if(! __ads1115GetAD(config,&adcBuff[i]))
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



        if(! __ads1115GetAD(config,& adcInfo->adcValue.ad))
        {
            return false;
        }

    }
 
    if(config->pga==ADS1115_PGA_6) {
        adcInfo->voltage=adcInfo->adcValue.ad*0.0001875;
        adcInfo->maxVoltage=adcInfo->adcValue.maxAD*0.0001875;
        adcInfo->minVoltage=adcInfo->adcValue.minAD*0.0001875;
    } else if(config->pga==ADS1115_PGA_4) {
        adcInfo->voltage=adcInfo->adcValue.ad*0.000125;
        adcInfo->maxVoltage=adcInfo->adcValue.maxAD*0.000125;
        adcInfo->minVoltage=adcInfo->adcValue.minAD*0.000125;

    } else if(config->pga==ADS1115_PGA_2) {
        adcInfo->voltage=adcInfo->adcValue.ad*0.0000625;
        adcInfo->maxVoltage=adcInfo->adcValue.maxAD*0.0000625;
        adcInfo->minVoltage=adcInfo->adcValue.minAD*0.0000625;

    } else if(config->pga==ADS1115_PGA_1) {
        adcInfo->voltage=adcInfo->adcValue.ad*0.00003125;
        adcInfo->maxVoltage=adcInfo->adcValue.maxAD*0.00003125;
        adcInfo->minVoltage=adcInfo->adcValue.minAD*0.00003125;
    } else if(config->pga==ADS1115_PGA_05) {
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







