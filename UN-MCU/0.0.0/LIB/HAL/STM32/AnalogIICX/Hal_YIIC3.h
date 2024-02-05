#ifndef __HAL_YIIC3__H__
#define	__HAL_YIIC3__H__
#include "JUTIL.h"



#define IIC3_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOX_CLK_ENABLE()
#define IIC3_GPIO_PORT                       GPIOX
#define IIC3_SCL_PIN                         GPIO_PIN_X
#define IIC3_SDA_PIN                         GPIO_PIN_X

#define IIC3_SCL_HIGH()                      HAL_GPIO_WritePin(IIC3_GPIO_PORT,IIC3_SCL_PIN,GPIO_PIN_SET)    // 输出高电平
#define IIC3_SCL_LOW()                       HAL_GPIO_WritePin(IIC3_GPIO_PORT,IIC3_SCL_PIN,GPIO_PIN_RESET)  // 输出低电平
#define IIC3_SDA_HIGH()                      HAL_GPIO_WritePin(IIC3_GPIO_PORT,IIC3_SDA_PIN,GPIO_PIN_SET)    // 输出高电平
#define IIC3_SDA_LOW()                       HAL_GPIO_WritePin(IIC3_GPIO_PORT,IIC3_SDA_PIN,GPIO_PIN_RESET)  // 输出低电平
#define IIC3_SDA_READ()                      HAL_GPIO_ReadPin(IIC3_GPIO_PORT,IIC3_SDA_PIN)


void IIC3_InitGPIO(void);
void    IIC3_Start(void);
void    IIC3_Stop(void);
void    IIC3_SendByte(uint8_t _ucByte);
uint8_t IIC3_ReadByte(void);
uint8_t IIC3_WaitAck(void);
void    IIC3_Ack(void);
void    IIC3_NAck(void);


#endif





