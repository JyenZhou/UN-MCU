
/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*                                                                     *
          �������� �ҵ�ģ��IIC1
					��1����ַ
					��2:5V����ʱ����Ҫ��IIC�ĵ�ƽת�� ����������ֵ

   ֱ�ӵ��� ADS1115_ConfigAndReadAVG(uint8_t channel,uint8_t VAGcount)  �ú�������
	       ����ֵҪ���� 0.000125���ǵ�ѹֵ
	 
	 ע�� �ð汾 ����ѹֻ����0-4.095
	 

					 ʵ�ʵ�ѹ=AD��ֵ/32768.0)*4.096;//0.000125


					 ����ѹ����
���ڲ�������ѹ������ѹ��8000~ffff,����ѹ��������0�����ƶԳƹ�ϵ����λȡ����+1��ͬ
if(data_get>=0x8000)
dianya=((float)(0xffff-data_get)/32768.0)*4.096;//0.000125
else
dianya=((float)data_get/32768.0)*4.096;

return dianya;
*                                                                     *
  *                                                                 *
   *                                                              *
*********@����*Jyen******************@����*Jyen***********************@����*Jyen********************/
#include "ads1115.h"


/*-----------------------------------------------------------------------*/
/**
  *@ �������ܻ���: ��ѹ��ȡ
  * @�������:0 ���� ֻ��������ѹ  1����ԭ������	
  * @�� �� ֵ: ��
  * @�ص�˵������
  * @���ߣ�Jyen Z
  *
  ******************************************************************************
  *
  *----------------------����Jyen---------------------����Jyen--------------------------*/

static uint32_t ADS1115_ReadData(uint8 Original)
{
    /*ָ��ADS1115ָ��Ĵ�������׼����ȡ����*/
    uint8_t	Readbuff[2];
    uint32_t conv_data = 0;
    Readbuff[0] = 0x00;
    Readbuff[1] = 0x00;

    //HAL_Delay(100);
    /*��ȡ����**/
    IIC1_Start();
    IIC1_SendByte(ADS115_ADDRESS_R);  //slave read
    IIC1_WaitAck();

    Readbuff[0]= IIC1_ReadByte();
    IIC1_Ack();
    Readbuff[1]= IIC1_ReadByte();
    IIC1_NAck();


    IIC1_Stop();
    conv_data=(Readbuff[0]<<8)|Readbuff[1];
	if(Original==0)
	{
    if(conv_data > 0x8000)//����ѹʱ��
    {
        conv_data = 0xffff-conv_data;
    }
	}
    return conv_data;
}

/*-----------------------------------------------------------------------*/
/**
  *@ �������ܻ���: ����ADS1115��ʼ��  
  * @�������:1.ͨ��  2.ģʽ��0�ǵ�ͨ��ģʽ��1�ǲ��ģʽ�� 3.���ȣ�0��6.144 1��4.096 2��2.048 3��1.024 4��0.512  5��0.256 ����û�����ˣ�                
  * @�� �� ֵ: ��
  * @�ص�˵��������Ĭ��Ϊ����ת��ģʽ
  * @���ߣ�Jyen Z
  *
  ******************************************************************************
000 : AINP = AIN0 and AINN = AIN1 (default)  100 : AINP = AIN0 and AINN = GND
001 : AINP = AIN0 and AINN = AIN3            101 : AINP = AIN1 and AINN = GND
010 : AINP = AIN1 and AINN = AIN3            110 : AINP = AIN2 and AINN = GND
011 : AINP = AIN2 and AINN = AIN3            111 : AINP = AIN3 and AINN = GND
  *----------------------����Jyen---------------------����Jyen--------------------------*/

static uint8_t ADS1115_Config(ADS1115Channel_TypeDefE channel ,uint8 Mode,uint8 Precision)
{
    uint8_t Chnel,WriteIntBuf[4],i;
    switch(channel)
    {

    case ADS1115Channel_1 :
			if(Mode==0)
			{
        Chnel=(0xc0);
			}else if(Mode==1)
			{
				// AINP = AIN0 and AINN = AIN1 
				 Chnel=(0x80);
			}		
        break;
    case ADS1115Channel_2:
      
					if(Mode==0)
			{
          Chnel=(0xd0);
			}else if(Mode==1)
			{
				 Chnel=(0x90);
			}
			
        break;
    case ADS1115Channel_3 :
			
					if(Mode==0)
			{
            Chnel=(0xe0);
			}else if(Mode==1)
			{
				 Chnel=(0xA0);
			}
      
        break;
    case ADS1115Channel_4:
					if(Mode==0)
			{
             Chnel=(0xf0);
			}else if(Mode==1)
			{
				 Chnel=(0xB0);
			}
      
        break;
    default :
        break;
    }
		//���� ����Ĭ������ת��
			if(Precision==0)
			{
				Chnel|=0x01;
			}else if(Precision==1)
			{
					Chnel|=0x03;
			}
			else if(Precision==2)
			{
				Chnel|=0x05;
			}
				else if(Precision==3)
			{
				Chnel|=0x07;
			}	else if(Precision==4)
			{
				Chnel|=0x09;
			}	else if(Precision==5)
			{
				Chnel|=0x0B;
			}
			
		
		
		
		
    WriteIntBuf[0] = ADS115_ADDRESS_W;      
/* ����
000 : FS = ��6.144V (1) 100 : FS = ��0.512V
001 : FS = ��4.096V (1) 101 : FS = ��0.256V
010 : FS = ��2.048V (default) 110 : FS = ��0.256V
011 : FS = ��1.024V 
		*/		
    WriteIntBuf[1] = 0x01;     //0x01 //ָ�����üĴ��� Table 6. Register Addres
    WriteIntBuf[2] = Chnel;  // д��Ҫ���õ�ͨ��
    WriteIntBuf[3] = 0x83;        // 83 e3 82    42
    //   3������0x42��//���ø�λ��ѡ��ΪAIN0����׼Ϊ4.096,����ת��ģʽ�����÷����ɸ������������
    //  4������0xe3��//���õ�λ��ת��ѡ860SPS�����ñȽϣ����÷����ɸ������������
    IIC1_Start();
    for(i=0; i<4; i++)
    {
        IIC1_SendByte(WriteIntBuf[i]);    /////���ͼĴ���������Ϣ
        if(IIC1_WaitAck())return 1;            ////ע�⣬�˴����������ʱû�У���������һֱ���䣬���Ϻ�����
    }
    IIC1_Stop();
    HAL_Delay(50);//Ads1115��ʱ
    return 0;
}

/*׼��ת��*/
static void ADS1115_Regrets (void)
{
    IIC1_Start();
    IIC1_SendByte(ADS115_ADDRESS_W);  //slave
    IIC1_WaitAck();
    IIC1_SendByte(0x00);//ת�����ݼĴ���
    IIC1_WaitAck();
    IIC1_Stop();
}




/*-----------------------------------------------------------------------*/
/**
  *@ �������ܻ���: ��ʼ��ͨ������ȡƽ��ֵ  �о���ƽ��ֵ���岻��  ��Ϊÿ�γ�ʼ��ͨ����Ż�ˢ��ֵ��̫��ʱ  ��������¶�һ�ξ�����
  * @�������:  ͨ��0-3  ƽ������ ��Сֵ1
  * @�� �� ֵ: ԭʼadֵ  Ҫ����  0.000125 ���ǵ�ѹ
  * @�ص�˵��������������ѹʱҪ����ÿ�ζ�ȡʱ��  ��Ҫʱ�ֿ���ʼ��  ת��  ��ȡ ������ܿ���סʱ���
  * @���ߣ�Jyen Z
  *
  ******************************************************************************
  *
  *----------------------����Jyen---------------------����Jyen--------------------------*/
uint32_t ADS1115_ConfigAndReadAVG(ADS1115Channel_TypeDefE channel,uint8_t VAGcount)
{
    uint8_t i=0;
    uint32_t AVGconv_data = 0;

    for(i=0; i<VAGcount; i++)
    {
			 
			  ADS1115_Config(channel,0,1);//��ʼ��ͨ��
        ADS1115_Config(channel,0,1);//��ʼ��ͨ������ ��ֹ�����ݴ���
        ADS1115_Regrets();//ת��׼����ȡ
			 HAL_Delay(50);//Ads1115��ʱ
			AVGconv_data= AVGconv_data+ ADS1115_ReadData(0);
    }
    AVGconv_data=AVGconv_data/VAGcount;
		float test=AVGconv_data*0.000125;	
    return AVGconv_data;
		
		
}


/*-----------------------------------------------------------------------*/
/**
  *@ �������ܻ���: Ads1115 ���ģʽ��ֵ
  * @�������:��                 
  * @�� �� ֵ: ��
  * @�ص�˵�������ȵȼ�Ҫ�͵�ǰ���� 
  * @���ߣ�Jyen Z
  *
  ******************************************************************************
  *
  *----------------------����Jyen---------------------����Jyen--------------------------*/
uint32_t  ADS1115_gainAmplifierConfigurAndReadAVG(ADS1115Channel_TypeDefE channel,uint8_t VAGcount)
{
     uint8_t i=0;
    uint32_t AVGconv_data = 0;

    for(i=0; i<VAGcount; i++)
    {
			
			  ADS1115_Config(channel,1,5);//��ʼ��ͨ��
        ADS1115_Config(channel,1,5);//��ʼ��ͨ������ ��ֹ�����ݴ���
        ADS1115_Regrets();//ת��׼����ȡ
			 HAL_Delay(50);//Ads1115��ʱ
			AVGconv_data= AVGconv_data+ ADS1115_ReadData(1);
    }
    AVGconv_data=AVGconv_data/VAGcount;
		float test=AVGconv_data/32768.0f*0.256f;
    return AVGconv_data;
}




