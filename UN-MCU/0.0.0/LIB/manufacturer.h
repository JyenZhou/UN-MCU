#ifndef __JSYS_Manufacturer__H__
#define  __JSYS_Manufacturer__H__
#ifdef __CplusPlus
extern "C" {
#endif
 
 
 
#ifdef  USE_HAL_DRIVER

#include "main.h"


#if    defined STM32F103xE 
//���������core_cm3  �ں˰汾������Ҫ��ͨ�����bootload����ʱ����תָ���ж�
#include "stm32f103xe.h"
 
 #elif defined STM32F103xB 
 
// #include "stm32f101xb.h"
 

#endif


#endif


#ifdef  USE_STDPERIPH_DRIVER
#include "HAL/MM32/Manufacturer/hal_conf.h"
#include "HAL/MM32/Manufacturer/mm32_device.h"

#endif

#ifdef  TC04
#include "HAL/TC/Manufacturer/TC04/tc04xx_uart.h"
#include "HAL/TC/Manufacturer/TC04/tc04xx_debug.h"
#include "HAL/TC/Manufacturer/TC04/tc04xx_delay.h"
#include "HAL/TC/Manufacturer/TC04/tc04xx_gpio.h"

#endif
 
 
 
#ifdef __cplusplus
}
#endif

#endif




