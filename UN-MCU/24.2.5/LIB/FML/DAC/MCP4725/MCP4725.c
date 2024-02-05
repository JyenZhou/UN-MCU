#include "MCP4725.h" 
 
//////////////////////////////////////////////////////////////////////////////////	 
//12λDA  IIC����	  
//////////////////////////////////////////////////////////////////////////////////



//��ʼ��MYIIC2�ӿ�
void MCP4725_Init(void)
{
	MYIIC2_Init();
}



void MCP4725_setVoltage_mV(uint16_t mVout)   //��ѹ��λmV 0-5000
{
 
	uint16_t Dn;
	
	Dn =  (mVout/5000.0*4096)-1  ;
	
	
	MCP4725_setVoltage_CodeValue(Dn);
}



 
void MCP4725_setVoltage_CodeValue(uint16_t codeValue)   //12λ������ 0-4095
{
	MYIIC2_Start(); 					//��ʼ�ź�
	MYIIC2_Send_Byte(0XC0);	    	//����д����  ��������1100 ����ַ000 ��д����0
	MYIIC2_Wait_Ack();
	
	MYIIC2_Send_Byte(0x0F&(codeValue>>8));	//0x0f|(DataToWrite>>8)(ֻȡ4λ)  ����ģʽ��C1=0,C2=0�� �ض�ѡ��PD1 = 0,PD2 =0)��ͨģʽ+��4λ����λ
	MYIIC2_Wait_Ack();
	
	MYIIC2_Send_Byte(0xFF&codeValue); //�������ݵĵ�8λ							   
	MYIIC2_Wait_Ack();

	MYIIC2_Stop();						//����һ��ֹͣ���� 
	HAL_Delay(10);
	
}





