
#ifndef _BSP_SPI_TLV56xx_2_H_
#define _BSP_SPI_TLV56xx_2_H_
#include "JHAL.h"
#ifdef __CplusPlus
extern "C" {
#endif

#define TLV56xx_2_RCC_SCK 	__HAL_RCC_GPIOG_CLK_ENABLE()
#define TLV56xx_2_RCC_DATA	 	__HAL_RCC_GPIOD_CLK_ENABLE()
#define TLV56xx_2_RCC_DOUT 	__HAL_RCC_GPIOA_CLK_ENABLE()

#define TLV56xx_2_RCC_LDAC	__HAL_RCC_GPIOE_CLK_ENABLE()
#define TLV56xx_2_RCC_FS 	__HAL_RCC_GPIOE_CLK_ENABLE()
#define TLV56xx_2_RCC_FRE 	__HAL_RCC_GPIOE_CLK_ENABLE()


////#define TLV56xx_2_PIN_LDAC	PAout(3)
////#define TLV56xx_2_PIN_FS		PAout(4)
////#define TLV56xx_2_PIN_SCK		PAout(5)
////#define TLV56xx_2_PIN_DATA	PAout(6)

#define TLV56xx_2_PORT_LDAC	Tlv5610_LOAD2_GPIO_Port
#define TLV56xx_2_PIN_LDAC	Tlv5610_LOAD2_Pin

#define TLV56xx_2_PORT_FS		Tlv5610_FS2_GPIO_Port
#define TLV56xx_2_PIN_FS		Tlv5610_FS2_Pin

#define TLV56xx_2_PORT_FRE	Tlv5610_PRE2_GPIO_Port
#define TLV56xx_2_PIN_FRE		 	Tlv5610_PRE2_Pin


//SCK
#define TLV56xx_2_PORT_SCK		Tlv5610_SCK2_GPIO_Port
#define TLV56xx_2_PIN_SCK		 Tlv5610_SCK2_Pin
//MOSI
#define TLV56xx_2_PORT_DATA	Tlv5610_DIN2_GPIO_Port
#define TLV56xx_2_PIN_DATA	Tlv5610_DIN2_Pin
//MISO
#define TLV56xx_2_PORT_DOUT	Tlv5610_DO2_GPIO_Port
#define TLV56xx_2_PIN_DOUT	Tlv5610_DO2_Pin


    void TLV56xx_2_Init(void);
    void TLV56xx_2_WriteChannelValue(uint8_t channel, uint16_t value);
    void unitTest_TLV56xx_2(void);
#ifdef CplusPlus
}
#endif

#endif
