#define _MAIN_C_

#include "main.h"
#include "cmsis_armcc.h"
#include "StandardSensorModule.h"
#define IWDG_STOP(A) 	  {RCC_APB1PeriphClockCmd(RCC_APB1ENR_DBGMCU, ENABLE);\
																DBGMCU_Configure(DBGMCU_CR_IWDG_STOP, A);}

sys_info_t sys_info;
debug_info_t debug_info;



volatile uint32_t timers[MAXTASKS];
volatile uint32_t UERROR = 1;

void SoftwareDelay(uint32_t Cnt)
{
  while(Cnt--);
}
void 	Sys_Init(void)
{
    	Delay_Init();
		Delay_Ms	(100);
//		Tim1_Init	();		//DAC�ö�ʱ��1���������DAC��δ�õ�
		Tim14_Init();
    	Led_Init	();		led_sys.led_sta = LED_FLASH_1HZ;
		LED_SYS_ON;
		Delay_Ms(1000);
		LED_SYS_OFF;
		Delay_Ms(1000); 
		LED_SYS_ON;
		Delay_Ms(1000);
		LED_SYS_OFF;
		Adc_Init	();
		Ldo_Init	();		ldo_sw.ldo_sta 	= LDO_ENABLE;
		Uart_Init (57600);
		memset(&debug_info,0,sizeof(debug_info_t));
		memset(&sys_info,	 0,sizeof(sys_info_t));
		
		paramDataInit();//�������㷨������ʼ��

//		Wwdg_ON(0x7e, 0x7f);	
#if 0	 // �������߷���ʱ���������³���Ρ�
		IWDG_STOP(ENABLE); 
#endif		
			  Write_Iwdg_ON(IWDG_Prescaler_64,100);
	
}
int main(void)
{
 		Sys_Init();
		
    while(1)
		{
			
			RunTask(ldo_task,       			1);
			RunTask(adc_task,       			2);
			RunTask(vol_res_task,     		3);
			RunTask(calculationTask,			4);
			RunTask(led_sys_task,       	5);	
			RunTask(send_data_task,				6);
			RunTask(wwdg_task,				7);
    }
}

