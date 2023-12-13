
#include "ADS1114.h"

void ADS1114_Init()
{
	 MYIIC3_Init();   

}



/*����ֵ*0.000125 �ǵ�ѹ*/
uint16_t ADS1114_Read()
{
uint8_t AdcResult[2];
	MYIIC3_Start(); 					//��ʼ�ź�
	MYIIC3_Send_Byte(0X92);	    	//  �ӻ���ַ+1115��
	MYIIC3_Wait_Ack();
	MYIIC3_Send_Byte(0X01);	    	//���üĴ�����ַΪ01 
 MYIIC3_Wait_Ack();
	MYIIC3_Send_Byte(0xC3);	    	//4.096
	  MYIIC3_Wait_Ack();
	MYIIC3_Send_Byte(0X83);	    	//
	  MYIIC3_Wait_Ack();
	MYIIC3_Stop();						//����һ��ֹͣ���� 
	HAL_Delay(1);

		MYIIC3_Start();
    MYIIC3_Send_Byte(0x92);  //
    MYIIC3_Wait_Ack();
    MYIIC3_Send_Byte(0x00);//
    MYIIC3_Wait_Ack();
    MYIIC3_Stop();
		HAL_Delay(1);

	MYIIC3_Start();
    MYIIC3_Send_Byte(0x93);  //slave read
    MYIIC3_Wait_Ack();
	AdcResult[0]= MYIIC3_Read_Byte(1);
   AdcResult[1]= MYIIC3_Read_Byte(1);

	
	MYIIC3_Stop();						//����һ��ֹͣ���� 
	HAL_Delay(10);	
	
	return (AdcResult[0]<<8)+AdcResult[1];

}

