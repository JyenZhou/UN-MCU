#ifndef __HAL_YIIC4__H__
#define	__HAL_YIIC4__H__
#include "JUTIL.h"


#define IIC4_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOX_CLK_ENABLE()
#define IIC4_GPIO_PORT                       GPIOX
#define IIC4_SCL_PIN                         GPIO_PIN_X
#define IIC4_SDA_PIN                         GPIO_PIN_X

#define IIC4_SCL_HIGH()                      HAL_GPIO_WritePin(IIC4_GPIO_PORT,IIC4_SCL_PIN,GPIO_PIN_SET)    // 输出高电平
#define IIC4_SCL_LOW()                       HAL_GPIO_WritePin(IIC4_GPIO_PORT,IIC4_SCL_PIN,GPIO_PIN_RESET)  // 输出低电平
#define IIC4_SDA_HIGH()                      HAL_GPIO_WritePin(IIC4_GPIO_PORT,IIC4_SDA_PIN,GPIO_PIN_SET)    // 输出高电平
#define IIC4_SDA_LOW()                       HAL_GPIO_WritePin(IIC4_GPIO_PORT,IIC4_SDA_PIN,GPIO_PIN_RESET)  // 输出低电平
#define IIC4_SDA_READ()                      HAL_GPIO_ReadPin(IIC4_GPIO_PORT,IIC4_SDA_PIN)


void IIC4_InitGPIO(void);
void    IIC4_Start(void);
void    IIC4_Stop(void);
void    IIC4_SendByte(uint8_t _ucByte);
uint8_t IIC4_ReadByte(void);
uint8_t IIC4_WaitAck(void);
void    IIC4_Ack(void);
void    IIC4_NAck(void);


#endif





