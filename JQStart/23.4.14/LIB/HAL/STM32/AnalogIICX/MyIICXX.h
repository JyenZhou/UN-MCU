#ifndef _MYIIXX_H
#define _MYIIXX_H
#include "JUTIL.h"
//////////////////////////////////////////////////////////////////////////////////

#define MYIIC0_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOB_CLK_ENABLE()
#define MYIIC0_GPIO_PORT                       NSA2300_SDA_GPIO_Port
#define MYIIC0_SCL_PIN                         NSA2300_SCL_Pin
#define MYIIC0_SDA_PIN                  	  NSA2300_SDA_Pin
//////////////////////////////////////////////////////////////////////////////////

//IO方向设置

#define MYIIC0_SCL_OUT()    { GPIO_InitTypeDef GPIO_InitStruct = {0};  GPIO_InitStruct.Pin = MYIIC0_SCL_PIN; \
                            GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  GPIO_InitStruct.Pull = GPIO_PULLUP;\
														GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  HAL_GPIO_Init(MYIIC0_GPIO_PORT, &GPIO_InitStruct);}


#define MYIIC0_SDA_OUT()    {GPIO_InitTypeDef GPIO_InitStruct = {0};  GPIO_InitStruct.Pin = MYIIC0_SDA_PIN; \
                            GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  GPIO_InitStruct.Pull = GPIO_PULLUP;\
														GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  HAL_GPIO_Init(MYIIC0_GPIO_PORT, &GPIO_InitStruct);}


#define MYIIC0_SDA_IN()     {GPIO_InitTypeDef GPIO_InitStruct = {0};  \
                           GPIO_InitStruct.Pin = MYIIC0_SDA_PIN;  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;  HAL_GPIO_Init(MYIIC0_GPIO_PORT, &GPIO_InitStruct);}


//IO操作
#define MYIIC0_SCL(n)   {n?HAL_GPIO_WritePin(MYIIC0_GPIO_PORT, MYIIC0_SCL_PIN, GPIO_PIN_SET):HAL_GPIO_WritePin(MYIIC0_GPIO_PORT, MYIIC0_SCL_PIN, GPIO_PIN_RESET);}
#define MYIIC0_SDA(n)  {n?HAL_GPIO_WritePin(MYIIC0_GPIO_PORT, MYIIC0_SDA_PIN, GPIO_PIN_SET):HAL_GPIO_WritePin(MYIIC0_GPIO_PORT, MYIIC0_SDA_PIN, GPIO_PIN_RESET);}


#define MYIIC0_READ_SDA  HAL_GPIO_ReadPin(MYIIC0_GPIO_PORT,MYIIC0_SDA_PIN)


#define u8 unsigned char


//IIC所有操作函数
void MYIIC0_Init(void);                //初始化IIC的IO口
void MYIIC0_Start(void);				//发送IIC开始信号
void MYIIC0_Stop(void);	  			//发送IIC停止信号
void MYIIC0_Send_Byte(uint8_t txd);			//IIC发送一个字节
uint8_t MYIIC0_Read_Byte(unsigned char ack);//IIC读取一个字节
uint8_t MYIIC0_Wait_Ack(void); 				//IIC等待ACK信号
void MYIIC0_Ack(void);					//IIC发送ACK信号
void MYIIC0_NAck(void);				//IIC不发送ACK信号

HAL_StatusTypeDef MYIIC0_Mem_Write(I2C_HandleTypeDef *hi2c,uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef MYIIC0_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);






void DELAY_US(u8 us);



#endif

