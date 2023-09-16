#include "MyIIC2.h"

//////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////

//US DELAY


void DELAY_US(unsigned char us)
{
    unsigned char i;
    for(i=100; i>0; i--);
}


//IIC初始化
void MYIIC2_Init(void)
{
#ifndef HAL_MYYIIC0_MODULE_ENABLED
    未初始化模拟IIC0
#endif
#ifndef 		HARDWARE_MYIIC2
    MYIIC2_GPIO_CLK_ENABLE();
    MYIIC2_SDA_OUT();
    MYIIC2_SCL_OUT();
    MYIIC2_SDA(1);
    MYIIC2_SCL(1);

#endif

}

//产生IIC起始信号
void MYIIC2_Start(void)
{
    MYIIC2_SDA_OUT();     //sda线输出
    MYIIC2_SDA(1);
    MYIIC2_SCL(1);
    DELAY_US(4);
    MYIIC2_SDA(0);//START:when CLK is high,DATA change form high to low
    DELAY_US(4);
    MYIIC2_SCL(0);//钳住I2C总线，准备发送或接收数据
}
//产生IIC停止信号
void MYIIC2_Stop(void)
{
    MYIIC2_SDA_OUT();//sda线输出
    MYIIC2_SCL(0);
    MYIIC2_SDA(0);//STOP:when CLK is high DATA change form low to high
    DELAY_US(4);
    MYIIC2_SCL(1);
    MYIIC2_SDA(1);//发送I2C总线结束信号
    DELAY_US(4);
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 MYIIC2_Wait_Ack(void)
{
    int ucErrTime=0;
    MYIIC2_SDA_IN();      //SDA设置为输入
    MYIIC2_SDA(1);
    DELAY_US(1);
    MYIIC2_SCL(1);
    DELAY_US(1);
    while(MYIIC2_READ_SDA)
    {
        ucErrTime++;
        if(ucErrTime>500)
        {
            MYIIC2_Stop();
            return 1;
        }
    }
    MYIIC2_SCL(0);//时钟输出0
    return 0;
}
//产生ACK应答
void MYIIC2_Ack(void)
{
    MYIIC2_SCL(0);
    MYIIC2_SDA_OUT();
    MYIIC2_SDA(0);
    DELAY_US(2);
    MYIIC2_SCL(1);
    DELAY_US(2);
    MYIIC2_SCL(0);
}
//不产生ACK应答
void MYIIC2_NAck(void)
{
    MYIIC2_SCL(0);
    MYIIC2_SDA_OUT();
    MYIIC2_SDA(1);
    DELAY_US(2);
    MYIIC2_SCL(1);
    DELAY_US(2);
    MYIIC2_SCL(0);
}
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
void MYIIC2_Send_Byte(u8 txd)
{
    u8 t;
    MYIIC2_SDA_OUT();
    MYIIC2_SCL(0);//拉低时钟开始数据传输
    for(t=0; t<8; t++)
    {
        MYIIC2_SDA((txd&0x80)>>7);
        txd<<=1;
        DELAY_US(2);   //对TEA5767这三个延时都是必须的
        MYIIC2_SCL(1);
        DELAY_US(2);
        MYIIC2_SCL(0);
        DELAY_US(2);
    }
}
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
u8 MYIIC2_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    MYIIC2_SDA_IN();//SDA设置为输入
    for(i=0; i<8; i++ )
    {
        MYIIC2_SCL(0);
        DELAY_US(2);
        MYIIC2_SCL(1);
        receive<<=1;
        if(MYIIC2_READ_SDA)receive++;
        DELAY_US(1);
    }
    if (!ack)
        MYIIC2_NAck();//发送nACK
    else
        MYIIC2_Ack(); //发送ACK
    return receive;
}
//void MYWriteReg(uint8_t slvaddr,uint8_t addr, uint8_t data)
HAL_StatusTypeDef MYIIC2_Mem_Write(I2C_HandleTypeDef *hi2c,uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    MYIIC2_Start();
    MYIIC2_Send_Byte(DevAddress);
    if(MYIIC2_Wait_Ack())
    {
        MYIIC2_Stop();
        return HAL_ERROR;
    }
    MYIIC2_Send_Byte(MemAddress);
    if(MYIIC2_Wait_Ack())
    {
        MYIIC2_Stop();
        return HAL_ERROR;
    }
    for(uint8_t i=0; i<Size; i++)
    {
        MYIIC2_Send_Byte(pData[i]);
        if(MYIIC2_Wait_Ack())
        {
            MYIIC2_Stop();
            return HAL_ERROR;
        }
    }
    MYIIC2_Stop();
    return HAL_OK;

}



HAL_StatusTypeDef MYIIC2_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{

    DELAY_US(10);
    MYIIC2_Start();
    MYIIC2_Send_Byte(DevAddress);
    if(MYIIC2_Wait_Ack())
    {
        MYIIC2_Stop();
        return HAL_ERROR;
    }
    MYIIC2_Send_Byte(MemAddress);
    if(MYIIC2_Wait_Ack())
    {
        MYIIC2_Stop();
        return HAL_ERROR;
    }


    MYIIC2_Start();
    MYIIC2_Send_Byte(DevAddress+1);
    if(MYIIC2_Wait_Ack())
    {
        MYIIC2_Stop();
        return HAL_ERROR;
    }
    for(uint8_t i=0; i<Size-1; i++)
    {


        pData[i] = MYIIC2_Read_Byte(1);

    }
    pData[Size-1] = MYIIC2_Read_Byte(0);

    MYIIC2_Stop();
    //正常返回
    return HAL_OK;

}








