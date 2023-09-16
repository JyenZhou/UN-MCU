#ifndef _ads_1115
#define _ads_1115

#include "JHAL.h"

#define Hdata_CH0 0xc2
#define Hdata_CH1 0xd2
#define Hdata_CH2 0xe2
#define Hdata_CH3 0xf2


#define Ldata 0xc3

#define ADS115_ADDRESS_GND  0x90  //ADDR PIN ->GND
#define  ADS115_ADDRESS_VDD  0x92  //ADDR PIN ->VDD
#define  ADS115_ADDRESS_SDA 0x94  //ADDR PIN ->SDA
#define  ADS115_ADDRESS_SCL  0x96  //ADDR PIN ->SCL

#ifdef  ADS115_AND_ADDRESS_GND
#define  ADS115_ADDRESS     ADS115_ADDRESS_GND    
#endif
#ifdef ADS115_AND_ADDRESS_VDD
#define  ADS115_ADDRESS     ADS115_ADDRESS_VDD
#endif
#ifdef ADS115_AND_ADDRESS_SDA
#define  ADS115_ADDRESS     ADS115_ADDRESS_SDA    
#endif
#ifdef ADS115_AND_ADDRESS_SCL
#define  ADS115_ADDRESS     ADS115_ADDRESS_SCL  
#endif

#define ADS115_ADDRESS_W  ADS115_ADDRESS|0x00 //–¥µÿ÷∑
#define ADS115_ADDRESS_R  ADS115_ADDRESS|0x01 //∂¡µÿ÷∑

typedef enum
{
	ADS1115Channel_1=0,
	ADS1115Channel_2=1,
	ADS1115Channel_3=2,
	ADS1115Channel_4=3
}ADS1115Channel_TypeDefE;


uint32_t ADS1115_ConfigAndReadAVG(ADS1115Channel_TypeDefE channel,uint8_t VAGcount);
uint32_t  ADS1115_gainAmplifierConfigurAndReadAVG(ADS1115Channel_TypeDefE channel,uint8_t VAGcount);



#endif
