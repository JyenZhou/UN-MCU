#ifndef __JSYS_Manufacturer__H__
#define  __JSYS_Manufacturer__H__
#ifdef __cplusPlus
extern "C" {
#endif
 
 
 
#ifdef  USE_HAL_DRIVER

#include "main.h"


#if    defined STM32F103xE 
//里面包含了core_cm3  内核版本是我需要的通过这个bootload升级时候跳转指令判断
#include "stm32f103xe.h"
 
 #elif defined STM32F103xB 
 
// #include "stm32f101xb.h"
 

#endif


#endif


#ifdef  MM32G0001
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




