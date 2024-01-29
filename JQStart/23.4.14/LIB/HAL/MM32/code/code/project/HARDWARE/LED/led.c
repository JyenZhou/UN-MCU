#define _LED_C_
/*
LED˵����

1��LED����ΪPA5
2��Ӧ�þ�����led_sys.led_sta = LED_OFF; �ر�LED
						 led_sys.led_sta = LED_ON;  ��LED
						 led_sys.led_sta = LED_FLASH_1HZ; LED  1HZ��˸
						 led_sys.led_sta = LED_FLASH_5HZ; LED  ��һ�룬������
3�����ڳ�������λ������LED״̬�����ǲ���������Ч�����led_sys_task���к���Ч
4������ѭ��֮ǰ���Ƶƿ�ֱ��ʹ�� LED_SYS_OFF/LED_SYS_ON

*/
#include "led.h"

led_t led_sys;

void Led_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

	
    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA, ENABLE);
	
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  =  LED_SYS_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LED_SYS_Port, &GPIO_InitStructure);

    LED_SYS_OFF;
}

uint32_t  led_sys_task(void){
_SS
  while(UERROR){
		if(led_sys.led_sta == LED_OFF)
		{
			LED_SYS_OFF;
			WaitX(100); 
		}
		else if(led_sys.led_sta == LED_ON)
		{
			LED_SYS_ON;
			WaitX(100); 
		}
		else if(led_sys.led_sta == LED_FLASH_1HZ)
		{
			LED_SYS_ON;
			WaitX(500); 
			LED_SYS_OFF;
			WaitX(500); 
		}
		else//5hz
		{
			LED_SYS_ON;
			WaitX(200); 
			LED_SYS_OFF;
			WaitX(200); 
		}
  }
_EE
}

