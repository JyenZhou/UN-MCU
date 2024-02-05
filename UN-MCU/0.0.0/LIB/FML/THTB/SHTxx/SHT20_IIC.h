#ifndef __HAL_YSHT20_IIC__H__
#define	__HAL_YSHT20_IIC__H__

#include "JHAL.h"


#define SHT20_IIC_GPIO_SCL_CLK_ENABLE()               __HAL_RCC_GPIOA_CLK_ENABLE()
#define SHT20_IIC_GPIO_SDA_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()
#define SHT20_IIC_SCL_GPIO_PORT                       SHT20_SCL1_GPIO_Port
#define SHT20_IIC_SDA_GPIO_PORT                   SHT20_SCL1_GPIO_Port
#define SHT20_IIC_SCL_PIN                         SHT20_SCL1_Pin
#define SHT20_IIC_SDA_PIN                         SHT20_SDA1_Pin





#define SHT20_IIC_SCL_HIGH()                      HAL_GPIO_WritePin(SHT20_IIC_SCL_GPIO_PORT,SHT20_IIC_SCL_PIN,GPIO_PIN_SET)    // 输出高电平
#define SHT20_IIC_SCL_LOW()                       HAL_GPIO_WritePin(SHT20_IIC_SCL_GPIO_PORT,SHT20_IIC_SCL_PIN,GPIO_PIN_RESET)  // 输出低电平
#define SHT20_IIC_SDA_HIGH()                      HAL_GPIO_WritePin(SHT20_IIC_SCL_GPIO_PORT,SHT20_IIC_SDA_PIN,GPIO_PIN_SET)    // 输出高电平
#define SHT20_IIC_SDA_LOW()                       HAL_GPIO_WritePin(SHT20_IIC_SCL_GPIO_PORT,SHT20_IIC_SDA_PIN,GPIO_PIN_RESET)  // 输出低电平
#define SHT20_IIC_SDA_READ()                      HAL_GPIO_ReadPin(SHT20_IIC_SCL_GPIO_PORT,SHT20_IIC_SDA_PIN)




void SHT20_IIC_Init(void);
void    SHT20_IIC_Start(void);
void    SHT20_IIC_Stop(void);
void    SHT20_IIC_SendByte(uint8_t _ucByte);
uint8_t SHT20_IIC_ReadByte(void);
uint8_t SHT20_IIC_WaitAck(void);
void    SHT20_IIC_Ack(void);
void    SHT20_IIC_NAck(void);
#else


#endif





