#ifndef __GATT__H__
#define __GATT__H__
#include "JHAL.h"
#ifdef __CplusPlus
extern "C" {
#endif



    typedef enum
    {

        AD_CH_4V4,

//���һ��������¼ͨ�����ȵ�
        ADC_CHANNEL_NUM
    } ADC_Channel_E;


    extern		u8 EN_GND0_JPin ;
    extern JHAL_IO_Port EN_GND0_GPIO_JPort ;
    extern u8 EN_GND1_JPin ;
    extern JHAL_IO_Port EN_GND1_GPIO_JPort ;
    extern u8 EN_GND2_JPin ;
    extern JHAL_IO_Port EN_GND2_GPIO_JPort ;
    extern u8 EN_GND3_JPin ;
    extern JHAL_IO_Port EN_GND3_GPIO_JPort ;
    extern u8 EN_GND4_JPin ;
    extern JHAL_IO_Port EN_GND4_GPIO_JPort ;
    extern u8 EN_GND5_JPin ;
    extern JHAL_IO_Port EN_GND5_GPIO_JPort ;
    extern u8 EN_GND6_JPin ;
    extern JHAL_IO_Port EN_GND6_GPIO_JPort ;
    extern u8 EN_GND7_JPin ;
    extern JHAL_IO_Port EN_GND7_GPIO_JPort ;
    extern u8 EN_GND8_JPin ;
    extern JHAL_IO_Port EN_GND8_GPIO_JPort ;
    extern u8 EN_GND9_JPin  ;
    extern JHAL_IO_Port EN_GND9_GPIO_JPort ;
    extern u8 SDA2_JPin ;
    extern JHAL_IO_Port SDA2_GPIO_JPort ;
    extern u8 SCL2_JPin ;
    extern JHAL_IO_Port SCL2_GPIO_JPort ;
    extern u8 SDA1_JPin ;
    extern JHAL_IO_Port SDA1_GPIO_JPort ;
    extern u8 SCL1_JPin ;
    extern JHAL_IO_Port SCL1_GPIO_JPort ;
    extern u8 EN_VCC9_JPin ;
    extern JHAL_IO_Port EN_VCC9_GPIO_JPort ;
    extern u8 EN_VCC8_JPin ;
    extern JHAL_IO_Port EN_VCC8_GPIO_JPort ;
    extern u8 EN_VCC7_JPin ;
    extern JHAL_IO_Port EN_VCC7_GPIO_JPort ;
    extern u8 EN_VCC6_JPin ;
    extern JHAL_IO_Port EN_VCC6_GPIO_JPort ;
    extern u8 EN_VCC5_JPin ;
    extern JHAL_IO_Port EN_VCC5_GPIO_JPort ;
    extern u8 EN_VCC4_JPin ;
    extern JHAL_IO_Port EN_VCC4_GPIO_JPort ;
    extern u8 EN_VCC3_JPin ;
    extern JHAL_IO_Port EN_VCC3_GPIO_JPort ;
    extern u8 EN_VCC2_JPin ;
    extern JHAL_IO_Port EN_VCC2_GPIO_JPort ;
    extern u8 EN_VCC1_JPin ;
    extern JHAL_IO_Port EN_VCC1_GPIO_JPort ;
    extern u8 EN_VCC0_JPin ;
    extern JHAL_IO_Port EN_VCC0_GPIO_JPort ;

    extern u8 SystemLed_JPin ;
    extern JHAL_IO_Port SystemLed_IO_Port ;


    extern  JHAL_ADC  adcConfig   ;
    extern  const JHAL_ADC SystemADC ;

    extern const JHAL_UART netUart ;

    extern u8  netUartData[ ];

    extern  JHAL_UART   uartConfig ;


    void gattInit(void);




#define ProductID 30









#ifdef CplusPlus
}
#endif

#endif


