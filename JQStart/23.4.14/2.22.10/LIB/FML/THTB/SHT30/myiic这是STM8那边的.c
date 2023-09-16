#include "myiic.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 								  
////////////////////////////////////////////////////////////////////////////////// 	

//IIC��ʼ��
void IIC0_Init(void)
{  
    IIC0_SDA_OUT();
    IIC0_SCL_OUT();
    IIC0_SDA(1);
    IIC0_SCL(1);  
}

//����IIC��ʼ�ź�
void IIC0_Start(void)
{
	IIC0_SDA_OUT();     //sda�����
	IIC0_SDA(1);	  	  
	IIC0_SCL(1);
	DELAY_US(4);
 	IIC0_SDA(0);//START:when CLK is high,DATA change form high to low 
	DELAY_US(4);
	IIC0_SCL(0);//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC0_Stop(void)
{
	IIC0_SDA_OUT();//sda�����
	IIC0_SCL(0);
	IIC0_SDA(0);//STOP:when CLK is high DATA change form low to high
 	DELAY_US(4);
	IIC0_SCL(1); 
	IIC0_SDA(1);//����I2C���߽����ź�
	DELAY_US(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC0_Wait_Ack(void)
{
	u8 ucErrTime=0;
	IIC0_SDA_IN();      //SDA����Ϊ����  
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
	IIC0_SCL(0);//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC0_Send_Byte(u8 txd)
{                        
    u8 t;   
    IIC0_SDA_OUT(); 	    
    IIC0_SCL(0);//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC0_SDA((txd&0x80)>>7);
        txd<<=1; 	  
		DELAY_US(2);   //��TEA5767��������ʱ���Ǳ����
		IIC0_SCL(1);
		DELAY_US(2); 
		IIC0_SCL(0);	
		DELAY_US(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC0_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	IIC0_SDA_IN();//SDA����Ϊ����
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
        IIC0_NAck();//����nACK
    else
        IIC0_Ack(); //����ACK   
    return receive;
}



