

/**
  ��IIC�󶨵���NSA2003
  */
#include "Hal_YIIC5.h"

/**
  * ��������: ����IIC5���ߵ�GPIO������ģ��IO�ķ�ʽʵ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void IIC5_InitGPIO(void)
{
#ifndef 		HARDWARE_IIC5
#ifndef HAL_YIIC5_MODULE_ENABLED
    δ��ʼ��ģ��IIC5
#endif
    GPIO_InitTypeDef GPIO_InitStruct;

    /* ��GPIOʱ�� */
    IIC5_GPIO_CLK_ENABLE();

    GPIO_InitStruct.Pin = IIC5_SCL_PIN|IIC5_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(IIC5_GPIO_PORT, &GPIO_InitStruct);

    /* ��һ��ֹͣ�ź�, ��λIIC5�����ϵ������豸������ģʽ */
    IIC5_Stop();
#endif
}


static void IIC5_Delay(void)
{
    uint8_t i;

    /*��
     	�����ʱ����ͨ���߼������ǲ��Եõ��ġ�
    	CPU��Ƶ72MHzʱ�����ڲ�Flash����, MDK���̲��Ż�
    	ѭ������Ϊ10ʱ��SCLƵ�� = 205KHz
    	ѭ������Ϊ7ʱ��SCLƵ�� = 347KHz�� SCL�ߵ�ƽʱ��1.5us��SCL�͵�ƽʱ��2.87us
     	ѭ������Ϊ5ʱ��SCLƵ�� = 421KHz�� SCL�ߵ�ƽʱ��1.25us��SCL�͵�ƽʱ��2.375us

    IAR���̱���Ч�ʸߣ���������Ϊ7
    */
    for (i = 0; i < 20; i++);
}

/**
  * ��������: CPU����IIC5���������ź�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void IIC5_Start(void)
{
    /* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾIIC5���������ź� */
    IIC5_SDA_HIGH();
    IIC5_SCL_HIGH();
    IIC5_Delay();
    IIC5_SDA_LOW();
    IIC5_Delay();
    IIC5_SCL_LOW();
    IIC5_Delay();
}

/**
  * ��������: CPU����IIC5����ֹͣ�ź�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void IIC5_Stop(void)
{
    /* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾIIC5����ֹͣ�ź� */
    IIC5_SDA_LOW();
    IIC5_SCL_HIGH();
    IIC5_Delay();
    IIC5_SDA_HIGH();
}

/**
  * ��������: CPU��IIC5�����豸����8bit����
  * �������: Byte �� �ȴ����͵��ֽ�
  * �� �� ֵ: ��
  * ˵    ������
  */
void IIC5_SendByte(uint8_t Byte)
{
    uint8_t i;
    /* �ȷ����ֽڵĸ�λbit7 */
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
            IIC5_SDA_HIGH(); // �ͷ�����
        }
        Byte <<= 1;	/* ����һ��bit */
        IIC5_Delay();
    }
}


/**
  * ��������: CPU��IIC5�����豸��ȡ8bit����
  * �������: ��
  * �� �� ֵ: ����������
  * ˵    ������
  */
uint8_t IIC5_ReadByte(void)
{
    uint8_t i;
    uint8_t value;

    /* ������1��bitΪ���ݵ�bit7 */
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
  * ��������: CPU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�
  * �������: ��
  * �� �� ֵ: ����0��ʾ��ȷӦ��1��ʾ��������Ӧ
  * ˵    ������
  */
uint8_t IIC5_WaitAck(void)
{
    uint8_t re;

    IIC5_SDA_HIGH();	/* CPU�ͷ�SDA���� */
    IIC5_Delay();
    IIC5_SCL_HIGH();	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
    IIC5_Delay();
    if (IIC5_SDA_READ())	/* CPU��ȡSDA����״̬ */
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
  * ��������: CPU����һ��ACK�ź�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void IIC5_Ack(void)
{
    IIC5_SDA_LOW();	/* CPU����SDA = 0 */
    IIC5_Delay();
    IIC5_SCL_HIGH();	/* CPU����1��ʱ�� */
    IIC5_Delay();
    IIC5_SCL_LOW();
    IIC5_Delay();
    IIC5_SDA_HIGH();	/* CPU�ͷ�SDA���� */
}

/**
  * ��������: CPU����1��NACK�ź�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void IIC5_NAck(void)
{
    IIC5_SDA_HIGH();	/* CPU����SDA = 1 */
    IIC5_Delay();
    IIC5_SCL_HIGH();	/* CPU����1��ʱ�� */
    IIC5_Delay();
    IIC5_SCL_LOW();
    IIC5_Delay();
}


/**�滻Ӳ��I2Cʱ ֻ��Ҫ��������  ����Ҫ������ ��ȫ����Ӳ����ʹ���ﲿ�ֲ���δʹ������������
  * @brief  Write an amount of data in blocking mode to a specific memory address
  * @param  DevAddress �豸��ַ
  * @param  MemAddress �ڴ��ַ
  * @param  MemAddSize �ڴ��ַ��С(8��16)  ������Ҫ��Ϊ�˼���Ӳ��I2C�ĺ�����װ������˵����������Ϊ�˼���Ӳ��i2c
* @param  pData ����������
* @param  Size ���������ݳ���  ����Ҫ�����������������������ʽ��
  * @param  Timeout ��ʱʱ��
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
  * @param  DevAddress �豸��ַ
  * @param  MemAddress �ڴ��ַ
  * @param  MemAddSize �ڴ��ַ��С(8��16)  ������Ҫ��Ϊ�˼���Ӳ��I2C�ĺ�����װ������˵����������Ϊ�˼���Ӳ��i2c
* @param  pData ���ջ�����
* @param  Size �����������ݳ���  ����Ҫ�����������������������ʽ��
  * @param  Timeout ��ʱʱ��
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
//    IIC5_SDA_HIGH();	/* CPU����SDA = 1 */


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
    //��������
    return HAL_OK;
}



