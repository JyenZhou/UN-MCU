#include "MyIIC2.h"

//////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////

//US DELAY


void DELAY_US(unsigned char us)
{
    unsigned char i;
    for(i=100; i>0; i--);
}


//IIC��ʼ��
void MYIIC2_Init(void)
{
#ifndef HAL_MYYIIC0_MODULE_ENABLED
    δ��ʼ��ģ��IIC0
#endif
#ifndef 		HARDWARE_MYIIC2
    MYIIC2_GPIO_CLK_ENABLE();
    MYIIC2_SDA_OUT();
    MYIIC2_SCL_OUT();
    MYIIC2_SDA(1);
    MYIIC2_SCL(1);

#endif

}

//����IIC��ʼ�ź�
void MYIIC2_Start(void)
{
    MYIIC2_SDA_OUT();     //sda�����
    MYIIC2_SDA(1);
    MYIIC2_SCL(1);
    DELAY_US(4);
    MYIIC2_SDA(0);//START:when CLK is high,DATA change form high to low
    DELAY_US(4);
    MYIIC2_SCL(0);//ǯסI2C���ߣ�׼�����ͻ��������
}
//����IICֹͣ�ź�
void MYIIC2_Stop(void)
{
    MYIIC2_SDA_OUT();//sda�����
    MYIIC2_SCL(0);
    MYIIC2_SDA(0);//STOP:when CLK is high DATA change form low to high
    DELAY_US(4);
    MYIIC2_SCL(1);
    MYIIC2_SDA(1);//����I2C���߽����ź�
    DELAY_US(4);
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 MYIIC2_Wait_Ack(void)
{
    int ucErrTime=0;
    MYIIC2_SDA_IN();      //SDA����Ϊ����
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
    MYIIC2_SCL(0);//ʱ�����0
    return 0;
}
//����ACKӦ��
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
//������ACKӦ��
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��
void MYIIC2_Send_Byte(u8 txd)
{
    u8 t;
    MYIIC2_SDA_OUT();
    MYIIC2_SCL(0);//����ʱ�ӿ�ʼ���ݴ���
    for(t=0; t<8; t++)
    {
        MYIIC2_SDA((txd&0x80)>>7);
        txd<<=1;
        DELAY_US(2);   //��TEA5767��������ʱ���Ǳ����
        MYIIC2_SCL(1);
        DELAY_US(2);
        MYIIC2_SCL(0);
        DELAY_US(2);
    }
}
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
u8 MYIIC2_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    MYIIC2_SDA_IN();//SDA����Ϊ����
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
        MYIIC2_NAck();//����nACK
    else
        MYIIC2_Ack(); //����ACK
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
    //��������
    return HAL_OK;

}








