#ifndef _MYIIC3_H
#define _MYIIC3_H
#include "JHAL.h"
//////////////////////////////////////////////////////////////////////////////////

#define MYIIC3_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOB_CLK_ENABLE()
#define MYIIC3_GPIO_PORT                      ADS_SDA_GPIO_Port
#define MYIIC3_SCL_PIN                         ADS_SCL_Pin
#define MYIIC3_SDA_PIN                  	  ADS_SDA_Pin
//////////////////////////////////////////////////////////////////////////////////

//IO��������

#define MYIIC3_SCL_OUT()    { GPIO_InitTypeDef GPIO_InitStruct = {0};  GPIO_InitStruct.Pin = MYIIC3_SCL_PIN; \
                            GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  GPIO_InitStruct.Pull = GPIO_PULLUP;\
														GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  HAL_GPIO_Init(MYIIC3_GPIO_PORT, &GPIO_InitStruct);}


#define MYIIC3_SDA_OUT()    {GPIO_InitTypeDef GPIO_InitStruct = {0};  GPIO_InitStruct.Pin = MYIIC3_SDA_PIN; \
                            GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  GPIO_InitStruct.Pull = GPIO_PULLUP;\
														GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  HAL_GPIO_Init(MYIIC3_GPIO_PORT, &GPIO_InitStruct);}


#define MYIIC3_SDA_IN()     {GPIO_InitTypeDef GPIO_InitStruct = {0};  \
                           GPIO_InitStruct.Pin = MYIIC3_SDA_PIN;  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;  HAL_GPIO_Init(MYIIC3_GPIO_PORT, &GPIO_InitStruct);}


//IO����
#define MYIIC3_SCL(n)   {n?HAL_GPIO_WritePin(MYIIC3_GPIO_PORT, MYIIC3_SCL_PIN, GPIO_PIN_SET):HAL_GPIO_WritePin(MYIIC3_GPIO_PORT, MYIIC3_SCL_PIN, GPIO_PIN_RESET);}
#define MYIIC3_SDA(n)  {n?HAL_GPIO_WritePin(MYIIC3_GPIO_PORT, MYIIC3_SDA_PIN, GPIO_PIN_SET):HAL_GPIO_WritePin(MYIIC3_GPIO_PORT, MYIIC3_SDA_PIN, GPIO_PIN_RESET);}


#define MYIIC3_READ_SDA  HAL_GPIO_ReadPin(MYIIC3_GPIO_PORT,MYIIC3_SDA_PIN)


#define u8 unsigned char


//IIC���в�������
void MYIIC3_Init(void);                //��ʼ��IIC��IO��
void MYIIC3_Start(void);				//����IIC��ʼ�ź�
void MYIIC3_Stop(void);	  			//����IICֹͣ�ź�
void MYIIC3_Send_Byte(uint8_t txd);			//IIC����һ���ֽ�
uint8_t MYIIC3_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
uint8_t MYIIC3_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void MYIIC3_Ack(void);					//IIC����ACK�ź�
void MYIIC3_NAck(void);				//IIC������ACK�ź�

HAL_StatusTypeDef MYIIC3_Mem_Write(I2C_HandleTypeDef *hi2c,uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef MYIIC3_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);









#endif

