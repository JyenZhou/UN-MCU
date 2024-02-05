#include "MCP4725.h" 
 
//////////////////////////////////////////////////////////////////////////////////	 
//12位DA  IIC驱动	  
//////////////////////////////////////////////////////////////////////////////////



//初始化MYIIC2接口
void MCP4725_Init(void)
{
	MYIIC2_Init();
}



void MCP4725_setVoltage_mV(uint16_t mVout)   //电压单位mV 0-5000
{
 
	uint16_t Dn;
	
	Dn =  (mVout/5000.0*4096)-1  ;
	
	
	MCP4725_setVoltage_CodeValue(Dn);
}



 
void MCP4725_setVoltage_CodeValue(uint16_t codeValue)   //12位数字量 0-4095
{
	MYIIC2_Start(); 					//开始信号
	MYIIC2_Send_Byte(0XC0);	    	//发送写命令  器件代码1100 ，地址000 ，写操作0
	MYIIC2_Wait_Ack();
	
	MYIIC2_Send_Byte(0x0F&(codeValue>>8));	//0x0f|(DataToWrite>>8)(只取4位)  快速模式（C1=0,C2=0） 关断选择（PD1 = 0,PD2 =0)普通模式+高4位数据位
	MYIIC2_Wait_Ack();
	
	MYIIC2_Send_Byte(0xFF&codeValue); //发送数据的低8位							   
	MYIIC2_Wait_Ack();

	MYIIC2_Stop();						//产生一个停止条件 
	HAL_Delay(10);
	
}





