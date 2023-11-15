
/*************Jyen*************************Jyen********************Jyen***********************Jyen********************
   *                                                                *
 *                                                                   *
*
 		通用属性配置
当不同平台时API相同时参数不通用时
*                                                                     *
  *                                                                 *
   *                                                              *
*********Jyen******************Jyen************************Jyen*****************************************/

#include "GATT.h"





u8 SystemLed_JPin ;
JHAL_IO_Port SystemLed_IO_Port ;
u8 EN_GND0_JPin ;
JHAL_IO_Port EN_GND0_GPIO_JPort ;
u8 EN_GND1_JPin ;
JHAL_IO_Port EN_GND1_GPIO_JPort ;
u8 EN_GND2_JPin ;
JHAL_IO_Port EN_GND2_GPIO_JPort ;
u8 EN_GND3_JPin ;
JHAL_IO_Port EN_GND3_GPIO_JPort ;
u8 EN_GND4_JPin ;
JHAL_IO_Port EN_GND4_GPIO_JPort ;
u8 EN_GND5_JPin ;
JHAL_IO_Port EN_GND5_GPIO_JPort ;
u8 EN_GND6_JPin ;
JHAL_IO_Port EN_GND6_GPIO_JPort ;
u8 EN_GND7_JPin ;
JHAL_IO_Port EN_GND7_GPIO_JPort ;
u8 EN_GND8_JPin ;
JHAL_IO_Port EN_GND8_GPIO_JPort ;
u8 EN_GND9_JPin  ;
JHAL_IO_Port EN_GND9_GPIO_JPort ;
u8 SDA2_JPin ;
JHAL_IO_Port SDA2_GPIO_JPort ;
u8 SCL2_JPin ;
JHAL_IO_Port SCL2_GPIO_JPort ;
u8 SDA1_JPin ;
JHAL_IO_Port SDA1_GPIO_JPort ;
u8 SCL1_JPin ;
JHAL_IO_Port SCL1_GPIO_JPort ;
u8 EN_VCC9_JPin ;
JHAL_IO_Port EN_VCC9_GPIO_JPort ;
u8 EN_VCC8_JPin ;
JHAL_IO_Port EN_VCC8_GPIO_JPort ;
u8 EN_VCC7_JPin ;
JHAL_IO_Port EN_VCC7_GPIO_JPort ;
u8 EN_VCC6_JPin ;
JHAL_IO_Port EN_VCC6_GPIO_JPort ;
u8 EN_VCC5_JPin ;
JHAL_IO_Port EN_VCC5_GPIO_JPort ;
u8 EN_VCC4_JPin ;
JHAL_IO_Port EN_VCC4_GPIO_JPort ;
u8 EN_VCC3_JPin ;
JHAL_IO_Port EN_VCC3_GPIO_JPort ;
u8 EN_VCC2_JPin ;
JHAL_IO_Port EN_VCC2_GPIO_JPort ;
u8 EN_VCC1_JPin ;
JHAL_IO_Port EN_VCC1_GPIO_JPort ;
u8 EN_VCC0_JPin ;
JHAL_IO_Port EN_VCC0_GPIO_JPort ;



//传感器总电压
const   u8 gasVCVH_All=5;

u8 adcChannelRemap[ADC_CHANNEL_NUM]= {10 };


JHAL_ADC  adcConfig= {.vref=JHAL_ADC_ReferVoltage_BandGap, .channels=adcChannelRemap,.channelsNumber=ADC_CHANNEL_NUM,JHAL_DEV_INIT};



extern UART_HandleTypeDef huart2;
u8  netUartData[50];

JHAL_UART   uartConfig= {.uart=(void*) &huart2, . dev=1, .rxConfig={.  receiveMode=JHAL_UART_ReceiveITMode_IT_DMA_IDLE,.length=sizeof(netUartData),.dataBuff=netUartData}, .baudRate=115200};

#ifndef JBoot
void gattInit()
{

    //引脚映射
    EN_GND0_GPIO_JPort=JHAL_port2jport(EN_GND0_GPIO_Port);
    EN_GND0_JPin= JHAL_pin2jpin (EN_GND0_Pin);
    EN_GND1_GPIO_JPort=JHAL_port2jport(EN_GND1_GPIO_Port);
    EN_GND1_JPin= JHAL_pin2jpin (EN_GND1_Pin);
    EN_GND2_GPIO_JPort=JHAL_port2jport(EN_GND2_GPIO_Port);
    EN_GND2_JPin= JHAL_pin2jpin (EN_GND2_Pin);
    EN_GND3_GPIO_JPort=JHAL_port2jport(EN_GND3_GPIO_Port);
    EN_GND3_JPin= JHAL_pin2jpin (EN_GND3_Pin);
    EN_GND4_GPIO_JPort=JHAL_port2jport(EN_GND4_GPIO_Port);
    EN_GND4_JPin= JHAL_pin2jpin (EN_GND4_Pin);
    EN_GND5_GPIO_JPort=JHAL_port2jport(EN_GND5_GPIO_Port);
    EN_GND5_JPin= JHAL_pin2jpin (EN_GND5_Pin);
    EN_GND6_GPIO_JPort=JHAL_port2jport(EN_GND6_GPIO_Port);
    EN_GND6_JPin= JHAL_pin2jpin (EN_GND6_Pin);
    EN_GND7_GPIO_JPort=JHAL_port2jport(EN_GND7_GPIO_Port);
    EN_GND7_JPin= JHAL_pin2jpin (EN_GND7_Pin);
    EN_GND8_GPIO_JPort=JHAL_port2jport(EN_GND8_GPIO_Port);
    EN_GND8_JPin= JHAL_pin2jpin (EN_GND8_Pin);
    EN_GND9_GPIO_JPort=JHAL_port2jport(EN_GND9_GPIO_Port);
    EN_GND9_JPin= JHAL_pin2jpin (EN_GND9_Pin);

    EN_VCC0_GPIO_JPort=JHAL_port2jport(EN_VCC0_GPIO_Port);
    EN_VCC0_JPin= JHAL_pin2jpin (EN_VCC0_Pin);
    EN_VCC1_GPIO_JPort=JHAL_port2jport(EN_VCC1_GPIO_Port);
    EN_VCC1_JPin= JHAL_pin2jpin (EN_VCC1_Pin);
    EN_VCC2_GPIO_JPort=JHAL_port2jport(EN_VCC2_GPIO_Port);
    EN_VCC2_JPin= JHAL_pin2jpin (EN_VCC2_Pin);
    EN_VCC3_GPIO_JPort=JHAL_port2jport(EN_VCC3_GPIO_Port);
    EN_VCC3_JPin= JHAL_pin2jpin (EN_VCC3_Pin);
    EN_VCC4_GPIO_JPort=JHAL_port2jport(EN_VCC4_GPIO_Port);
    EN_VCC4_JPin= JHAL_pin2jpin (EN_VCC4_Pin);
    EN_VCC5_GPIO_JPort=JHAL_port2jport(EN_VCC5_GPIO_Port);
    EN_VCC5_JPin= JHAL_pin2jpin (EN_VCC5_Pin);
    EN_VCC6_GPIO_JPort=JHAL_port2jport(EN_VCC6_GPIO_Port);
    EN_VCC6_JPin= JHAL_pin2jpin (EN_VCC6_Pin);
    EN_VCC7_GPIO_JPort=JHAL_port2jport(EN_VCC7_GPIO_Port);
    EN_VCC7_JPin= JHAL_pin2jpin (EN_VCC7_Pin);
    EN_VCC8_GPIO_JPort=JHAL_port2jport(EN_VCC8_GPIO_Port);
    EN_VCC8_JPin= JHAL_pin2jpin (EN_VCC8_Pin);
    EN_VCC9_GPIO_JPort=JHAL_port2jport(EN_VCC9_GPIO_Port);
    EN_VCC9_JPin= JHAL_pin2jpin (EN_VCC9_Pin);

    SystemLed_IO_Port=JHAL_port2jport(SystemLed_GPIO_Port);
    SystemLed_JPin= JHAL_pin2jpin (SystemLed_Pin);



    SDA1_GPIO_JPort=JHAL_port2jport(SDA1_GPIO_Port);
    SDA1_JPin= JHAL_pin2jpin (SDA1_Pin);
    SDA2_GPIO_JPort=JHAL_port2jport(SDA2_GPIO_Port);
    SDA2_JPin= JHAL_pin2jpin (SDA2_Pin);

    SCL1_GPIO_JPort=JHAL_port2jport(SCL1_GPIO_Port);
    SCL1_JPin= JHAL_pin2jpin (SCL1_Pin);
    SCL2_GPIO_JPort=JHAL_port2jport(SCL2_GPIO_Port);
    SCL2_JPin= JHAL_pin2jpin (SCL2_Pin);



}

#endif
