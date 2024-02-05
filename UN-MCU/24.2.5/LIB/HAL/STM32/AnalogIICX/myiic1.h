#ifndef _MYIIC_H
#define _MYIIC_H

#include "JUTIL.h"

//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////

//IO��������

#define IIC0_SCL_OUT()    { GPIO_InitTypeDef GPIO_InitStruct = {0};  __HAL_RCC_GPIOB_CLK_ENABLE();  __HAL_RCC_GPIOA_CLK_ENABLE();GPIO_InitStruct.Pin = scl_Pin; \
                            GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  GPIO_InitStruct.Pull = GPIO_PULLUP;\
														GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);}
#define IIC0_SDA_OUT()    {GPIO_InitTypeDef GPIO_InitStruct = {0};  __HAL_RCC_GPIOB_CLK_ENABLE();  __HAL_RCC_GPIOA_CLK_ENABLE();GPIO_InitStruct.Pin = sda_Pin; \
                            GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  GPIO_InitStruct.Pull = GPIO_PULLUP;\
														GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);}



#define IIC0_SDA_IN()     {GPIO_InitTypeDef GPIO_InitStruct = {0};  __HAL_RCC_GPIOB_CLK_ENABLE();  __HAL_RCC_GPIOA_CLK_ENABLE();\
                           GPIO_InitStruct.Pin = sda_Pin;  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);}


//IO����
#define IIC0_SCL(n)   {n?HAL_GPIO_WritePin(GPIOB, scl_Pin, GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB, scl_Pin, GPIO_PIN_RESET);}
#define IIC0_SDA(n)  {n?HAL_GPIO_WritePin(GPIOB, sda_Pin, GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB, sda_Pin, GPIO_PIN_RESET);}

#define IIC0_READ_SDA  HAL_GPIO_ReadPin(GPIOB,sda_Pin)


#define u8 unsigned char


//IIC���в�������
void IIC0_Init(void);                //��ʼ��IIC��IO��
void IIC0_Start(void);				//����IIC��ʼ�ź�
void IIC0_Stop(void);	  			//����IICֹͣ�ź�
void IIC0_Send_Byte(uint8_t txd);			//IIC����һ���ֽ�
uint8_t IIC0_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
uint8_t IIC0_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC0_Ack(void);					//IIC����ACK�ź�
void IIC0_NAck(void);				//IIC������ACK�ź�


void WriteReg(uint8_t slvaddr,uint8_t addr, uint8_t data);
uint8_t ReadReg(uint8_t slvaddr,uint8_t addr);








void DELAY_US(u8 us);



#endif

