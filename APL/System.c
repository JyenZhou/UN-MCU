#include "system.h"

#include "controller.h"
#include "communication.h"

#define  GASNumber4Row 3
#define  GASNumber4Column 10
#define GASNumber (GASNumber4Row*GASNumber4Column)

ADS1115   ads1115Config_row1_row2  ;
ADS1115  ads1115Config_row3;

u8 ads1115Status=0;


bool sendGASVoltage  ()
{


    float  vt[GASNumber];
    //��ʽ
    //{"id":"22222","vt":[1.11,1.222]}
    for(u8 column=0; column<GASNumber4Column; column++ )
    {




        vhRadioOpen(column);
        JHAL_delayMs(1000);

        JHAL_ADCInfo adcInfo;

        if(!  ads1115GetVoltage( &ads1115Config_row1_row2,ADS1115Channel_Diff_01,&adcInfo))
        {
            return false;
        }

        vt[column]=adcInfo.voltage*1000;

        if(! ads1115GetVoltage( &ads1115Config_row1_row2,ADS1115Channel_Diff_23,&adcInfo ))
        {
            return false;
        }

        vt[1*GASNumber4Column+column]=adcInfo.voltage*1000;
        if(! ads1115GetVoltage( &ads1115Config_row3,ADS1115Channel_Diff_01,&adcInfo))
        {
            return false;
        }
        vt[2*GASNumber4Column+column]= adcInfo.voltage*1000;

    }

    vhAllClose();
    sendGASVoltage2NetUart(vt,GASNumber,ads1115Status);
    return true;
}









void jSystemInit()
{

    gattInit();
    JHAL_adcClose( &adcConfig);

    JHAL_uartOpen( &uartConfig);
    //ADS1115��ʼ������
    ads1115Config_row3 .filteredModel= ads1115Config_row1_row2.  filteredModel=JHAL_FilteredModel_Median;
    ads1115Config_row3.isSingleConversion=ads1115Config_row1_row2.isSingleConversion=true;
    ads1115Config_row3.pga=ads1115Config_row1_row2.pga=ADS1115_PGA_02;
    ads1115Config_row3.speed=ads1115Config_row1_row2.speed=ADS1115_Speed_SPS_8;
    ads1115Config_row3.samplingCount4filter=ads1115Config_row1_row2.samplingCount4filter=5;

    ads1115Config_row3.i2c.delayMultiple=ads1115Config_row1_row2.i2c.delayMultiple=10;
    ads1115Config_row3.i2c.isIO_Mode_PP=	ads1115Config_row1_row2.i2c.isIO_Mode_PP=true;
    ads1115Config_row3.i2c.slaveAddress=ads1115Config_row1_row2.i2c.slaveAddress=ADS115_ADDRESS_GND ;

    ads1115Config_row1_row2.i2c.sclPort=SCL1_GPIO_JPort;
    ads1115Config_row1_row2.i2c.sclPin=SCL1_JPin;
    ads1115Config_row1_row2.i2c.sdaPort=SDA1_GPIO_JPort;
    ads1115Config_row1_row2.i2c.sdaPin=SDA1_JPin;

    ads1115Config_row3.i2c.sclPort=SCL2_GPIO_JPort;
    ads1115Config_row3.i2c.sclPin=SCL2_JPin;
    ads1115Config_row3.i2c.sdaPort=SDA2_GPIO_JPort;
    ads1115Config_row3.i2c.sdaPin=SDA2_JPin;



    if(!ads1115Open(&ads1115Config_row1_row2))
    {
        ads1115Status=1;
    }

    if(!ads1115Open(&ads1115Config_row3) )
    {
        ads1115Status+=2;
    }


    //��ģ��Ĺ����ʼ��
    controllerInit();


}



void jSystemRun()
{
    jSystemInit();
    while(true) {
        JHAL_gpioTogglePin(SystemLed_IO_Port,SystemLed_JPin);

        parseNetUartData();
        sendGASVoltage();


    }

}



