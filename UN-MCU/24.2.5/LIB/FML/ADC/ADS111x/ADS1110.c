
/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*                                                                     *


					坑  5V输入时不需要做IIC的电平转换 否则会读不到值




					 负电压处理
用于测量负电压，负电压从8000~ffff,负电压与正关于0有类似对称关系，按位取反后+1相同
if(data_get>=0x8000)
dianya=((float)(0xffff-data_get)/32768.0)*4.096;//0.000125
else
dianya=((float)data_get/32768.0)*4.096;

return dianya;
*                                                                     *
  *                                                                 *
   *                                                              *
*********@作者*Jyen******************@作者*Jyen***********************@作者*Jyen********************/
#include "ads1110.h"





// 单次转换开始
#define ads1110_REG_CONFIG_OS_START                     (0x1U << 7)//设备单词转换开启 高字节的最高位
#define ads1110_REG_CONFIG_OS_NULL                      (0x0U << 7)
//比较器模式
#define ads1110_REG_CONFIG_COMP_MODE_TRADITIONAL    (0x0U << 4)//默认
#define ads1110_REG_CONFIG_COMP_MODE_WINDOW         (0x1U << 4)
#define ads1110_REG_CONFIG_COMP_POL_LOW         (0x0U << 3)//默认
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
    /*指向ads1110指针寄存器用于准备读取数据*/
    uint8_t	readbuff[3];
	  u8 timeoutCount=10;


//通过SC寄存器判断是否转化完成 可以防止延时带来的不准 
  u8 regData  =ads1110_REG_CONFIG_OS_START  | config->isSingleConversion <<4|config->speed <<2|config->pga;
	 u32 timeoutCount2=0xFFFFFFF;
    while(1) {
			
			  if(  !JHAL_i2csfReceice(&config->i2c,(u8*)NULL,0,true,readbuff,3))
				{
					  if(timeoutCount--==0) {
            return false;
        }
					
				}
	 
					//0x20 SC寄存器位置
					  if(readbuff[2]== (0x7F&regData))
        {
            break;
        }else{
					
					 if(timeoutCount2--==0) {
            return false;
        }
				}
 
					
				}
			
    
  

    /*读取数据**/

 
 
    *conv_data=(readbuff[0]<<8)|readbuff[1];
 
    if(*conv_data > 0x8000)//负电压时候
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







