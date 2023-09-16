
#ifndef _BSP_SPI_TLV56xx_H_
#define _BSP_SPI_TLV56xx_H_
#include "JHAL.h"
#ifdef __CplusPlus
extern "C" {
#endif

#define TLV56xx_RCC_SCK 	__HAL_RCC_GPIOG_CLK_ENABLE()
#define TLV56xx_RCC_DATA	 	__HAL_RCC_GPIOD_CLK_ENABLE()
#define TLV56xx_RCC_DOUT 	__HAL_RCC_GPIOA_CLK_ENABLE()

#define TLV56xx_RCC_LDAC	__HAL_RCC_GPIOE_CLK_ENABLE()
#define TLV56xx_RCC_FS 	__HAL_RCC_GPIOE_CLK_ENABLE()
#define TLV56xx_RCC_FRE 	__HAL_RCC_GPIOE_CLK_ENABLE()


////#define TLV56xx_PIN_LDAC	PAout(3)
////#define TLV56xx_PIN_FS		PAout(4)
////#define TLV56xx_PIN_SCK		PAout(5)
////#define TLV56xx_PIN_DATA	PAout(6)

#define TLV56xx_PORT_LDAC	Tlv5610_LOAD1_GPIO_Port
#define TLV56xx_PIN_LDAC	Tlv5610_LOAD1_Pin

#define TLV56xx_PORT_FS		Tlv5610_FS1_GPIO_Port
#define TLV56xx_PIN_FS		Tlv5610_FS1_Pin

#define TLV56xx_PORT_FRE	Tlv5610_PRE1_GPIO_Port
#define TLV56xx_PIN_FRE		 	Tlv5610_PRE1_Pin


//SCK
#define TLV56xx_PORT_SCK		GPIOA
#define TLV56xx_PIN_SCK		 GPIO_PIN_15
//MOSI
#define TLV56xx_PORT_DATA	Tlv5610_DIN1_GPIO_Port
#define TLV56xx_PIN_DATA	Tlv5610_DIN1_Pin
//MISO
#define TLV56xx_PORT_DOUT	Tlv5610_DO1_GPIO_Port
#define TLV56xx_PIN_DOUT	Tlv5610_DO1_Pin


    void TLV56xx_Init(void);
    void TLV56xx_WriteChannelValue(uint8_t channel, uint16_t value);
    void unitTest_TLV56xx(void);
#ifdef CplusPlus
}
#endif

#endif
