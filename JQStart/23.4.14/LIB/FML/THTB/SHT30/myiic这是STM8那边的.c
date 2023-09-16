#include "myiic.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 								  
////////////////////////////////////////////////////////////////////////////////// 	

//IIC初始化
void IIC0_Init(void)
{  
    IIC0_SDA_OUT();
    IIC0_SCL_OUT();
    IIC0_SDA(1);
    IIC0_SCL(1);  
}

//产生IIC起始信号
void IIC0_Start(void)
{
	IIC0_SDA_OUT();     //sda线输出
	IIC0_SDA(1);	  	  
	IIC0_SCL(1);
	DELAY_US(4);
 	IIC0_SDA(0);//START:when CLK is high,DATA change form high to low 
	DELAY_US(4);
	IIC0_SCL(0);//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC0_Stop(void)
{
	IIC0_SDA_OUT();//sda线输出
	IIC0_SCL(0);
	IIC0_SDA(0);//STOP:when CLK is high DATA change form low to high
 	DELAY_US(4);
	IIC0_SCL(1); 
	IIC0_SDA(1);//发送I2C总线结束信号
	DELAY_US(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC0_Wait_Ack(void)
{
	u8 ucErrTime=0;
	IIC0_SDA_IN();      //SDA设置为输入  
	IIC0_SDA(1);DELAY_US(1);	   
	IIC0_SCL(1);DELAY_US(1);	 
	while(IIC0_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC0_Stop();
			return 1;
		}
	}
	IIC0_SCL(0);//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC0_Ack(void)
{
	IIC0_SCL(0);
	IIC0_SDA_OUT();
	IIC0_SDA(0);
	DELAY_US(2);
	IIC0_SCL(1);
	DELAY_US(2);
	IIC0_SCL(0);
}
//不产生ACK应答		    
void IIC0_NAck(void)
{
	IIC0_SCL(0);
	IIC0_SDA_OUT();
	IIC0_SDA(1);
	DELAY_US(2);
	IIC0_SCL(1);
	DELAY_US(2);
	IIC0_SCL(0);
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC0_Send_Byte(u8 txd)
{                        
    u8 t;   
    IIC0_SDA_OUT(); 	    
    IIC0_SCL(0);//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        IIC0_SDA((txd&0x80)>>7);
        txd<<=1; 	  
		DELAY_US(2);   //对TEA5767这三个延时都是必须的
		IIC0_SCL(1);
		DELAY_US(2); 
		IIC0_SCL(0);	
		DELAY_US(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC0_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	IIC0_SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC0_SCL(0); 
        DELAY_US(2);
		IIC0_SCL(1);
        receive<<=1;
        if(IIC0_READ_SDA)receive++;   
		DELAY_US(1); 
    }					 
    if (!ack)
        IIC0_NAck();//发送nACK
    else
        IIC0_Ack(); //发送ACK   
    return receive;
}



