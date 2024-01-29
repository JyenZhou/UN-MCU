#define _LED_C_
/*
LED说明：

1、LED引脚为PA5
2、应用举例：led_sys.led_sta = LED_OFF; 关闭LED
						 led_sys.led_sta = LED_ON;  打开LED
						 led_sys.led_sta = LED_FLASH_1HZ; LED  1HZ闪烁
						 led_sys.led_sta = LED_FLASH_5HZ; LED  亮一秒，灭三秒
3、可在程序任意位置设置LED状态，但是不会立即生效，需等led_sys_task运行后生效
4、在主循环之前控制灯可直接使用 LED_SYS_OFF/LED_SYS_ON

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


