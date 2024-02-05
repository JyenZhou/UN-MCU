
#include "SHT20_IIC.h"
#define SHT20_ODMODE

#ifdef USE_HAL_DRIVER



#ifdef  SHT20_ODMODE

static void SHT20_IIC_Delay(void)
{
    uint8_t i;

    /*　
     	下面的时间是通过逻辑分析仪测试得到的。
    	CPU主频72MHz时，在内部Flash运行, MDK工程不优化
    	循环次数为10时，SCL频率 = 205KHz
    	循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us
     	循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us

    IAR工程编译效率高，不能设置为7
    */
    for (i = 0; i < 10; i++);
}

/**
  * 函数功能: CPU发起SHT20_IIC总线启动信号
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void SHT20_IIC_Start(void)
{
    /* 当SCL高电平时，SDA出现一个下跳沿表示SHT20_IIC总线启动信号 */
    SHT20_IIC_SDA_HIGH();
    SHT20_IIC_SCL_HIGH();
    SHT20_IIC_Delay();
    SHT20_IIC_SDA_LOW();
    SHT20_IIC_Delay();
    SHT20_IIC_SCL_LOW();
    SHT20_IIC_Delay();
}

/**
  * 函数功能: CPU发起SHT20_IIC总线停止信号
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void SHT20_IIC_Stop(void)
{
    /* 当SCL高电平时，SDA出现一个上跳沿表示SHT20_IIC总线停止信号 */
    SHT20_IIC_SDA_LOW();
    SHT20_IIC_SCL_HIGH();
    SHT20_IIC_Delay();
    SHT20_IIC_SDA_HIGH();
}

/**
  * 函数功能: CPU向SHT20_IIC总线设备发送8bit数据
  * 输入参数: Byte ： 等待发送的字节
  * 返 回 值: 无
  * 说    明：无
  */
void SHT20_IIC_SendByte(uint8_t Byte)
{
    uint8_t i;

    /* 先发送字节的高位bit7 */
    for (i = 0; i < 8; i++)
    {
        if (Byte & 0x80)
        {
            SHT20_IIC_SDA_HIGH();
        }
        else
        {
            SHT20_IIC_SDA_LOW();
        }
        SHT20_IIC_Delay();
        SHT20_IIC_SCL_HIGH();
        SHT20_IIC_Delay();
        SHT20_IIC_SCL_LOW();
        if (i == 7)
        {
            SHT20_IIC_SDA_HIGH(); // 释放总线
        }
        Byte <<= 1;	/* 左移一个bit */
        SHT20_IIC_Delay();
    }
}


/**
  * 函数功能: CPU从SHT20_IIC总线设备读取8bit数据
  * 输入参数: 无
  * 返 回 值: 读到的数据
  * 说    明：无
  */
uint8_t SHT20_IIC_ReadByte(void)
{
    uint8_t i;
    uint8_t value;

    /* 读到第1个bit为数据的bit7 */
    value = 0;
    for (i = 0; i < 8; i++)
    {
        value <<= 1;
        SHT20_IIC_SCL_HIGH();
        SHT20_IIC_Delay();
        if (SHT20_IIC_SDA_READ())
        {
            value++;
        }
        SHT20_IIC_SCL_LOW();
        SHT20_IIC_Delay();
    }
    return value;
}

/**
  * 函数功能: CPU产生一个时钟，并读取器件的ACK应答信号
  * 输入参数: 无
  * 返 回 值: 返回0表示正确应答，1表示无器件响应
  * 说    明：无
  */
uint8_t SHT20_IIC_WaitAck(void)
{
    uint8_t re;

    SHT20_IIC_SDA_HIGH();	/* CPU释放SDA总线 */
    SHT20_IIC_Delay();
    SHT20_IIC_SCL_HIGH();	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
    SHT20_IIC_Delay();
    if (SHT20_IIC_SDA_READ())	/* CPU读取SDA口线状态 */
    {
        re = 1;
    }
    else
    {
        re = 0;
    }
    SHT20_IIC_SCL_LOW();
    SHT20_IIC_Delay();
    return re;
}

/**
  * 函数功能: CPU产生一个ACK信号
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void SHT20_IIC_Ack(void)
{
    SHT20_IIC_SDA_LOW();	/* CPU驱动SDA = 0 */
    SHT20_IIC_Delay();
    SHT20_IIC_SCL_HIGH();	/* CPU产生1个时钟 */
    SHT20_IIC_Delay();
    SHT20_IIC_SCL_LOW();
    SHT20_IIC_Delay();
    SHT20_IIC_SDA_HIGH();	/* CPU释放SDA总线 */
}

/**
  * 函数功能: CPU产生1个NACK信号
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void SHT20_IIC_NAck(void)
{
    SHT20_IIC_SDA_HIGH();	/* CPU驱动SDA = 1 */
    SHT20_IIC_Delay();
    SHT20_IIC_SCL_HIGH();	/* CPU产生1个时钟 */
    SHT20_IIC_Delay();
    SHT20_IIC_SCL_LOW();
    SHT20_IIC_Delay();
}

#else


//////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////

//US DELAY

/*outpp*/
//IO方向设置

void SHT20_IIC_SCL_OUT()
{   GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = SHT20_IIC_SCL_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SHT20_IIC_SCL_GPIO_PORT, &GPIO_InitStruct);
}
void SHT20_IIC_SDA_OUT()    {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = SHT20_IIC_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SHT20_IIC_SDA_GPIO_PORT, &GPIO_InitStruct);
}
void SHT20_IIC_SDA_IN()     {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = SHT20_IIC_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(SHT20_IIC_SDA_GPIO_PORT, &GPIO_InitStruct);
}
//IO操作
#define SHT20_IIC_SCL(n)   {n?HAL_GPIO_WritePin(SHT20_IIC_SCL_GPIO_PORT, SHT20_IIC_SCL_PIN, GPIO_PIN_SET):HAL_GPIO_WritePin(SHT20_IIC_SCL_GPIO_PORT, SHT20_IIC_SCL_PIN, GPIO_PIN_RESET);}
#define SHT20_IIC_SDA(n)  {n?HAL_GPIO_WritePin(SHT20_IIC_SDA_GPIO_PORT, SHT20_SDA_Pin, GPIO_PIN_SET):HAL_GPIO_WritePin(SHT20_IIC_SDA_GPIO_PORT, SHT20_SDA_Pin, GPIO_PIN_RESET);}
#define SHT20_IIC_READ_SDA  HAL_GPIO_ReadPin(SHT20_IIC_SDA_GPIO_PORT,SHT20_SDA_Pin)


static void DELAY_US(unsigned char us)
{
    unsigned char i;
    for(i=100; i>0; i--);
}


//产生IIC起始信号
void SHT20_IIC_Start(void)
{
    SHT20_IIC_SDA_OUT();     //sda线输出
    SHT20_IIC_SDA(1);
    SHT20_IIC_SCL(1);
    DELAY_US(4);
    SHT20_IIC_SDA(0);//START:when CLK is high,DATA change form high to low
    DELAY_US(4);
    SHT20_IIC_SCL(0);//钳住I2C总线，准备发送或接收数据
}
//产生IIC停止信号
void SHT20_IIC_Stop(void)
{
    SHT20_IIC_SDA_OUT();//sda线输出
    SHT20_IIC_SCL(0);
    SHT20_IIC_SDA(0);//STOP:when CLK is high DATA change form low to high
    DELAY_US(4);
    SHT20_IIC_SCL(1);
    SHT20_IIC_SDA(1);//发送I2C总线结束信号
    DELAY_US(4);
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 SHT20_IIC_WaitAck(void)
{
    int ucErrTime=0;
    SHT20_IIC_SDA_IN();      //SDA设置为输入
    SHT20_IIC_SDA(1);
    DELAY_US(1);
    SHT20_IIC_SCL(1);
    DELAY_US(1);
    while(SHT20_IIC_READ_SDA)
    {
        ucErrTime++;
        if(ucErrTime>500)
        {
            SHT20_IIC_Stop();
            return 1;
        }
    }
    SHT20_IIC_SCL(0);//时钟输出0
    return 0;
}
//产生ACK应答
void SHT20_IIC_Ack(void)
{
    SHT20_IIC_SCL(0);
    SHT20_IIC_SDA_OUT();
    SHT20_IIC_SDA(0);
    DELAY_US(2);
    SHT20_IIC_SCL(1);
    DELAY_US(2);
    SHT20_IIC_SCL(0);
}
//不产生ACK应答
void SHT20_IIC_NAck(void)
{
    SHT20_IIC_SCL(0);
    SHT20_IIC_SDA_OUT();
    SHT20_IIC_SDA(1);
    DELAY_US(2);
    SHT20_IIC_SCL(1);
    DELAY_US(2);
    SHT20_IIC_SCL(0);
}
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
void SHT20_IIC_SendByte(u8 txd)
{
    u8 t;
    SHT20_IIC_SDA_OUT();
    SHT20_IIC_SCL(0);//拉低时钟开始数据传输
    for(t=0; t<8; t++)
    {
        SHT20_IIC_SDA((txd&0x80)>>7);
        txd<<=1;
        DELAY_US(2);   //对TEA5767这三个延时都是必须的
        SHT20_IIC_SCL(1);
        DELAY_US(2);
        SHT20_IIC_SCL(0);
        DELAY_US(2);
    }
}
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
u8 SHT20_IIC_ReadByte()
{
    unsigned char i,receive=0;
    SHT20_IIC_SDA_IN();//SDA设置为输入
    for(i=0; i<8; i++ )
    {
        SHT20_IIC_SCL(0);
        DELAY_US(2);
        SHT20_IIC_SCL(1);
        receive<<=1;
        if(SHT20_IIC_READ_SDA)receive++;
        DELAY_US(1);
    }

    return receive;
}


void WriteReg(uint8_t slvaddr,uint8_t addr, uint8_t data)
{
    SHT20_IIC_Start();
    SHT20_IIC_SendByte(slvaddr);
    if(SHT20_IIC_WaitAck())
    {
        SHT20_IIC_Stop();
    }
    SHT20_IIC_SendByte(addr);
    SHT20_IIC_WaitAck();
    SHT20_IIC_SendByte(data);
    SHT20_IIC_WaitAck();
    SHT20_IIC_Stop();
}



uint8_t ReadReg(uint8_t slvaddr,uint8_t addr)
{
    u8 data;
// SHT20_IIC_Stop();
    DELAY_US(10);
    SHT20_IIC_Start();
    SHT20_IIC_SendByte( slvaddr);
    if(SHT20_IIC_WaitAck())
    {
        SHT20_IIC_Stop();
        return 0x00;
    }
    SHT20_IIC_SendByte(addr);
    if(SHT20_IIC_WaitAck())
    {
        SHT20_IIC_Stop();
        return 0x00;
    }
    SHT20_IIC_Start();
    SHT20_IIC_SendByte(slvaddr+1);
    if(SHT20_IIC_WaitAck())
    {
        SHT20_IIC_Stop();
        return 0x00;
    }
    data = SHT20_IIC_ReadByte();
    SHT20_IIC_NAck();//发送nACK
    SHT20_IIC_Stop();
    return data;
}


#endif


/**
  * 函数功能: 配置SHT20_IIC总线的GPIO，采用模拟IO的方式实现
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void SHT20_IIC_Init(void)
{
#ifdef  SHT20_ODMODE
    GPIO_InitTypeDef GPIO_InitStruct;
    /* 打开GPIO时钟 */
    SHT20_IIC_GPIO_SCL_CLK_ENABLE();

    GPIO_InitStruct.Pin = SHT20_IIC_SCL_PIN|SHT20_IIC_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(SHT20_IIC_SCL_GPIO_PORT, &GPIO_InitStruct);

    /* 给一个停止信号, 复位SHT20_IIC总线上的所有设备到待机模式 */
    SHT20_IIC_Stop();
#else

    SHT20_IIC_GPIO_SCL_CLK_ENABLE();
    SHT20_IIC_GPIO_SDA_CLK_ENABLE();
    SHT20_IIC_SCL_OUT();
    SHT20_IIC_SDA_OUT();
    SHT20_IIC_SCL(1);
    SHT20_IIC_SDA(1);

#endif
}


#endif
