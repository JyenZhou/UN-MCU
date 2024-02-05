#ifndef __HAL_YIIC1__H__
#define	__HAL_YIIC1__H__

#include "JUTIL.h"

#define IIC1_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOB_CLK_ENABLE()
#define IIC1_GPIO_PORT                       ADS1115_SDA2_GPIO_Port
#define IIC1_SCL_PIN                         ADS1115_SCL2_Pin//B6
#define IIC1_SDA_PIN                          ADS1115_SDA2_Pin//B6

#define IIC1_SCL_HIGH()                      HAL_GPIO_WritePin(IIC1_GPIO_PORT,IIC1_SCL_PIN,GPIO_PIN_SET)    // 输出高电平
#define IIC1_SCL_LOW()                       HAL_GPIO_WritePin(IIC1_GPIO_PORT,IIC1_SCL_PIN,GPIO_PIN_RESET)  // 输出低电平
#define IIC1_SDA_HIGH()                      HAL_GPIO_WritePin(IIC1_GPIO_PORT,IIC1_SDA_PIN,GPIO_PIN_SET)    // 输出高电平
#define IIC1_SDA_LOW()                       HAL_GPIO_WritePin(IIC1_GPIO_PORT,IIC1_SDA_PIN,GPIO_PIN_RESET)  // 输出低电平
#define IIC1_SDA_READ()                      HAL_GPIO_ReadPin(IIC1_GPIO_PORT,IIC1_SDA_PIN)


void IIC1_InitGPIO(void);
void    IIC1_Start(void);
void    IIC1_Stop(void);
void    IIC1_SendByte(uint8_t _ucByte);
uint8_t IIC1_ReadByte(void);
uint8_t IIC1_WaitAck(void);
void    IIC1_Ack(void);
void    IIC1_NAck(void);


#endif





