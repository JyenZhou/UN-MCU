#include "sht20.h" 

#define SHT20ADDR 0x80

const int16_t POLYNOMIAL = 0x131;

IIC_INFO 	 iic_info;

SHT20_INFO sht20_info;

void IIC_SpeedCtl(unsigned short speed)
{

	iic_info.speed = speed;

}

void IIC_Start(void)
{
	
	SDA_H;						//拉高SDA线
	SCL_H;						//拉高SCL线
	Delay_Us(iic_info.speed);	//延时，速度控制
	
	SDA_L;						//当SCL线为高时，SDA线一个下降沿代表开始信号
	Delay_Us(iic_info.speed);	//延时，速度控制
	SCL_L;						//钳住SCL线，以便发送数据

}

void IIC_Stop(void)
{

	SDA_L;						//拉低SDA线
	SCL_L;						//拉低SCL先
	Delay_Us(iic_info.speed);	//延时，速度控制
	
	SCL_H;						//拉高SCL线
	SDA_H;						//拉高SDA线，当SCL线为高时，SDA线一个上升沿代表停止信号
	Delay_Us(iic_info.speed);

}

_Bool IIC_WaitAck(unsigned int timeOut)
{
	
	
	SDA_H;Delay_Us(iic_info.speed);			//拉高SDA线
	SCL_H;Delay_Us(iic_info.speed);			//拉高SCL线
	
	while(SDA_read)							//如果读到SDA线为1，则等待。应答信号应是0
	{
		if(--timeOut == 0)
		{
//			UsartPrintf(USART1, "WaitAck TimeOut\r\n");

			IIC_Stop();						//超时未收到应答，则停止总线
			
			return IIC_Err;					//返回失败
		}
		
		Delay_Us(iic_info.speed);
	}
	
	SCL_L;									//拉低SCL线，以便继续收发数据
	
	return IIC_OK;							//返回成功
	
}

void IIC_Ack(void)
{
	
	SCL_L;						//拉低SCL线
	SDA_L;						//拉低SDA线
	Delay_Us(iic_info.speed);
	SCL_H;						//拉高SCL线
	Delay_Us(iic_info.speed);
	SCL_L;						//拉低SCL线
	
}

void IIC_NAck(void)
{
	
	SCL_L;						//拉低SCL线
	SDA_H;						//拉高SDA线
	Delay_Us(iic_info.speed);
	SCL_H;						//拉高SCL线
	Delay_Us(iic_info.speed);
	SCL_L;						//拉低SCL线
	
}

void IIC_SendByte(unsigned char byte)
{

	unsigned char count = 0;
	
    SCL_L;							//拉低时钟开始数据传输
	
    for(; count < 8; count++)		//循环8次，每次发送一个bit
    {
		if(byte & 0x80)				//发送最高位
			SDA_H;
		else
			SDA_L;
		
		byte <<= 1;					//byte左移1位
		
		Delay_Us(iic_info.speed);
		SCL_H;
		Delay_Us(iic_info.speed);
		SCL_L;
    }

}

unsigned char IIC_RecvByte(void)
{
	
	unsigned char count = 0, receive = 0;
	
	SDA_H;							//拉高SDA线，开漏状态下，需线拉高以便读取数据
	
    for(; count < 8; count++ )		//循环8次，每次发送一个bit
	{
		SCL_L;
		Delay_Us(iic_info.speed);
		SCL_H;
		
        receive <<= 1;				//左移一位
		
        if(SDA_read)					//如果SDA线为1，则receive变量自增，每次自增都是对bit0的+1，然后下一次循环会先左移一次
			receive++;
		
		Delay_Us(iic_info.speed);
    }
	
    return receive;
	
}

_Bool I2C_WriteByte(unsigned char slaveAddr, unsigned char regAddr, unsigned char *byte)
{

	unsigned char addr = 0;

	addr = slaveAddr << 1;		//IIC地址是7bit，这里需要左移1位，bit0：1-读	0-写
	
	IIC_Start();				//起始信号
	
	IIC_SendByte(addr);			//发送设备地址(写)
	if(IIC_WaitAck(5000))		//等待应答
		return IIC_Err;
	
	IIC_SendByte(regAddr);		//发送寄存器地址
	if(IIC_WaitAck(5000))		//等待应答
		return IIC_Err;
	
	if(byte)
	{
		IIC_SendByte(*byte);	//发送数据
		if(IIC_WaitAck(5000))	//等待应答
			return IIC_Err;
	}
	
	IIC_Stop();					//停止信号
	
	return IIC_OK;

}

_Bool I2C_ReadByte(unsigned char slaveAddr, unsigned char regAddr, unsigned char *val)
{

	unsigned char addr = 0;

    addr = slaveAddr << 1;		//IIC地址是7bit，这里需要左移1位，bit0：1-读	0-写
	
	IIC_Start();				//起始信号
	
	IIC_SendByte(addr);			//发送设备地址(写)
	if(IIC_WaitAck(5000))		//等待应答
		return IIC_Err;
	
	IIC_SendByte(regAddr);		//发送寄存器地址
	if(IIC_WaitAck(5000))		//等待应答
		return IIC_Err;
	
	IIC_Start();				//重启信号
	
	IIC_SendByte(addr + 1);		//发送设备地址(读)
	if(IIC_WaitAck(5000))		//等待应答
		return IIC_Err;
	
	*val = IIC_RecvByte();		//接收
	IIC_NAck();					//产生一个非应答信号，代表读取接收
	
	IIC_Stop();					//停止信号
	
	return IIC_OK;

}

_Bool I2C_WriteBytes(unsigned char slaveAddr, unsigned char regAddr, unsigned char *buf, unsigned char num)
{

	unsigned char addr = 0;

	addr = slaveAddr << 1;		//IIC地址是7bit，这里需要左移1位，bit0：1-读	0-写
	
	IIC_Start();				//起始信号
	
	IIC_SendByte(addr);			//发送设备地址(写)
	if(IIC_WaitAck(5000))		//等待应答
		return IIC_Err;
	
	IIC_SendByte(regAddr);		//发送寄存器地址
	if(IIC_WaitAck(5000))		//等待应答
		return IIC_Err;
	
	while(num--)				//循环写入数据
	{
		IIC_SendByte(*buf);		//发送数据
		if(IIC_WaitAck(5000))	//等待应答
			return IIC_Err;
		
		buf++;					//数据指针偏移到下一个
		
		Delay_Us(10);
	}
	
	IIC_Stop();					//停止信号
	
	return IIC_OK;

}

_Bool I2C_ReadBytes(unsigned char slaveAddr, unsigned char regAddr, unsigned char *buf, unsigned char num)
{

	unsigned short addr = 0;

    addr = slaveAddr << 1;		//IIC地址是7bit，这里需要左移1位，bit0：1-读	0-写
	
	IIC_Start();						//起始信号
	
	IIC_SendByte(addr);			//发送设备地址(写)
	if(IIC_WaitAck(5000))		//等待应答
		return IIC_Err;
	
	IIC_SendByte(regAddr);		//发送寄存器地址
	if(IIC_WaitAck(5000))		//等待应答
		return IIC_Err;
	
	IIC_Start();				//重启信号
	
	IIC_SendByte(addr + 1);		//发送设备地址(读)
	if(IIC_WaitAck(5000))		//等待应答
		return IIC_Err;
	
	while(num--)
	{
		*buf = IIC_RecvByte();
		buf++;					//偏移到下一个数据存储地址
		
		if(num == 0)
        {
           IIC_NAck();			//最后一个数据需要回NOACK
        }
        else
        {
          IIC_Ack();			//回应ACK
		}
	}
	
	IIC_Stop();
	
	return IIC_OK;

}

void SHT20_reset(void)
{
    I2C_WriteByte(SHT20_ADDRESS, SHT20_SOFT_RESET, (void *)0);    
}

unsigned char  SHT20_read_user_reg(void)
{    
    unsigned char val = 0;
    I2C_ReadByte(SHT20_ADDRESS, SHT20_READ_REG, &val);
    return val;    
}

char SHT2x_CheckCrc(char data[], char nbrOfBytes, char checksum)
{
    char crc = 0;
    char bit = 0;
    char byteCtr = 0;

    for(byteCtr = 0; byteCtr < nbrOfBytes; ++byteCtr)
    {
        crc ^= (data[byteCtr]);
        for ( bit = 8; bit > 0; --bit)
        {
            if (crc & 0x80) crc = (crc << 1) ^ POLYNOMIAL;
            else crc = (crc << 1);
        }
    }    
    if(crc != checksum)
        return 1;
    else
        return 0;
}

float SHT2x_CalcTemperatureC(unsigned short u16sT)
{
    float temperatureC = 0;            // variable for result
    u16sT &= ~0x0003;           // clear bits [1..0] (status bits)
    temperatureC = -46.85 + 175.72 / 65536 * (float)u16sT; //T= -46.85 + 175.72 * ST/2^16
    return temperatureC;

}

float SHT2x_CalcRH(unsigned short u16sRH)
{
    float humidityRH = 0; 
    u16sRH &= ~0x0003;          // clear bits [1..0] (status bits)
    //humidityRH = -6.0 + 125.0/65536 * (float)u16sRH; // RH= -6 + 125 * SRH/2^16
    humidityRH = ((double)u16sRH * 0.00190735) - 6;
    return humidityRH;
}

float SHT2x_MeasureHM(unsigned char cmd, unsigned short *pMeasurand)
{
    char  checksum = 0;  //checksum
    char  data[2];    //data array for checksum verification
    unsigned char addr = 0;
    unsigned short tmp = 0;
    float t = 0;
    addr = SHT20_ADDRESS << 1;
    IIC_Start();
    IIC_SendByte(addr);
    if(IIC_WaitAck(50000)) //等待应答
        return 0.0;
    IIC_SendByte(cmd);
    if(IIC_WaitAck(50000)) //等待应答
        return 0.0;
    IIC_Start();
    IIC_SendByte(addr + 1);
    while(IIC_WaitAck(50000)) //等待应答
    {
        IIC_Start();
        IIC_SendByte(addr + 1);
    }
    Delay_Ms(70);
    data[0] = IIC_RecvByte();
    IIC_Ack();
    data[1] = IIC_RecvByte();
    IIC_Ack();
    checksum = IIC_RecvByte();
    IIC_NAck();

    IIC_Stop();
    SHT2x_CheckCrc(data, 2, checksum);
    tmp = (data[0] << 8) + data[1];
    if(cmd == SHT20_Measurement_T_HM)
    {
        t = SHT2x_CalcTemperatureC(tmp);
    }
    else
    {
        t = SHT2x_CalcRH(tmp);
    }
    if(pMeasurand)
    {
        *pMeasurand = (unsigned short)t;
    }
    return t;
}

void SHT20_GetValue(void)
{
    unsigned char val = 0;
    IIC_SpeedCtl(1);
    SHT20_read_user_reg();
    Delay_Us(100);
    sht20_info.tempreture = SHT2x_MeasureHM(SHT20_Measurement_T_HM, (void *)0);
    Delay_Ms(1);
    sht20_info.humidity = SHT2x_MeasureHM(SHT20_Measurement_RH_HM, (void *)0);
    Delay_Ms(1);
    SHT20_read_user_reg();
    Delay_Ms(1);
    I2C_WriteByte(SHT20_ADDRESS, SHT20_WRITE_REG, &val);
    Delay_Us(100);
    SHT20_read_user_reg();
    Delay_Us(100);
    SHT20_reset();
    Delay_Us(100);
}
