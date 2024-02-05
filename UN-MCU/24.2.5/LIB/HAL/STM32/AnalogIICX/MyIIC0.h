#ifndef _MYIIC0_H
#define _MYIIC0_H
#include "JUTIL.h"
//////////////////////////////////////////////////////////////////////////////////

#define MYIIC0_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOB_CLK_ENABLE()
#define MYIIC0_GPIO_PORT                       NSA2300_SDA_GPIO_Port
#define MYIIC0_SCL_PIN                         NSA2300_SCL_Pin
#define MYIIC0_SDA_PIN                  	  NSA2300_SDA_Pin
//////////////////////////////////////////////////////////////////////////////////

//IO��������

#define MYIIC0_SCL_OUT()    { GPIO_InitTypeDef GPIO_InitStruct = {0};  GPIO_InitStruct.Pin = MYIIC0_SCL_PIN; \
                            GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  GPIO_InitStruct.Pull = GPIO_PULLUP;\
														GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  HAL_GPIO_Init(MYIIC0_GPIO_PORT, &GPIO_InitStruct);}


#define MYIIC0_SDA_OUT()    {GPIO_InitTypeDef GPIO_InitStruct = {0};  GPIO_InitStruct.Pin = MYIIC0_SDA_PIN; \
                            GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  GPIO_InitStruct.Pull = GPIO_PULLUP;\
														GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  HAL_GPIO_Init(MYIIC0_GPIO_PORT, &GPIO_InitStruct);}


#define MYIIC0_SDA_IN()     {GPIO_InitTypeDef GPIO_InitStruct = {0};  \
                           GPIO_InitStruct.Pin = MYIIC0_SDA_PIN;  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;  HAL_GPIO_Init(MYIIC0_GPIO_PORT, &GPIO_InitStruct);}


//IO����
#define MYIIC0_SCL(n)   {n?HAL_GPIO_WritePin(MYIIC0_GPIO_PORT, MYIIC0_SCL_PIN, GPIO_PIN_SET):HAL_GPIO_WritePin(MYIIC0_GPIO_PORT, MYIIC0_SCL_PIN, GPIO_PIN_RESET);}
#define MYIIC0_SDA(n)  {n?HAL_GPIO_WritePin(MYIIC0_GPIO_PORT, MYIIC0_SDA_PIN, GPIO_PIN_SET):HAL_GPIO_WritePin(MYIIC0_GPIO_PORT, MYIIC0_SDA_PIN, GPIO_PIN_RESET);}


#define MYIIC0_READ_SDA  HAL_GPIO_ReadPin(MYIIC0_GPIO_PORT,MYIIC0_SDA_PIN)


#define u8 unsigned char


//IIC���в�������
void MYIIC0_Init(void);                //��ʼ��IIC��IO��
void MYIIC0_Start(void);				//����IIC��ʼ�ź�
void MYIIC0_Stop(void);	  			//����IICֹͣ�ź�
void MYIIC0_Send_Byte(uint8_t txd);			//IIC����һ���ֽ�
uint8_t MYIIC0_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
uint8_t MYIIC0_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void MYIIC0_Ack(void);					//IIC����ACK�ź�
void MYIIC0_NAck(void);				//IIC������ACK�ź�

HAL_StatusTypeDef MYIIC0_Mem_Write(I2C_HandleTypeDef *hi2c,uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef MYIIC0_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);






void DELAY_US(u8 us);



#endif

