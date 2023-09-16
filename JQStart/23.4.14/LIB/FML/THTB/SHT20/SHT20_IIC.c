
#include "SHT20_IIC.h"
#define SHT20_ODMODE

#ifdef USE_HAL_DRIVER



#ifdef  SHT20_ODMODE

static void SHT20_IIC_Delay(void)
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
  * ��������: CPU����SHT20_IIC���������ź�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void SHT20_IIC_Start(void)
{
    /* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾSHT20_IIC���������ź� */
    SHT20_IIC_SDA_HIGH();
    SHT20_IIC_SCL_HIGH();
    SHT20_IIC_Delay();
    SHT20_IIC_SDA_LOW();
    SHT20_IIC_Delay();
    SHT20_IIC_SCL_LOW();
    SHT20_IIC_Delay();
}

/**
  * ��������: CPU����SHT20_IIC����ֹͣ�ź�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void SHT20_IIC_Stop(void)
{
    /* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾSHT20_IIC����ֹͣ�ź� */
    SHT20_IIC_SDA_LOW();
    SHT20_IIC_SCL_HIGH();
    SHT20_IIC_Delay();
    SHT20_IIC_SDA_HIGH();
}

/**
  * ��������: CPU��SHT20_IIC�����豸����8bit����
  * �������: Byte �� �ȴ����͵��ֽ�
  * �� �� ֵ: ��
  * ˵    ������
  */
void SHT20_IIC_SendByte(uint8_t Byte)
{
    uint8_t i;

    /* �ȷ����ֽڵĸ�λbit7 */
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
            SHT20_IIC_SDA_HIGH(); // �ͷ�����
        }
        Byte <<= 1;	/* ����һ��bit */
        SHT20_IIC_Delay();
    }
}


/**
  * ��������: CPU��SHT20_IIC�����豸��ȡ8bit����
  * �������: ��
  * �� �� ֵ: ����������
  * ˵    ������
  */
uint8_t SHT20_IIC_ReadByte(void)
{
    uint8_t i;
    uint8_t value;

    /* ������1��bitΪ���ݵ�bit7 */
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
  * ��������: CPU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�
  * �������: ��
  * �� �� ֵ: ����0��ʾ��ȷӦ��1��ʾ��������Ӧ
  * ˵    ������
  */
uint8_t SHT20_IIC_WaitAck(void)
{
    uint8_t re;

    SHT20_IIC_SDA_HIGH();	/* CPU�ͷ�SDA���� */
    SHT20_IIC_Delay();
    SHT20_IIC_SCL_HIGH();	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
    SHT20_IIC_Delay();
    if (SHT20_IIC_SDA_READ())	/* CPU��ȡSDA����״̬ */
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
  * ��������: CPU����һ��ACK�ź�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void SHT20_IIC_Ack(void)
{
    SHT20_IIC_SDA_LOW();	/* CPU����SDA = 0 */
    SHT20_IIC_Delay();
    SHT20_IIC_SCL_HIGH();	/* CPU����1��ʱ�� */
    SHT20_IIC_Delay();
    SHT20_IIC_SCL_LOW();
    SHT20_IIC_Delay();
    SHT20_IIC_SDA_HIGH();	/* CPU�ͷ�SDA���� */
}

/**
  * ��������: CPU����1��NACK�ź�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void SHT20_IIC_NAck(void)
{
    SHT20_IIC_SDA_HIGH();	/* CPU����SDA = 1 */
    SHT20_IIC_Delay();
    SHT20_IIC_SCL_HIGH();	/* CPU����1��ʱ�� */
    SHT20_IIC_Delay();
    SHT20_IIC_SCL_LOW();
    SHT20_IIC_Delay();
}

#else


//////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////

//US DELAY

/*outpp*/
//IO��������

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
//IO����
#define SHT20_IIC_SCL(n)   {n?HAL_GPIO_WritePin(SHT20_IIC_SCL_GPIO_PORT, SHT20_IIC_SCL_PIN, GPIO_PIN_SET):HAL_GPIO_WritePin(SHT20_IIC_SCL_GPIO_PORT, SHT20_IIC_SCL_PIN, GPIO_PIN_RESET);}
#define SHT20_IIC_SDA(n)  {n?HAL_GPIO_WritePin(SHT20_IIC_SDA_GPIO_PORT, SHT20_SDA_Pin, GPIO_PIN_SET):HAL_GPIO_WritePin(SHT20_IIC_SDA_GPIO_PORT, SHT20_SDA_Pin, GPIO_PIN_RESET);}
#define SHT20_IIC_READ_SDA  HAL_GPIO_ReadPin(SHT20_IIC_SDA_GPIO_PORT,SHT20_SDA_Pin)


static void DELAY_US(unsigned char us)
{
    unsigned char i;
    for(i=100; i>0; i--);
}


//����IIC��ʼ�ź�
void SHT20_IIC_Start(void)
{
    SHT20_IIC_SDA_OUT();     //sda�����
    SHT20_IIC_SDA(1);
    SHT20_IIC_SCL(1);
    DELAY_US(4);
    SHT20_IIC_SDA(0);//START:when CLK is high,DATA change form high to low
    DELAY_US(4);
    SHT20_IIC_SCL(0);//ǯסI2C���ߣ�׼�����ͻ��������
}
//����IICֹͣ�ź�
void SHT20_IIC_Stop(void)
{
    SHT20_IIC_SDA_OUT();//sda�����
    SHT20_IIC_SCL(0);
    SHT20_IIC_SDA(0);//STOP:when CLK is high DATA change form low to high
    DELAY_US(4);
    SHT20_IIC_SCL(1);
    SHT20_IIC_SDA(1);//����I2C���߽����ź�
    DELAY_US(4);
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 SHT20_IIC_WaitAck(void)
{
    int ucErrTime=0;
    SHT20_IIC_SDA_IN();      //SDA����Ϊ����
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
    SHT20_IIC_SCL(0);//ʱ�����0
    return 0;
}
//����ACKӦ��
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
//������ACKӦ��
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��
void SHT20_IIC_SendByte(u8 txd)
{
    u8 t;
    SHT20_IIC_SDA_OUT();
    SHT20_IIC_SCL(0);//����ʱ�ӿ�ʼ���ݴ���
    for(t=0; t<8; t++)
    {
        SHT20_IIC_SDA((txd&0x80)>>7);
        txd<<=1;
        DELAY_US(2);   //��TEA5767��������ʱ���Ǳ����
        SHT20_IIC_SCL(1);
        DELAY_US(2);
        SHT20_IIC_SCL(0);
        DELAY_US(2);
    }
}
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
u8 SHT20_IIC_ReadByte()
{
    unsigned char i,receive=0;
    SHT20_IIC_SDA_IN();//SDA����Ϊ����
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
    SHT20_IIC_NAck();//����nACK
    SHT20_IIC_Stop();
    return data;
}


#endif


/**
  * ��������: ����SHT20_IIC���ߵ�GPIO������ģ��IO�ķ�ʽʵ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void SHT20_IIC_Init(void)
{
#ifdef  SHT20_ODMODE
    GPIO_InitTypeDef GPIO_InitStruct;
    /* ��GPIOʱ�� */
    SHT20_IIC_GPIO_SCL_CLK_ENABLE();

    GPIO_InitStruct.Pin = SHT20_IIC_SCL_PIN|SHT20_IIC_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(SHT20_IIC_SCL_GPIO_PORT, &GPIO_InitStruct);

    /* ��һ��ֹͣ�ź�, ��λSHT20_IIC�����ϵ������豸������ģʽ */
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
