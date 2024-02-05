
#include "Hal_YIIC1.h"

/**
  * ��������: ����IIC1���ߵ�GPIO������ģ��IO�ķ�ʽʵ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void IIC1_InitGPIO(void)
{
#ifndef HAL_YIIC1_MODULE_ENABLED
    δ��ʼ��ģ��IIC2
#endif
    GPIO_InitTypeDef GPIO_InitStruct;

    /* ��GPIOʱ�� */
    IIC1_GPIO_CLK_ENABLE();

    GPIO_InitStruct.Pin = IIC1_SCL_PIN|IIC1_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(IIC1_GPIO_PORT, &GPIO_InitStruct);

    /* ��һ��ֹͣ�ź�, ��λIIC1�����ϵ������豸������ģʽ */
    IIC1_Stop();
}


static void IIC1_Delay(void)
{
    uint16_t i;

    /*��
     	�����ʱ����ͨ���߼������ǲ��Եõ��ġ�
    	CPU��Ƶ72MHzʱ�����ڲ�Flash����, MDK���̲��Ż�
    	ѭ������Ϊ10ʱ��SCLƵ�� = 205KHz
    	ѭ������Ϊ7ʱ��SCLƵ�� = 347KHz�� SCL�ߵ�ƽʱ��1.5us��SCL�͵�ƽʱ��2.87us
     	ѭ������Ϊ5ʱ��SCLƵ�� = 421KHz�� SCL�ߵ�ƽʱ��1.25us��SCL�͵�ƽʱ��2.375us

    IAR���̱���Ч�ʸߣ���������Ϊ7
    */
    for (i = 0; i < 100; i++);
}

/**
  * ��������: CPU����IIC1���������ź�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void IIC1_Start(void)
{
    /* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾIIC1���������ź� */
    IIC1_SDA_HIGH();
    IIC1_SCL_HIGH();
    IIC1_Delay();
    IIC1_SDA_LOW();
    IIC1_Delay();
    IIC1_SCL_LOW();
    IIC1_Delay();


}

/**
  * ��������: CPU����IIC1����ֹͣ�ź�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void IIC1_Stop(void)
{
    /* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾIIC1����ֹͣ�ź� */
    IIC1_SDA_LOW();
    IIC1_SCL_HIGH();
    IIC1_Delay();
    IIC1_SDA_HIGH();
}

/**
  * ��������: CPU��IIC1�����豸����8bit����
  * �������: Byte �� �ȴ����͵��ֽ�
  * �� �� ֵ: ��
  * ˵    ������
  */
void IIC1_SendByte(uint8_t Byte)
{
    uint8_t i;

    /* �ȷ����ֽڵĸ�λbit7 */
    for (i = 0; i < 8; i++)
    {
        if (Byte & 0x80)
        {
            IIC1_SDA_HIGH();
        }
        else
        {
            IIC1_SDA_LOW();
        }
        IIC1_Delay();
        IIC1_SCL_HIGH();
        IIC1_Delay();
        IIC1_SCL_LOW();
        if (i == 7)
        {
            IIC1_SDA_HIGH(); // �ͷ�����
        }
        Byte <<= 1;	/* ����һ��bit */
        IIC1_Delay();
    }
}


/**
  * ��������: CPU��IIC1�����豸��ȡ8bit����
  * �������: ��
  * �� �� ֵ: ����������
  * ˵    ������
  */
uint8_t IIC1_ReadByte(void)
{
    uint8_t i;
    uint8_t value;

    /* ������1��bitΪ���ݵ�bit7 */
    value = 0;
    for (i = 0; i < 8; i++)
    {
        value <<= 1;
        IIC1_SCL_HIGH();
        IIC1_Delay();
        if (IIC1_SDA_READ())
        {
            value++;
        }
        IIC1_SCL_LOW();
        IIC1_Delay();
    }
    return value;
}

/**
  * ��������: CPU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�
  * �������: ��
  * �� �� ֵ: ����0��ʾ��ȷӦ��1��ʾ��������Ӧ
  * ˵    ������
  */
uint8_t IIC1_WaitAck(void)
{
    uint8_t re;

    IIC1_SDA_HIGH();	/* CPU�ͷ�SDA���� */
    IIC1_Delay();
    IIC1_SCL_HIGH();	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
    IIC1_Delay();
    if (IIC1_SDA_READ())	/* CPU��ȡSDA����״̬ */
    {
        re = 1;
    }
    else
    {
        re = 0;
    }
    IIC1_SCL_LOW();
    IIC1_Delay();
    return re;
}

/**
  * ��������: CPU����һ��ACK�ź�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void IIC1_Ack(void)
{
    IIC1_SDA_LOW();	/* CPU����SDA = 0 */
    IIC1_Delay();
    IIC1_SCL_HIGH();	/* CPU����1��ʱ�� */
    IIC1_Delay();
    IIC1_SCL_LOW();
    IIC1_Delay();
    IIC1_SDA_HIGH();	/* CPU�ͷ�SDA���� */
}

/**
  * ��������: CPU����1��NACK�ź�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void IIC1_NAck(void)
{
    IIC1_SDA_HIGH();	/* CPU����SDA = 1 */
    IIC1_Delay();
    IIC1_SCL_HIGH();	/* CPU����1��ʱ�� */
    IIC1_Delay();
    IIC1_SCL_LOW();
    IIC1_Delay();
}


