#ifndef _MYIIC_H
#define _MYIIC_H

#include "JUTIL.h"

//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////

//IO方向设置

#define IIC0_SCL_OUT()    { GPIO_InitTypeDef GPIO_InitStruct = {0};  __HAL_RCC_GPIOB_CLK_ENABLE();  __HAL_RCC_GPIOA_CLK_ENABLE();GPIO_InitStruct.Pin = scl_Pin; \
                            GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  GPIO_InitStruct.Pull = GPIO_PULLUP;\
														GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);}
#define IIC0_SDA_OUT()    {GPIO_InitTypeDef GPIO_InitStruct = {0};  __HAL_RCC_GPIOB_CLK_ENABLE();  __HAL_RCC_GPIOA_CLK_ENABLE();GPIO_InitStruct.Pin = sda_Pin; \
                            GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  GPIO_InitStruct.Pull = GPIO_PULLUP;\
														GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);}



#define IIC0_SDA_IN()     {GPIO_InitTypeDef GPIO_InitStruct = {0};  __HAL_RCC_GPIOB_CLK_ENABLE();  __HAL_RCC_GPIOA_CLK_ENABLE();\
                           GPIO_InitStruct.Pin = sda_Pin;  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);}


//IO操作
#define IIC0_SCL(n)   {n?HAL_GPIO_WritePin(GPIOB, scl_Pin, GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB, scl_Pin, GPIO_PIN_RESET);}
#define IIC0_SDA(n)  {n?HAL_GPIO_WritePin(GPIOB, sda_Pin, GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB, sda_Pin, GPIO_PIN_RESET);}

#define IIC0_READ_SDA  HAL_GPIO_ReadPin(GPIOB,sda_Pin)


#define u8 unsigned char


//IIC所有操作函数
void IIC0_Init(void);                //初始化IIC的IO口
void IIC0_Start(void);				//发送IIC开始信号
void IIC0_Stop(void);	  			//发送IIC停止信号
void IIC0_Send_Byte(uint8_t txd);			//IIC发送一个字节
uint8_t IIC0_Read_Byte(unsigned char ack);//IIC读取一个字节
uint8_t IIC0_Wait_Ack(void); 				//IIC等待ACK信号
void IIC0_Ack(void);					//IIC发送ACK信号
void IIC0_NAck(void);				//IIC不发送ACK信号


void WriteReg(uint8_t slvaddr,uint8_t addr, uint8_t data);
uint8_t ReadReg(uint8_t slvaddr,uint8_t addr);








void DELAY_US(u8 us);



#endif

