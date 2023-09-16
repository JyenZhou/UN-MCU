#ifndef __HAL_YIIC5__H__
#define	__HAL_YIIC5__H__
#include "JUTIL.h"


#define IIC5_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOB_CLK_ENABLE()
#define IIC5_GPIO_PORT                       NSA2300_SDA_GPIO_Port
#define IIC5_SCL_PIN                         NSA2300_SCL_Pin
#define IIC5_SDA_PIN                         NSA2300_SDA_Pin

#define IIC5_SCL_HIGH()                      HAL_GPIO_WritePin(IIC5_GPIO_PORT,IIC5_SCL_PIN,GPIO_PIN_SET)    // 输出高电平
#define IIC5_SCL_LOW()                       HAL_GPIO_WritePin(IIC5_GPIO_PORT,IIC5_SCL_PIN,GPIO_PIN_RESET)  // 输出低电平
#define IIC5_SDA_HIGH()                      HAL_GPIO_WritePin(IIC5_GPIO_PORT,IIC5_SDA_PIN,GPIO_PIN_SET)    // 输出高电平
#define IIC5_SDA_LOW()                       HAL_GPIO_WritePin(IIC5_GPIO_PORT,IIC5_SDA_PIN,GPIO_PIN_RESET)  // 输出低电平
#define IIC5_SDA_READ()                      HAL_GPIO_ReadPin(IIC5_GPIO_PORT,IIC5_SDA_PIN)


void IIC5_InitGPIO(void);
void    IIC5_Start(void);
void    IIC5_Stop(void);
void    IIC5_SendByte(uint8_t _ucByte);
uint8_t IIC5_ReadByte(void);
uint8_t IIC5_WaitAck(void);
void    IIC5_Ack(void);
void    IIC5_NAck(void);

HAL_StatusTypeDef IIC5_Mem_Write(I2C_HandleTypeDef *hi2c,uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef IIC5_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);
#endif





