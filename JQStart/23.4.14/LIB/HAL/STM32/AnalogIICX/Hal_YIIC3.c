
#include "Hal_YIIC3.h"

/**
  * 函数功能: 配置IIC3总线的GPIO，采用模拟IO的方式实现
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void IIC3_InitGPIO(void)
{
#ifndef HAL_YIIC3_MODULE_ENABLED
    未初始化模拟IIC3
#endif
    GPIO_InitTypeDef GPIO_InitStruct;

    /* 打开GPIO时钟 */
    IIC3_GPIO_CLK_ENABLE();

    GPIO_InitStruct.Pin = IIC3_SCL_PIN|IIC3_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(IIC3_GPIO_PORT, &GPIO_InitStruct);

    /* 给一个停止信号, 复位IIC3总线上的所有设备到待机模式 */
    IIC3_Stop();
}


static void IIC3_Delay(void)
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
  * 函数功能: CPU发起IIC3总线启动信号
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void IIC3_Start(void)
{
    /* 当SCL高电平时，SDA出现一个下跳沿表示IIC3总线启动信号 */
    IIC3_SDA_HIGH();
    IIC3_SCL_HIGH();
    IIC3_Delay();
    IIC3_SDA_LOW();
    IIC3_Delay();
    IIC3_SCL_LOW();
    IIC3_Delay();
}

/**
  * 函数功能: CPU发起IIC3总线停止信号
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void IIC3_Stop(void)
{
    /* 当SCL高电平时，SDA出现一个上跳沿表示IIC3总线停止信号 */
    IIC3_SDA_LOW();
    IIC3_SCL_HIGH();
    IIC3_Delay();
    IIC3_SDA_HIGH();
}

/**
  * 函数功能: CPU向IIC3总线设备发送8bit数据
  * 输入参数: Byte ： 等待发送的字节
  * 返 回 值: 无
  * 说    明：无
  */
void IIC3_SendByte(uint8_t Byte)
{
    uint8_t i;

    /* 先发送字节的高位bit7 */
    for (i = 0; i < 8; i++)
    {
        if (Byte & 0x80)
        {
            IIC3_SDA_HIGH();
        }
        else
        {
            IIC3_SDA_LOW();
        }
        IIC3_Delay();
        IIC3_SCL_HIGH();
        IIC3_Delay();
        IIC3_SCL_LOW();
        if (i == 7)
        {
            IIC3_SDA_HIGH(); // 释放总线
        }
        Byte <<= 1;	/* 左移一个bit */
        IIC3_Delay();
    }
}


/**
  * 函数功能: CPU从IIC3总线设备读取8bit数据
  * 输入参数: 无
  * 返 回 值: 读到的数据
  * 说    明：无
  */
uint8_t IIC3_ReadByte(void)
{
    uint8_t i;
    uint8_t value;

    /* 读到第1个bit为数据的bit7 */
    value = 0;
    for (i = 0; i < 8; i++)
    {
        value <<= 1;
        IIC3_SCL_HIGH();
        IIC3_Delay();
        if (IIC3_SDA_READ())
        {
            value++;
        }
        IIC3_SCL_LOW();
        IIC3_Delay();
    }
    return value;
}

/**
  * 函数功能: CPU产生一个时钟，并读取器件的ACK应答信号
  * 输入参数: 无
  * 返 回 值: 返回0表示正确应答，1表示无器件响应
  * 说    明：无
  */
uint8_t IIC3_WaitAck(void)
{
    uint8_t re;

    IIC3_SDA_HIGH();	/* CPU释放SDA总线 */
    IIC3_Delay();
    IIC3_SCL_HIGH();	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
    IIC3_Delay();
    if (IIC3_SDA_READ())	/* CPU读取SDA口线状态 */
    {
        re = 1;
    }
    else
    {
        re = 0;
    }
    IIC3_SCL_LOW();
    IIC3_Delay();
    return re;
}

/**
  * 函数功能: CPU产生一个ACK信号
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void IIC3_Ack(void)
{
    IIC3_SDA_LOW();	/* CPU驱动SDA = 0 */
    IIC3_Delay();
    IIC3_SCL_HIGH();	/* CPU产生1个时钟 */
    IIC3_Delay();
    IIC3_SCL_LOW();
    IIC3_Delay();
    IIC3_SDA_HIGH();	/* CPU释放SDA总线 */
}

/**
  * 函数功能: CPU产生1个NACK信号
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void IIC3_NAck(void)
{
    IIC3_SDA_HIGH();	/* CPU驱动SDA = 1 */
    IIC3_Delay();
    IIC3_SCL_HIGH();	/* CPU产生1个时钟 */
    IIC3_Delay();
    IIC3_SCL_LOW();
    IIC3_Delay();
}


