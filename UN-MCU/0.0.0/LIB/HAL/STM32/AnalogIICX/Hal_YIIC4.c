
#include "Hal_YIIC4.h"

/**
  * ��������: ����IIC4���ߵ�GPIO������ģ��IO�ķ�ʽʵ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void IIC4_InitGPIO(void)
{
#ifndef HAL_YIIC4_MODULE_ENABLED
    δ��ʼ��ģ��IIC4
#endif
    GPIO_InitTypeDef GPIO_InitStruct;

    /* ��GPIOʱ�� */
    IIC4_GPIO_CLK_ENABLE();

    GPIO_InitStruct.Pin = IIC4_SCL_PIN|IIC4_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(IIC4_GPIO_PORT, &GPIO_InitStruct);

    /* ��һ��ֹͣ�ź�, ��λIIC4�����ϵ������豸������ģʽ */
    IIC4_Stop();
}


static void IIC4_Delay(void)
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
  * ��������: CPU����IIC4���������ź�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void IIC4_Start(void)
{
    /* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾIIC4���������ź� */
    IIC4_SDA_HIGH();
    IIC4_SCL_HIGH();
    IIC4_Delay();
    IIC4_SDA_LOW();
    IIC4_Delay();
    IIC4_SCL_LOW();
    IIC4_Delay();
}

/**
  * ��������: CPU����IIC4����ֹͣ�ź�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void IIC4_Stop(void)
{
    /* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾIIC4����ֹͣ�ź� */
    IIC4_SDA_LOW();
    IIC4_SCL_HIGH();
    IIC4_Delay();
    IIC4_SDA_HIGH();
}

/**
  * ��������: CPU��IIC4�����豸����8bit����
  * �������: Byte �� �ȴ����͵��ֽ�
  * �� �� ֵ: ��
  * ˵    ������
  */
void IIC4_SendByte(uint8_t Byte)
{
    uint8_t i;

    /* �ȷ����ֽڵĸ�λbit7 */
    for (i = 0; i < 8; i++)
    {
        if (Byte & 0x80)
        {
            IIC4_SDA_HIGH();
        }
        else
        {
            IIC4_SDA_LOW();
        }
        IIC4_Delay();
        IIC4_SCL_HIGH();
        IIC4_Delay();
        IIC4_SCL_LOW();
        if (i == 7)
        {
            IIC4_SDA_HIGH(); // �ͷ�����
        }
        Byte <<= 1;	/* ����һ��bit */
        IIC4_Delay();
    }
}


/**
  * ��������: CPU��IIC4�����豸��ȡ8bit����
  * �������: ��
  * �� �� ֵ: ����������
  * ˵    ������
  */
uint8_t IIC4_ReadByte(void)
{
    uint8_t i;
    uint8_t value;

    /* ������1��bitΪ���ݵ�bit7 */
    value = 0;
    for (i = 0; i < 8; i++)
    {
        value <<= 1;
        IIC4_SCL_HIGH();
        IIC4_Delay();
        if (IIC4_SDA_READ())
        {
            value++;
        }
        IIC4_SCL_LOW();
        IIC4_Delay();
    }
    return value;
}

/**
  * ��������: CPU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�
  * �������: ��
  * �� �� ֵ: ����0��ʾ��ȷӦ��1��ʾ��������Ӧ
  * ˵    ������
  */
uint8_t IIC4_WaitAck(void)
{
    uint8_t re;

    IIC4_SDA_HIGH();	/* CPU�ͷ�SDA���� */
    IIC4_Delay();
    IIC4_SCL_HIGH();	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
    IIC4_Delay();
    if (IIC4_SDA_READ())	/* CPU��ȡSDA����״̬ */
    {
        re = 1;
    }
    else
    {
        re = 0;
    }
    IIC4_SCL_LOW();
    IIC4_Delay();
    return re;
}

/**
  * ��������: CPU����һ��ACK�ź�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void IIC4_Ack(void)
{
    IIC4_SDA_LOW();	/* CPU����SDA = 0 */
    IIC4_Delay();
    IIC4_SCL_HIGH();	/* CPU����1��ʱ�� */
    IIC4_Delay();
    IIC4_SCL_LOW();
    IIC4_Delay();
    IIC4_SDA_HIGH();	/* CPU�ͷ�SDA���� */
}

/**
  * ��������: CPU����1��NACK�ź�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void IIC4_NAck(void)
{
    IIC4_SDA_HIGH();	/* CPU����SDA = 1 */
    IIC4_Delay();
    IIC4_SCL_HIGH();	/* CPU����1��ʱ�� */
    IIC4_Delay();
    IIC4_SCL_LOW();
    IIC4_Delay();
}


