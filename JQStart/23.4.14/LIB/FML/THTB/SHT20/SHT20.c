/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*                     �������� �ҵ�ģ��SHT20_IIC                                                *

						�ú����Ƿ�����ģʽ�µ���ʪ�ȶ�ȡ��ʵ��������Ҳ��������ʽ ����ת�������Ͳ�ȡ��ʱ���ȡ�ˣ� ����������ɾ��

       ������   SHT20_PARAM  g_SHT20_param    �ⲿ�ṹ����

         ����������	 ����ֱ��������������Ǵ���ڲ��Ժ����ı���
				         g_SHT20_param.TEMP_HM = SHT20_MeasureTempHM();
                 g_SHT20_param.HUMI_HM = SHT20_MeasureHumiHM();
								 �������½��������մ˺�������ֵ
*                                                                     *
  *                                                                 *
   *                                                              *
*********@����*Jyen******************@����*Jyen***********************@����*Jyen********************/


#include "SHT20.h"
#include "SHT20_iic.h"

 

////////////////////////////////////////////*********************************************************////////////////
void SHT20_Init()
{
    SHT20_IIC_Init();
}

/***��ȡ�¶� */
uint16 SHT20_MeasureTemp(void)//��ȡ�¶�
{
    uint8_t Restart,Restarttimeout=15;

    u8 tmp1, tmp2;
    uint16 ST;
    do
    {
        Restart=0;
        SHT20_IIC_Start();
        SHT20_IIC_SendByte(I2C_ADR_W);//address+write
        if(SHT20_IIC_WaitAck())Restart=1;
        SHT20_IIC_SendByte(TRIG_TEMP_MEASUREMENT_POLL);//e3�����¶Ȳ���e5ʪ��
        if(SHT20_IIC_WaitAck())Restart=1;
        HAL_Delay(90);
        SHT20_IIC_Start();
        SHT20_IIC_SendByte(I2C_ADR_R);//address+read
        if(SHT20_IIC_WaitAck())Restart=1;

        tmp1 = SHT20_IIC_ReadByte();
        SHT20_IIC_Ack();
        tmp2 = SHT20_IIC_ReadByte();
        //  SHT20_IIC_Ack();
        //  crc = SHT20_IIC_ReadByte();
        SHT20_IIC_NAck();
        SHT20_IIC_Stop();

        ST = (tmp1 << 8) | (tmp2 << 0);
        ST &= ~0x0003;
    }
    while(Restart&Restarttimeout--);

    return ST;// TEMP = (float)ST*175.72f/0xffff-46.85f;//�����ܽϺ�ʱֱ�ӵ�Ƭ����((float)ST * 0.00268127) - 46.85;
}

/***��ȡʪ�� */
uint16 SHT20_MeasureHumi(void)
{
    uint8_t Restart,Restarttimeout=15;

    u8 tmp1, tmp2;
    uint16 SRH;
    do
    {
        Restart=0;
        SHT20_IIC_Start();
        SHT20_IIC_SendByte(I2C_ADR_W);//80 д
        if(SHT20_IIC_WaitAck())Restart=1;
        SHT20_IIC_SendByte(TRIG_HUMI_MEASUREMENT_POLL);//e3�����¶Ȳ���e5ʪ��
        if(SHT20_IIC_WaitAck())Restart=1;
        HAL_Delay(90);
        SHT20_IIC_Start();
        SHT20_IIC_SendByte(I2C_ADR_R);
        if(SHT20_IIC_WaitAck())Restart=1;
    }
    while(Restart&Restarttimeout--);
    tmp1 = SHT20_IIC_ReadByte();
    SHT20_IIC_Ack();
    tmp2 = SHT20_IIC_ReadByte();
//   c=SHT20_IIC_ReadByte();
    SHT20_IIC_NAck();
    SHT20_IIC_Stop();

    SRH = (tmp1 << 8) | (tmp2 << 0);
    SRH &= ~0x0003;
    //	g_SHT20_param.HUMI_HM_32=(uint16)SRH;




    return SRH;// HUMI = SRH*125.0f/0xffff-6.0f;	//�����ܽϺ�ʱֱ�ӵ�Ƭ����((float)SRH * 0.00190735) - 6;
}






float Temp=0,HUMI =0;
void SHT20_Test(void)
{

    SHT20_Init();
    Temp=   SHT20_MeasureTemp() *175.72f/0xffff-46.85f;//�����ܽϺ�ʱֱ�ӵ�Ƭ����((float)ST * 0.00268127) - 46.85;;
    HUMI =  SHT20_MeasureHumi()*125.0f/0xffff-6.0f;	//�����ܽϺ�ʱֱ�ӵ�Ƭ����((float)SRH * 0.00190735) - 6;



}




