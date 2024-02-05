
#include "ADS1114.h"

void ADS1114_Init()
{
	 MYIIC3_Init();   

}



/*返回值*0.000125 是电压*/
uint16_t ADS1114_Read()
{
uint8_t AdcResult[2];
	MYIIC3_Start(); 					//开始信号
	MYIIC3_Send_Byte(0X92);	    	//  从机地址+1115读
	MYIIC3_Wait_Ack();
	MYIIC3_Send_Byte(0X01);	    	//配置寄存器地址为01 
 MYIIC3_Wait_Ack();
	MYIIC3_Send_Byte(0xC3);	    	//4.096
	  MYIIC3_Wait_Ack();
	MYIIC3_Send_Byte(0X83);	    	//
	  MYIIC3_Wait_Ack();
	MYIIC3_Stop();						//产生一个停止条件 
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

	
	MYIIC3_Stop();						//产生一个停止条件 
	HAL_Delay(10);	
	
	return (AdcResult[0]<<8)+AdcResult[1];

}

