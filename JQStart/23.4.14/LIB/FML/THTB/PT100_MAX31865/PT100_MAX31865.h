
#ifndef _BSP_PT100_SPI_MAX31865_H_
#define _BSP_PT100_SPI_MAX31865_H_
#include "JHAL.h"
#ifdef __CplusPlus
extern "C" {
#endif

/* 定义硬件SPI(需要重写硬件的 这个无效) */
//#define MAX31865_HARD_SPI


#define MAX31865_PORT_NCS_Default  PT100_nCS1_GPIO_Port
#define MAX31865_PIN_NCS_Default PT100_nCS1_Pin

#define MAX31865_RCC_NCS 	 __HAL_RCC_GPIOA_CLK_ENABLE()
 





#ifndef MAX31865_HARD_SPI		/* 定义硬件SPI */
#define MAX31865_RCC_SDI 		  __HAL_RCC_GPIOA_CLK_ENABLE()
#define MAX31865_RCC_SDO 	__HAL_RCC_GPIOG_CLK_ENABLE()
#define MAX31865_RCC_SCLK 	__HAL_RCC_GPIOD_CLK_ENABLE()
#define MAX31865_PORT_SDI  GPIOA
#define MAX31865_PIN_SDI GPIO_PIN_7
#define MAX31865_PORT_SCLK  GPIOA
#define MAX31865_PIN_SCLK  GPIO_PIN_5
#define MAX31865_PORT_SDO  GPIOA
#define MAX31865_PIN_SDO GPIO_PIN_6
#else
extern SPI_HandleTypeDef hspi3;
#define MAX31865_SPI hspi3
#endif

#define MAX31865_RCC_DRDY 	 __HAL_RCC_GPIOG_CLK_ENABLE()
#define MAX31865_PORT_DRDY  PT100_DRDY_GPIO_Port
#define MAX31865_PIN_DRDY PT100_DRDY_Pin






void MAX31865_NCS_GPIO_Set(GPIO_TypeDef* GPIOX,uint16_t GPIO_PinX);
void unitTest_PT100_max3185(void);
float MAX31865_Get_PT100_tempture(void);
float MAX31865_Get_PT1000_tempture(void);
void MAX31865_SB_Write(uint8_t addr,uint8_t wdata);
//正确时返回00
uint8 MAX31865_Get_Fault_Status(void);
void PT100_max3185_init(void);

#ifdef CplusPlus
}
#endif

#endif
