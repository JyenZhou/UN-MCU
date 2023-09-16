
#include "Hal_YIIC3.h"

/**
  * ��������: ����IIC3���ߵ�GPIO������ģ��IO�ķ�ʽʵ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void IIC3_InitGPIO(void)
{
#ifndef HAL_YIIC3_MODULE_ENABLED
    δ��ʼ��ģ��IIC3
#endif
    GPIO_InitTypeDef GPIO_InitStruct;

    /* ��GPIOʱ�� */
    IIC3_GPIO_CLK_ENABLE();

    GPIO_InitStruct.Pin = IIC3_SCL_PIN|IIC3_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(IIC3_GPIO_PORT, &GPIO_InitStruct);

    /* ��һ��ֹͣ�ź�, ��λIIC3�����ϵ������豸������ģʽ */
    IIC3_Stop();
}


static void IIC3_Delay(void)
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
    for (i = 0; i < 10; i++);
}

/**
  * ��������: CPU����IIC3���������ź�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void IIC3_Start(void)
{
    /* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾIIC3���������ź� */
    IIC3_SDA_HIGH();
    IIC3_SCL_HIGH();
    IIC3_Delay();
    IIC3_SDA_LOW();
    IIC3_Delay();
    IIC3_SCL_LOW();
    IIC3_Delay();
}

/**
  * ��������: CPU����IIC3����ֹͣ�ź�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void IIC3_Stop(void)
{
    /* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾIIC3����ֹͣ�ź� */
    IIC3_SDA_LOW();
    IIC3_SCL_HIGH();
    IIC3_Delay();
    IIC3_SDA_HIGH();
}

/**
  * ��������: CPU��IIC3�����豸����8bit����
  * �������: Byte �� �ȴ����͵��ֽ�
  * �� �� ֵ: ��
  * ˵    ������
  */
void IIC3_SendByte(uint8_t Byte)
{
    uint8_t i;

    /* �ȷ����ֽڵĸ�λbit7 */
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
            IIC3_SDA_HIGH(); // �ͷ�����
        }
        Byte <<= 1;	/* ����һ��bit */
        IIC3_Delay();
    }
}


/**
  * ��������: CPU��IIC3�����豸��ȡ8bit����
  * �������: ��
  * �� �� ֵ: ����������
  * ˵    ������
  */
uint8_t IIC3_ReadByte(void)
{
    uint8_t i;
    uint8_t value;

    /* ������1��bitΪ���ݵ�bit7 */
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
  * ��������: CPU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�
  * �������: ��
  * �� �� ֵ: ����0��ʾ��ȷӦ��1��ʾ��������Ӧ
  * ˵    ������
  */
uint8_t IIC3_WaitAck(void)
{
    uint8_t re;

    IIC3_SDA_HIGH();	/* CPU�ͷ�SDA���� */
    IIC3_Delay();
    IIC3_SCL_HIGH();	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
    IIC3_Delay();
    if (IIC3_SDA_READ())	/* CPU��ȡSDA����״̬ */
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
  * ��������: CPU����һ��ACK�ź�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void IIC3_Ack(void)
{
    IIC3_SDA_LOW();	/* CPU����SDA = 0 */
    IIC3_Delay();
    IIC3_SCL_HIGH();	/* CPU����1��ʱ�� */
    IIC3_Delay();
    IIC3_SCL_LOW();
    IIC3_Delay();
    IIC3_SDA_HIGH();	/* CPU�ͷ�SDA���� */
}

/**
  * ��������: CPU����1��NACK�ź�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void IIC3_NAck(void)
{
    IIC3_SDA_HIGH();	/* CPU����SDA = 1 */
    IIC3_Delay();
    IIC3_SCL_HIGH();	/* CPU����1��ʱ�� */
    IIC3_Delay();
    IIC3_SCL_LOW();
    IIC3_Delay();
}


