#include "myiic0.h"

//////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////

//US DELAY


void DELAY_US(unsigned char us)
{
    unsigned char i;
    for(i=100; i>0; i--);
}


//IIC��ʼ��
void MYIIC0_Init(void)
{
#ifndef HAL_MYYIIC0_MODULE_ENABLED
    δ��ʼ��ģ��IIC0
#endif
#ifndef 		HARDWARE_MYIIC0
    MYIIC0_GPIO_CLK_ENABLE();
    MYIIC0_SDA_OUT();
    MYIIC0_SCL_OUT();
    MYIIC0_SDA(1);
    MYIIC0_SCL(1);

#endif

}

//����IIC��ʼ�ź�
void MYIIC0_Start(void)
{
    MYIIC0_SDA_OUT();     //sda�����
    MYIIC0_SDA(1);
    MYIIC0_SCL(1);
    DELAY_US(4);
    MYIIC0_SDA(0);//START:when CLK is high,DATA change form high to low
    DELAY_US(4);
    MYIIC0_SCL(0);//ǯסI2C���ߣ�׼�����ͻ��������
}
//����IICֹͣ�ź�
void MYIIC0_Stop(void)
{
    MYIIC0_SDA_OUT();//sda�����
    MYIIC0_SCL(0);
    MYIIC0_SDA(0);//STOP:when CLK is high DATA change form low to high
    DELAY_US(4);
    MYIIC0_SCL(1);
    MYIIC0_SDA(1);//����I2C���߽����ź�
    DELAY_US(4);
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 MYIIC0_Wait_Ack(void)
{
    int ucErrTime=0;
    MYIIC0_SDA_IN();      //SDA����Ϊ����
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
    MYIIC0_SCL(0);//ʱ�����0
    return 0;
}
//����ACKӦ��
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
//������ACKӦ��
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��
void MYIIC0_Send_Byte(u8 txd)
{
    u8 t;
    MYIIC0_SDA_OUT();
    MYIIC0_SCL(0);//����ʱ�ӿ�ʼ���ݴ���
    for(t=0; t<8; t++)
    {
        MYIIC0_SDA((txd&0x80)>>7);
        txd<<=1;
        DELAY_US(2);   //��TEA5767��������ʱ���Ǳ����
        MYIIC0_SCL(1);
        DELAY_US(2);
        MYIIC0_SCL(0);
        DELAY_US(2);
    }
}
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
u8 MYIIC0_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    MYIIC0_SDA_IN();//SDA����Ϊ����
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
        MYIIC0_NAck();//����nACK
    else
        MYIIC0_Ack(); //����ACK
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
    //��������
    return HAL_OK;

}








