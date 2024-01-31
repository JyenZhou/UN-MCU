/*
 * @Description: 
 * @Author: wangshi@tinychip.com.cn
 * @Date: 2023-01-13 15:18:26
 * @Disclaimer: 
 * 	* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * 	* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * 	* TIME. AS A RESULT, TINY CHIP SHALL NOT BE HELD LIABLE FOR ANY
 * 	* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * 	* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * 	* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * 	* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 * 
 * 	* Copyright (c) 2020 Tinychip Microelectronics Co.,Ltd, All Rights Reserved. 
 */

#ifndef __TC04XX_GPIO_H__
#define __TC04XX_GPIO_H__
#include "tc04xx.h"

 /* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

#define                      GPIOA_00                                     0x0000U
#define                      GPIOA_01                                     0x0001U
#define                      GPIOA_02                                     0x0002U
#define                      GPIOA_03                                     0x0003U
#define                      GPIOA_04                                     0x0004U
#define                      GPIOA_05                                     0x0005U
#define                      GPIOA_06                                     0x0006U
#define                      GPIOA_07                                     0x0007U
#define                      GPIOA_08                                     0x0008U
#define                      GPIOA_09                                     0x0009U
#define                      GPIOA_10                                     0x000AU
#define                      GPIOA_11                                     0x000BU
#define                      GPIOA_12                                     0x000CU
#define                      GPIOA_13                                     0x000DU
                                            
#define                      GPIOB_00                                     0x0100U
#define                      GPIOB_01                                     0x0101U
#define                      GPIOB_02                                     0x0102U
#define                      GPIOB_03                                     0x0103U
#define                      GPIOB_04                                     0x0104U
#define                      GPIOB_05                                     0x0105U
#define                      GPIOB_06                                     0x0106U
#define                      GPIOB_07                                     0x0107U
#define                      GPIOB_08                                     0x0108U
#define                      GPIOB_09                                     0x0109U
#define                      GPIOB_10                                     0x010AU
#define                      GPIOB_11                                     0x010BU
#define                      GPIOB_12                                     0x010CU
#define                      GPIOB_13                                     0x010DU
#define                      GPIOB_14                                     0x010EU
#define                      GPIOB_15                                     0x010FU
#define                      GPIOB_16                                     0x0110U
#define                      GPIOB_17                                     0x0111U
#define                      GPIOB_18                                     0x0112U
#define                      GPIOB_19                                     0x0113U
#define                      GPIOB_20                                     0x0114U
#define                      GPIOB_21                                     0x0115U
#define                      GPIOB_22                                     0x0116U
#define                      GPIOB_23                                     0x0117U
#define                      GPIOB_24                                     0x0118U
#define                      GPIOB_25                                     0x0119U
#define                      GPIOB_26                                     0x011AU
#define                      GPIOB_27                                     0x011BU
#define                      GPIOB_28                                     0x011CU
#define                      GPIOB_29                                     0x011DU
#define                      GPIOB_30                                     0x011EU
            
//GPIO ALTERNATE FUNCTION 
#define                      AF0                                          0x00U
#define                      AF1                                          0x01U
#define                      AF2                                          0x02U
#define                      AF3                                          0x03U
#define                      AF4                                          0x04U
#define                      AF5                                          0x05U
#define                      AF6                                          0x06U
            
//GPIO INOUT               
#define                      GPIO_INPUT_MODE                              0x00U
#define                      GPIO_OUTPUT_MODE                             0x01U
                 
                 
//GPIO EXTRAL MODE                     
#define                      GPIO_PIN_OPEN_SOURCE                         0x00U
#define                      GPIO_PIN_OPEN_DRAIN                          0x01U
#define                      GPIO_PIN_PUSH_PULL                           0x02U
#define                      GPIO_PIN_HIGH_IMPEDANCE                      0x03U
                 
                 
//GPIO PULL MODE
#define                      GPIO_PULL_NONE                               0x00U
#define                      GPIO_PULL_UP                                 0x01U
#define                      GPIO_PULL_DOWN                               0x02U
                 
//GPIO IT MODE               
#define                      GPIO_IT_FALLING_EDGE                         0x00U
#define                      GPIO_IT_RAISING_EDGE                         0x01U
#define                      GPIO_IT_LOW_LEVEL                            0x02U
#define                      GPIO_IT_HIGH_LEVEL                           0x03U



#define         PIN_MASK(io)                (1U<<((io)&0xFFU))

#define         PINMUX_PA_ADDR              PINMUX_BASE_ADDR

#define         PINMUX_PB_ADDR              (PINMUX_BASE_ADDR + 0x38)

#define         PINMUX_SEL(io)              (((io)&0xFF00U)?PINMUX_PB_ADDR:PINMUX_PA_ADDR)

#define         PINMUX_ADDR(io)             (PINMUX_SEL(io)+((io)&0xFFU)*4)


#define         GPIO_SET_AFMODE(io,mode)     (((APINMUX_CFG_FIELD_T *)PINMUX_ADDR(io))->SW_SRC_SEL = (mode))

#define         GPIO_CLR_AFMODE(io)          (((APINMUX_CFG_FIELD_T *)PINMUX_ADDR(io))->SW_SRC_SEL = 0)

// #define         GPIO_DIGITAL_ENABLE(io)      (((APINMUX_CFG_FIELD_T *)PINMUX_ADDR(io))->SW_IE = ENABLE)

// #define         GPIO_DIGITAL_DISABLE(io)     (((APINMUX_CFG_FIELD_T *)PINMUX_ADDR(io))->SW_IE = DISABLE)



typedef struct
{
    uint16_t  gpio;     /*!< Specifies the GPIO Port to be configured.
                                      This parameter can be any value of @ref GPIO_TYPE */
	

    uint8_t  mode;     /*!< Specifies the operating mode for the selected pins.
                                      This parameter can be a value of @ref GPIOMode_TypeDef */

    uint8_t  ext_mode; /*!< Specifies the operating mode for the selected pins.
                                      This parameter can be a value of @ref GPIO_EXT_MODE_E */

    uint8_t  pull_mode;/*!< Specifies the operating mode for the selected pins.
                                            This parameter can be a value of @ref GPIO_PUSH_PULL_MODE_E */
} GPIO_INIT_T;


void GPIO_Config(uint16_t gpio, uint8_t mode);
void GPIO_Init(GPIO_INIT_T * pInit_t);
void GPIO_DeInit(uint16_t gpio);
void GPIO_ITConfig(uint16_t gpio, uint8_t mode);
void GPIO_IT_Enable(uint16_t gpio);
void GPIO_IT_Disable(uint16_t gpio);
void GPIO_SetBit(uint16_t gpio);
void GPIO_ClrBit(uint16_t gpio);
uint8_t GPIO_GetBit(uint16_t gpio);
void GPIO_ToggleBit(uint16_t gpio);
uint8_t GPIO_GetFlag(uint16_t gpio);
void GPIO_ClrFlag(uint16_t gpio);
void GPIO_ExtModeConfig(uint16_t gpio, uint8_t ext_mode, uint8_t pull_mode);
#ifdef __cplusplus
}
#endif
#endif /* __GPIO_H__ */

