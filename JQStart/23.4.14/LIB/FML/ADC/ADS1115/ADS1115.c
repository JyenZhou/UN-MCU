
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
#include "ads1115.h"





// 单次转换开始
#define ADS1115_REG_CONFIG_OS_START                     (0x1U << 7)//设备单词转换开启 高字节的最高位
#define ADS1115_REG_CONFIG_OS_NULL                      (0x0U << 7)
//比较器模式
#define ADS1115_REG_CONFIG_COMP_MODE_TRADITIONAL    (0x0U << 4)//默认
#define ADS1115_REG_CONFIG_COMP_MODE_WINDOW         (0x1U << 4)
#define ADS1115_REG_CONFIG_COMP_POL_LOW         (0x0U << 3)//默认
#define ADS1115_REG_CONFIG_COMP_POL_HIG         (0x1U << 3)
#define ADS1115_REG_CONFIG_COMP_LAT_NONLATCH        (0x0U << 2)
#define ADS1115_REG_CONFIG_COMP_LAT_LATCH           (0x1U << 2)
#define ADS1115_REG_CONFIG_COMP_QUE_ONE         (0x0U << 0)
#define ADS1115_REG_CONFIG_COMP_QUE_TWO         (0x1U << 0)
#define ADS1115_REG_CONFIG_COMP_QUE_THR         (0x2U << 0)
#define ADS1115_REG_CONFIG_COMP_QUE_DIS         (0x3U << 0)



bool  __ads115Init( ADS1115Config  *config,  ADS1115Channel ch )
{



    u8 regData[2];
    regData[0]=ADS1115_REG_CONFIG_OS_START| ch|config->pga|config->isSingleConversion;
    regData 	[1]=config->speed|ADS1115_REG_CONFIG_COMP_MODE_TRADITIONAL|ADS1115_REG_CONFIG_COMP_POL_LOW|ADS1115_REG_CONFIG_COMP_LAT_NONLATCH|ADS1115_REG_CONFIG_COMP_QUE_DIS;

    u8 timeoutCount=10;

    while(!JHAL_iicTransmit(&config->iicConfig,regData,2))
    {
        if(timeoutCount--==0) {
            return false;
        }
    }


    JHAL_delayMs(10);
    return false;
}

bool  ads1115Init( ADS1115Config  *config)
{
    JHAL_iicInit(	&config->iicConfig);
    return  __ads115Init(config,ADS1115Channel_INGLE_0);
}

uint32_t __ads1115GetAD(ADS1115Config  *config  )

{


    /*指向ADS1115指针寄存器用于准备读取数据*/
    uint8_t	readbuff[2];
    uint32_t conv_data = 0;



    /*读取数据**/

    JHAL_iicReceice(&config->iicConfig,readbuff,2);

    conv_data=(readbuff[0]<<8)|readbuff[1];

    if(conv_data > 0x8000)//负电压时候
    {
        conv_data = 0xffff-conv_data;
    }

    if(config->isSingleConversion)
    {
        while(JHAL_gpioReadPin(config->alert_rdy_Port,config->alert_rdy_Pin));

    } else {
        JHAL_delayMs(10);
    }

    return conv_data;

}




JHAL_ADCInfo ads1115GetVoltage(ADS1115Config  *config,ADS1115Channel ch )

{
    JHAL_ADCInfo  adcInfo ;

    if(config->isSingleConversion)
    {
        __ads115Init(config,ch);
    }





    u8 samplingCount=config->samplingCount4filter;
    if(samplingCount>1)
    {

        u32 adcBuff[ samplingCount];

        for(u8 i=0; i< samplingCount; i++)
        {
            adcBuff[i]= __ads1115GetAD(config);
        }


        JHAL_sortU32ArrayAsc( adcBuff, samplingCount);

        adcInfo.adcValue.minAD=adcBuff[0];
        adcInfo.adcValue.maxAD=adcBuff[samplingCount-1];



        if(config->filteredModel==JHAL_FilteredModel_Median) {

            adcInfo.adcValue.ad= adcBuff[( samplingCount+1)/2];
        } else {
            samplingCount-=1;

            adcInfo.adcValue.ad=adcBuff[1] ;

            for(u8 i=2; i< samplingCount ; i++)
            {


                adcInfo.adcValue.ad =    adcInfo.adcValue.ad + (adcBuff[i]-  adcInfo.adcValue.ad )/samplingCount ;

            }

        }

    } else {
        adcInfo.adcValue.ad= __ads1115GetAD(config);
    }



    if(config->pga==ADS1115_PGA_6) {
        adcInfo.voltage=adcInfo.adcValue.ad*0.0001875;
        adcInfo.maxVoltage=adcInfo.adcValue.maxAD*0.0001875;
        adcInfo.minVoltage=adcInfo.adcValue.minAD*0.0001875;
    } else if(config->pga==ADS1115_PGA_4) {
        adcInfo.voltage=adcInfo.adcValue.ad*0.000125;
        adcInfo.maxVoltage=adcInfo.adcValue.maxAD*0.000125;
        adcInfo.minVoltage=adcInfo.adcValue.minAD*0.000125;

    } else if(config->pga==ADS1115_PGA_2) {
        adcInfo.voltage=adcInfo.adcValue.ad*0.0000625;
        adcInfo.maxVoltage=adcInfo.adcValue.maxAD*0.0000625;
        adcInfo.minVoltage=adcInfo.adcValue.minAD*0.0000625;

    } else if(config->pga==ADS1115_PGA_1) {
        adcInfo.voltage=adcInfo.adcValue.ad*0.00003125;
        adcInfo.maxVoltage=adcInfo.adcValue.maxAD*0.00003125;
        adcInfo.minVoltage=adcInfo.adcValue.minAD*0.00003125;
    } else if(config->pga==ADS1115_PGA_05) {
        adcInfo.voltage=adcInfo.adcValue.ad*0.000015625;
        adcInfo.maxVoltage=adcInfo.adcValue.maxAD*0.000015625;
        adcInfo.minVoltage=adcInfo.adcValue.minAD*0.000015625;
    } else {
        adcInfo.voltage=adcInfo.adcValue.ad*0.0000078125;
        adcInfo.maxVoltage=adcInfo.adcValue.maxAD*0.0000078125;
        adcInfo.minVoltage=adcInfo.adcValue.minAD*0.0000078125;
    }





    return adcInfo;


}





 

