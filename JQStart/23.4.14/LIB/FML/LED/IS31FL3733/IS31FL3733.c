/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*                    �������� �ҵ�ģ��IIC1                                                   *
            IS31FL3733 LED�ƿ���оƬ������װ����
*                                                                     *
  *                                                                 *
   *                                                              *
*********@����*Jyen******************@����*Jyen***********************@����*Jyen********************/

#include "IS31FL3733.h"

uint8 RowLeftDataBuff[9];
uint8 RowReightDataBuff[9];
//��������������״̬�� ����0���� ������12*2  �������״̬����ʵ�ʵƵ�״̬һ��

uint32_t ADS1115_ReadData()
{
    /*ָ��ADS1115ָ��Ĵ�������׼����ȡ����*/
    uint8_t	Readbuff[2];
    uint32_t conv_data = 0;
    Readbuff[0] = 0x00;
    Readbuff[1] = 0x00;

    //HAL_Delay(100);
    /*��ȡ����**/
    IIC1_Start();
    IIC1_SendByte(0x91);  //slave read
    IIC1_WaitAck();

    Readbuff[0]= IIC1_ReadByte();
    IIC1_Ack();
    Readbuff[1]= IIC1_ReadByte();
    IIC1_NAck();


    IIC1_Stop();
    conv_data=(Readbuff[0]<<8)|Readbuff[1];
    if(conv_data > 0x8000)//����ѹʱ��
    {
        conv_data = 0xffff-conv_data;
    }
    return conv_data;
}



/*-----------------------------------------------------------------------*/
/**
  *@ �������ܻ���: Ҫѡ��PG0~PG3����Ҫ�Ƚ����üĴ������Ա���üĴ����������
              ����0xC5д��FEHʱ������FDH�޸�һ�Σ��޸�FDH(����Ĵ���)��FEH����������Ϊ0x00��
  * @�������:0x00 FDh write disable   0xC5 FDh write enable once
  * @�� �� ֵ: ��
  * @�ص�˵������
  * @���ߣ�Jyen Z
  *
  ******************************************************************************
  *
  *----------------------����Jyen---------------------����Jyen--------------------------*/
static/*��һ������ ���濴�����*/ void UnLoCk (void)
{
    IIC1_Start();
    IIC1_SendByte(DeviceAddress<<1|Write);
    IIC1_WaitAck();
    IIC1_SendByte(CommandRegisterWriteLock);
    IIC1_WaitAck();
    IIC1_SendByte(0xC5);
    IIC1_Stop();

}



/*-----------------------------------------------------------------------*/
/**
*@ �������ܻ���: LED�����п���  ����һ�ζԵƿ���ʱҪҪ�����ʶ
* @�������:1.�к� 1-12  2. ����������   3���� FF����ȫ�� 4.�Ƿ���IICֹͣ���һ�β���
  * @�� �� ֵ:  ���ؼĴ�����ַ
  * @�ص�˵������
  * @���ߣ�Jyen Z
  *
  ******************************************************************************
  *
  *----------------------����Jyen---------------------����Jyen--------------------------*/

//���ڴ洢���ƿ����мĴ�����ַ 0����
uint8 LedRows_ON_OFF_Control[13]={0x00,0x00,0x02,0x04,0x06,0x08,0x0A,0X0C,0X0E,0x10,0X12,0X14,0x16};
void Led_ON_OFF_LineControl(uint8 Row,Direction_TypeDefE direction,uint8 data,WhichoneWrite_TypeDefE WhichoneWrite)
{

    //��һ�ν���ʱҪ���͵��豸��ַ-->���ƼĴ���-->LED���ƼĴ��� -->֮��������������ݲ���
    if(WhichoneWrite==FirstWrite)
    {
        IIC1_Start();
        IIC1_SendByte(DeviceAddress<<1|Write);
        IIC1_WaitAck();
        IIC1_SendByte(ConfigureCommandRegister);
        IIC1_WaitAck();
        IIC1_SendByte(LEDCOntrolRegister);
        IIC1_WaitAck();
    }

    //������ö����ʹ�õľ���ʵ�ʵ�ֱַ�Ӳ���
    if(direction==Left)
    {
        IIC1_SendByte(LedRows_ON_OFF_Control[Row]);
			RowLeftDataBuff[Row]=data;
    }
    else
    {
        IIC1_SendByte((LedRows_ON_OFF_Control[Row]+0x01));
			RowReightDataBuff[Row]=data;
    }
    IIC1_WaitAck();
    IIC1_SendByte(data);
    IIC1_WaitAck();	
		 	if(WhichoneWrite==Last_Write||Once_Write==WhichoneWrite)
		{
			IIC1_Stop();
		}
}

/*
data|=(1<<n);					//��nλ��1
data&=~(1<<n);				//��nλ��0
data^=(1<<n);				//��nλȡ��
(data>>n)&1;				//ȡ��nλ��ֵ
*/


/*-----------------------------------------------------------------------*/
/**
  *@ �������ܻ���: LED ���صĶ�������
  * @�������:�к�1-12 ��1-16 �����  �Ƿ����׸�ͨ�ſ���                   
  * @�� �� ֵ: ��
  * @�ص�˵������
  * @���ߣ�Jyen Z
  *
  ******************************************************************************
  *
  *----------------------����Jyen---------------------����Jyen--------------------------*/
void Led_ON_OFF_RadioControl(uint8 Row,u8 Column,ONOrOFF_TypeDefE ONorOFF,WhichoneWrite_TypeDefE WhichoneWrite)
{
    Direction_TypeDefE direction;
    //����8��֮�� �еĵ�ַ����仯 //������������ʹ�õľ���ʵ�ʵ�ֱַ�Ӳ���
    if(Column<8)
    {
        direction=Left;
    }
    else
    {
        direction=Reight;
    }
    if(ONorOFF==ON)//��Ҫ����ʱ ��ָ��λ��1����
    {
        //�����������е� ����״̬
        RowLeftDataBuff[Row]=RowLeftDataBuff[Row]|(1<<Column);
        Led_ON_OFF_LineControl(Row,direction,RowLeftDataBuff[Row],WhichoneWrite);
    }
    else  //�����������е� ����״̬
    {
        RowReightDataBuff[Row]=RowReightDataBuff[Row]&(~(1<<Column));
        Led_ON_OFF_LineControl(Row,direction,RowReightDataBuff[Row],WhichoneWrite);
    }

}



/*-----------------------------------------------------------------------*/
/**
*@ �������ܻ���: LED PWM��������  ����һ�ζԵƿ���ʱҪҪ�����ʶ
* @�������:1.�� ö����Row1-12  2.��ö����Column1-16    3.PWM�ȼ�
  * @�� �� ֵ:  ���ؼĴ�����ַ
  * @�ص�˵������
  * @���ߣ�Jyen Z
  *
  ******************************************************************************
  *
  *----------------------����Jyen---------------------����Jyen--------------------------*/

//����13����0����
uint8 Led_Row_A_L[13]= {0x00,0x00,0x10,0x20,0x30,0x40,0x05,0x60,0x70,0x80,0x90,0xA0,0xB0};
//����17����0����
uint8 Led_Column_1_16[17]= {0x00,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0X0F};

//����λ�ÿ��Ƶ�ö��
void Led_PWM_RadioControl(uint8 Row,u8 Column,u8 PWMGrade,WhichoneWrite_TypeDefE WhichoneWrite)
{   //λ�õ�ַ
    uint8_t position;
    //��һ�ν���ʱҪ���͵��豸��ַ-->���ƼĴ���-->LED���ƼĴ��� -->֮��������������ݲ���
    if(WhichoneWrite==FirstWrite)
    {
        IIC1_Start();
        IIC1_SendByte(DeviceAddress<<1|Write);
        IIC1_WaitAck();
        IIC1_SendByte(ConfigureCommandRegister);
        IIC1_WaitAck();
        IIC1_SendByte(PWMRegister);
        IIC1_WaitAck();
    }

    //������������ʹ�õľ���ʵ�ʵ�ֱַ�Ӳ���
    position=Led_Row_A_L[Row]|Led_Column_1_16[Column];
    IIC1_SendByte(position);
    IIC1_WaitAck();
    IIC1_SendByte(PWMGrade);
    IIC1_WaitAck();
		 if(WhichoneWrite==Last_Write||Once_Write==WhichoneWrite)
		{
			IIC1_Stop();
		}
}




/*-----------------------------------------------------------------------*/
/**
*@ �������ܻ���: LED �Զ�����ģʽ��������  ����һ�ζԵƿ���ʱҪҪ�����ʶ
* @�������:1.�� ö����Row1-12  2.��ö����Column1-16    3.PWM�ȼ�
  * @�� �� ֵ:  ���ؼĴ�����ַ
  * @�ص�˵������
  * @���ߣ�Jyen Z
  *
  ******************************************************************************
  *
  *----------------------����Jyen---------------------����Jyen--------------------------*/
typedef enum
{
    PWMControlMode  =0x00,//PWM����ģʽ
    AutoBreathMode_1 =0x01,//�Զ�����ģʽ123
    AutoBreathMode_2 =0x02,
    AutoBreathMode_3 =0x03,

} AutoBreathMode_TypeDefE;
//�Զ�����ģʽ������
//����λ�ÿ��Ƶ�ö��
void Led_AutoBreath_RadioControl(uint8 Row,u8 Column,AutoBreathMode_TypeDefE autoBreathMode,WhichoneWrite_TypeDefE WhichoneWrite)
{
    uint8_t position;//λ�õ�ַ
    //��һ�ν���ʱҪ���͵��豸��ַ-->���ƼĴ���-->LED���ƼĴ��� -->֮��������������ݲ���
    if(WhichoneWrite==FirstWrite)
    {
        IIC1_Start();
        IIC1_SendByte(DeviceAddress<<1|Write);
        IIC1_WaitAck();
        IIC1_SendByte(ConfigureCommandRegister);
        IIC1_WaitAck();
        IIC1_SendByte(AutoBreathModeRegister);
        IIC1_WaitAck();
    }
    //������������ʹ�õľ���ʵ�ʵ�ֱַ�Ӳ���
    position=Led_Row_A_L[Row]|Led_Column_1_16[Column];
    IIC1_SendByte(position);
    IIC1_WaitAck();
    IIC1_SendByte(autoBreathMode);
		IIC1_WaitAck();
   	if(WhichoneWrite==Last_Write||Once_Write==WhichoneWrite)
		{
			IIC1_Stop();
		}
    
		
}



//�ƿ���1���Գ���  �п��� ��ʱֻ��������
void test_ON_LED1()
{
	    //1.�������ƼĴ���
    UnLoCk();
  Led_ON_OFF_LineControl(1,Left,0xFF,FirstWrite);
	Led_ON_OFF_LineControl(1,Reight,0xFF,Last_Write);
	HAL_Delay(1000);
	   //1.�������ƼĴ���
    UnLoCk();
	  Led_ON_OFF_LineControl(1,Left,0x00,FirstWrite);
	Led_ON_OFF_LineControl(1,Reight,0x00,Last_Write);
	HAL_Delay(1000);
	//������ �����ܲ��ܿ���
	 // UnLoCk();
  Led_ON_OFF_LineControl(1,Left,0xFF,FirstWrite);
	Led_ON_OFF_LineControl(2,Reight,0xFF,Last_Write);
		HAL_Delay(1000);
}
//�ƿ���2���Գ���  ����ˮ��
void test_ON_LED2()
{
    for(uint8 Rows; Rows<=12; Rows++)
    {
        for(uint8 Column; Column<=16; Column++)
        {
					 //1.�������ƼĴ���
     UnLoCk();
            Led_ON_OFF_RadioControl(Rows,Column,ON,Once_Write);
        }
    }
		HAL_Delay(1000);
		   for(uint8 Rows; Rows<=12; Rows++)
    {
        for(uint8 Column; Column<=16; Column++)
        {
					//1.�������ƼĴ���
             UnLoCk();
            Led_ON_OFF_RadioControl(Rows,Column,OFF,Once_Write);
        }
    }
	HAL_Delay(1000);
}


/*-----------------------------------------------------------------------*/
/**
  *@ �������ܻ���: ���Գ���
  * @�������:��
  * @�� �� ֵ: ��
  * @�ص�˵������
  * @���ߣ�Jyen Z
  *
  ******************************************************************************
  *
  *----------------------����Jyen---------------------����Jyen--------------------------*/
void IS31FL3733_Testmain (void)
{


    //1.���ò��Գ�����Ƶ�
    test_ON_LED1();
//	test_ON_LED2();
	
}
