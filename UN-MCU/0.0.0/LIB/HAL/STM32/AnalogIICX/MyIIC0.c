#include "myiic0.h"

//////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////

//US DELAY


void DELAY_US(unsigned char us)
{
    unsigned char i;
    for(i=100; i>0; i--);
}


//IIC初始化
void MYIIC0_Init(void)
{
#ifndef HAL_MYYIIC0_MODULE_ENABLED
    未初始化模拟IIC0
#endif
#ifndef 		HARDWARE_MYIIC0
    MYIIC0_GPIO_CLK_ENABLE();
    MYIIC0_SDA_OUT();
    MYIIC0_SCL_OUT();
    MYIIC0_SDA(1);
    MYIIC0_SCL(1);

#endif

}

//产生IIC起始信号
void MYIIC0_Start(void)
{
    MYIIC0_SDA_OUT();     //sda线输出
    MYIIC0_SDA(1);
    MYIIC0_SCL(1);
    DELAY_US(4);
    MYIIC0_SDA(0);//START:when CLK is high,DATA change form high to low
    DELAY_US(4);
    MYIIC0_SCL(0);//钳住I2C总线，准备发送或接收数据
}
//产生IIC停止信号
void MYIIC0_Stop(void)
{
    MYIIC0_SDA_OUT();//sda线输出
    MYIIC0_SCL(0);
    MYIIC0_SDA(0);//STOP:when CLK is high DATA change form low to high
    DELAY_US(4);
    MYIIC0_SCL(1);
    MYIIC0_SDA(1);//发送I2C总线结束信号
    DELAY_US(4);
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 MYIIC0_Wait_Ack(void)
{
    int ucErrTime=0;
    MYIIC0_SDA_IN();      //SDA设置为输入
    MYIIC0_SDA(1);
    DELAY_US(1);
    MYIIC0_SCL(1);
    DELAY_US(1);
    while(MYIIC0_READ_SDA)
    {
        ucErrTime++;
        if(ucErrTime>500)
        {
            MYIIC0_Stop();
            return 1;
        }
    }
    MYIIC0_SCL(0);//时钟输出0
    return 0;
}
//产生ACK应答
void MYIIC0_Ack(void)
{
    MYIIC0_SCL(0);
    MYIIC0_SDA_OUT();
    MYIIC0_SDA(0);
    DELAY_US(2);
    MYIIC0_SCL(1);
    DELAY_US(2);
    MYIIC0_SCL(0);
}
//不产生ACK应答
void MYIIC0_NAck(void)
{
    MYIIC0_SCL(0);
    MYIIC0_SDA_OUT();
    MYIIC0_SDA(1);
    DELAY_US(2);
    MYIIC0_SCL(1);
    DELAY_US(2);
    MYIIC0_SCL(0);
}
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
void MYIIC0_Send_Byte(u8 txd)
{
    u8 t;
    MYIIC0_SDA_OUT();
    MYIIC0_SCL(0);//拉低时钟开始数据传输
    for(t=0; t<8; t++)
    {
        MYIIC0_SDA((txd&0x80)>>7);
        txd<<=1;
        DELAY_US(2);   //对TEA5767这三个延时都是必须的
        MYIIC0_SCL(1);
        DELAY_US(2);
        MYIIC0_SCL(0);
        DELAY_US(2);
    }
}
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
u8 MYIIC0_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    MYIIC0_SDA_IN();//SDA设置为输入
    for(i=0; i<8; i++ )
    {
        MYIIC0_SCL(0);
        DELAY_US(2);
        MYIIC0_SCL(1);
        receive<<=1;
        if(MYIIC0_READ_SDA)receive++;
        DELAY_US(1);
    }
    if (!ack)
        MYIIC0_NAck();//发送nACK
    else
        MYIIC0_Ack(); //发送ACK
    return receive;
}
//void MYWriteReg(uint8_t slvaddr,uint8_t addr, uint8_t data)
HAL_StatusTypeDef MYIIC0_Mem_Write(I2C_HandleTypeDef *hi2c,uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    MYIIC0_Start();
    MYIIC0_Send_Byte(DevAddress);
    if(MYIIC0_Wait_Ack())
    {
        MYIIC0_Stop();
        return HAL_ERROR;
    }
    MYIIC0_Send_Byte(MemAddress);
    if(MYIIC0_Wait_Ack())
    {
        MYIIC0_Stop();
        return HAL_ERROR;
    }
    for(uint8 i=0; i<Size; i++)
    {
        MYIIC0_Send_Byte(pData[i]);
        if(MYIIC0_Wait_Ack())
        {
            MYIIC0_Stop();
            return HAL_ERROR;
        }
    }
    MYIIC0_Stop();
    return HAL_OK;

}



HAL_StatusTypeDef MYIIC0_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{

    DELAY_US(10);
    MYIIC0_Start();
    MYIIC0_Send_Byte(DevAddress);
    if(MYIIC0_Wait_Ack())
    {
        MYIIC0_Stop();
        return HAL_ERROR;
    }
    MYIIC0_Send_Byte(MemAddress);
    if(MYIIC0_Wait_Ack())
    {
        MYIIC0_Stop();
        return HAL_ERROR;
    }


    MYIIC0_Start();
    MYIIC0_Send_Byte(DevAddress+1);
    if(MYIIC0_Wait_Ack())
    {
        MYIIC0_Stop();
        return HAL_ERROR;
    }
    for(uint8 i=0; i<Size-1; i++)
    {


        pData[i] = MYIIC0_Read_Byte(1);

    }
    pData[Size-1] = MYIIC0_Read_Byte(0);

    MYIIC0_Stop();
    //正常返回
    return HAL_OK;

}








