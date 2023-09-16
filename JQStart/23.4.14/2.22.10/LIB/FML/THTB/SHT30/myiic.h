#ifndef _MYIIC_H
#define _MYIIC_H
#include "stm8s.h"
//////////////////////////////////////////////////////////////////////////////////	 
								  

#define IIC0_SCL_PORT  GPIOE
#define IIC0_SCL_PIN GPIO_PIN_1
#define IIC0_SDA_PORT  GPIOE
#define IIC0_SDA_PIN GPIO_PIN_2
////////////////////////////////////////////////////////////////////////////////// 	

//IO方向设置

//PD5--TX(SCL)  PD6--RX
//TX-SCL
#define IIC0_SCL_OUT()    {GPIO_Init(IIC0_SCL_PORT, IIC0_SCL_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);}
#define IIC0_SDA_OUT()    {GPIO_Init(IIC0_SDA_PORT, IIC0_SDA_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);}
#define IIC0_SDA_IN()     {GPIO_Init(IIC0_SDA_PORT, IIC0_SDA_PIN,GPIO_MODE_IN_PU_NO_IT);}



//IO操作
#define IIC0_SCL(n)   {n?GPIO_WriteHigh(IIC0_SCL_PORT, IIC0_SCL_PIN):GPIO_WriteLow(IIC0_SCL_PORT, IIC0_SCL_PIN);}//SCL
#define IIC0_SDA(n)   {n?GPIO_WriteHigh(IIC0_SDA_PORT, IIC0_SDA_PIN):GPIO_WriteLow(IIC0_SDA_PORT, IIC0_SDA_PIN);} //SDA
#define IIC0_READ_SDA  GPIO_ReadInputPin(IIC0_SDA_PORT, IIC0_SDA_PIN) //输入SDA



//IIC所有操作函数
void IIC0_Init(void);                //初始化IIC的IO口				 
void IIC0_Start(void);				//发送IIC开始信号
void IIC0_Stop(void);	  			//发送IIC停止信号
void IIC0_Send_Byte(u8 txd);			//IIC发送一个字节
u8   IIC0_Read_Byte(unsigned char ack);//IIC读取一个字节
u8   IIC0_Wait_Ack(void); 				//IIC等待ACK信号
void IIC0_Ack(void);					//IIC发送ACK信号
void IIC0_NAck(void);				//IIC不发送ACK信号
	 


	



#endif

