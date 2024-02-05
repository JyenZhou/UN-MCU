

/**
  该IIC绑定的于NSA2003
  */
#include "Hal_YIIC5.h"

/**
  * 函数功能: 配置IIC5总线的GPIO，采用模拟IO的方式实现
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void IIC5_InitGPIO(void)
{
#ifndef 		HARDWARE_IIC5
#ifndef HAL_YIIC5_MODULE_ENABLED
    未初始化模拟IIC5
#endif
    GPIO_InitTypeDef GPIO_InitStruct;

    /* 打开GPIO时钟 */
    IIC5_GPIO_CLK_ENABLE();

    GPIO_InitStruct.Pin = IIC5_SCL_PIN|IIC5_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(IIC5_GPIO_PORT, &GPIO_InitStruct);

    /* 给一个停止信号, 复位IIC5总线上的所有设备到待机模式 */
    IIC5_Stop();
#endif
}


static void IIC5_Delay(void)
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
    for (i = 0; i < 20; i++);
}

/**
  * 函数功能: CPU发起IIC5总线启动信号
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void IIC5_Start(void)
{
    /* 当SCL高电平时，SDA出现一个下跳沿表示IIC5总线启动信号 */
    IIC5_SDA_HIGH();
    IIC5_SCL_HIGH();
    IIC5_Delay();
    IIC5_SDA_LOW();
    IIC5_Delay();
    IIC5_SCL_LOW();
    IIC5_Delay();
}

/**
  * 函数功能: CPU发起IIC5总线停止信号
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void IIC5_Stop(void)
{
    /* 当SCL高电平时，SDA出现一个上跳沿表示IIC5总线停止信号 */
    IIC5_SDA_LOW();
    IIC5_SCL_HIGH();
    IIC5_Delay();
    IIC5_SDA_HIGH();
}

/**
  * 函数功能: CPU向IIC5总线设备发送8bit数据
  * 输入参数: Byte ： 等待发送的字节
  * 返 回 值: 无
  * 说    明：无
  */
void IIC5_SendByte(uint8_t Byte)
{
    uint8_t i;
    /* 先发送字节的高位bit7 */
    for (i = 0; i < 8; i++)
    {
        if (Byte & 0x80)
        {
            IIC5_SDA_HIGH();
        }
        else
        {
            IIC5_SDA_LOW();
        }
        IIC5_Delay();
        IIC5_SCL_HIGH();
        IIC5_Delay();
        IIC5_SCL_LOW();
        if (i == 7)
        {
            IIC5_SDA_HIGH(); // 释放总线
        }
        Byte <<= 1;	/* 左移一个bit */
        IIC5_Delay();
    }
}


/**
  * 函数功能: CPU从IIC5总线设备读取8bit数据
  * 输入参数: 无
  * 返 回 值: 读到的数据
  * 说    明：无
  */
uint8_t IIC5_ReadByte(void)
{
    uint8_t i;
    uint8_t value;

    /* 读到第1个bit为数据的bit7 */
    value = 0;
    for (i = 0; i < 8; i++)
    {
        value <<= 1;
        IIC5_SCL_HIGH();
        IIC5_Delay();
        if (IIC5_SDA_READ())
        {
            value++;
        }
        IIC5_SCL_LOW();
        IIC5_Delay();
    }
    return value;
}

/**
  * 函数功能: CPU产生一个时钟，并读取器件的ACK应答信号
  * 输入参数: 无
  * 返 回 值: 返回0表示正确应答，1表示无器件响应
  * 说    明：无
  */
uint8_t IIC5_WaitAck(void)
{
    uint8_t re;

    IIC5_SDA_HIGH();	/* CPU释放SDA总线 */
    IIC5_Delay();
    IIC5_SCL_HIGH();	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
    IIC5_Delay();
    if (IIC5_SDA_READ())	/* CPU读取SDA口线状态 */
    {
        re = 1;
    }
    else
    {
        re = 0;
    }
    IIC5_SCL_LOW();
    IIC5_Delay();
    return re;
}

/**
  * 函数功能: CPU产生一个ACK信号
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void IIC5_Ack(void)
{
    IIC5_SDA_LOW();	/* CPU驱动SDA = 0 */
    IIC5_Delay();
    IIC5_SCL_HIGH();	/* CPU产生1个时钟 */
    IIC5_Delay();
    IIC5_SCL_LOW();
    IIC5_Delay();
    IIC5_SDA_HIGH();	/* CPU释放SDA总线 */
}

/**
  * 函数功能: CPU产生1个NACK信号
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void IIC5_NAck(void)
{
    IIC5_SDA_HIGH();	/* CPU驱动SDA = 1 */
    IIC5_Delay();
    IIC5_SCL_HIGH();	/* CPU产生1个时钟 */
    IIC5_Delay();
    IIC5_SCL_LOW();
    IIC5_Delay();
}


/**替换硬件I2C时 只需要换函数名  不需要动参数 完全兼容硬件的使这里部分参数未使用是正常现象
  * @brief  Write an amount of data in blocking mode to a specific memory address
  * @param  DevAddress 设备地址
  * @param  MemAddress 内存地址
  * @param  MemAddSize 内存地址大小(8或16)  这里主要是为了兼容硬件I2C的函数封装，或者说这整个就是为了兼容硬件i2c
* @param  pData 待发送数据
* @param  Size 待发送数据长度  （主要是上面待发送数据是数组形式）
  * @param  Timeout 超时时间
  * @retval HAL status
  */
HAL_StatusTypeDef IIC5_Mem_Write(I2C_HandleTypeDef *hi2c,uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    uint8_t dev_addr_w = DevAddress;
    IIC5_Start();
    IIC5_SendByte(dev_addr_w);
    if(IIC5_WaitAck())
    {
        IIC5_Stop();
        return HAL_ERROR;
    }
    IIC5_SendByte(MemAddress);
    if(IIC5_WaitAck())
    {
        IIC5_Stop();
        return HAL_ERROR;
    }
    for(uint8 i=0; i<Size; i++)
    {
        IIC5_SendByte(pData[i]);
        if(IIC5_WaitAck())
        {
            IIC5_Stop();
            return HAL_ERROR;
        }
    }
    IIC5_Stop();
    return HAL_OK;

}

/**
  * @brief  Write an amount of data in blocking mode to a specific memory address
  * @param  DevAddress 设备地址
  * @param  MemAddress 内存地址
  * @param  MemAddSize 内存地址大小(8或16)  这里主要是为了兼容硬件I2C的函数封装，或者说这整个就是为了兼容硬件i2c
* @param  pData 接收缓冲区
* @param  Size 待待接收数据长度  （主要是上面待发送数据是数组形式）
  * @param  Timeout 超时时间
  * @retval HAL status
  */
HAL_StatusTypeDef IIC5_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    IIC5_Delay();
    IIC5_Start();
    IIC5_SendByte(DevAddress);
    if(IIC5_WaitAck())
    {
        IIC5_Stop();
        return HAL_ERROR;
    }
    IIC5_SendByte(MemAddress);
    if(IIC5_WaitAck())
    {
        IIC5_Stop();
        return HAL_ERROR;
    }

    IIC5_Delay();
//    IIC5_SCL_LOW();
//    IIC5_Delay();
//    IIC5_SDA_HIGH();	/* CPU驱动SDA = 1 */


    IIC5_Start();
    IIC5_SendByte(DevAddress+1);
    if(IIC5_WaitAck())
    {
        IIC5_Stop();
        return HAL_ERROR;
    }
    for(uint8 i=0; i<Size-1; i++)
    {

        /*get one byte data*/
        pData[i] = IIC5_ReadByte();
        /*read ACK*/
        IIC5_Ack();
    }
    pData[Size-1] = IIC5_ReadByte();
    IIC5_Ack();
    IIC5_Stop();
    //正常返回
    return HAL_OK;
}



