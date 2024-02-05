/**
  ******************************************************************************
  * @file    传感器模块测试程序
  * @author  dotod
  * @version V1.0
  * @date    10-DEC-2017
  * @brief  
  ******************************************************************************
  * @attention	
  *
  ******************************************************************************
  */ 
	
#ifndef __KEYBOARD_H
#define __KEYBOARD_H
#include "JHAL.h"
/* Includes -----------------------------------------------------------------*/

#include <stdio.h>

/* 宏定义 -------------------------------------------------------------------*/
#define GPIOX GPIOB

	
#define Key_R1 GPIO_PIN_15					//传感器模块stm32 
#define Key_R2 GPIO_PIN_13					//传感器模块stm32 
#define Key_R3 GPIO_PIN_11  			//传感器模块stm32 
#define Key_R4 GPIO_PIN_9					//传感器模块stm32 

#define Key_C1 GPIO_PIN_7					//传感器模块stm32 
#define Key_C2 GPIO_PIN_5					//传感器模块stm32 
#define Key_C3 GPIO_PIN_3				//传感器模块stm32 
#define Key_C4 GPIO_PIN_1					//传感器模块stm32

#define  jpys 1   //宏定义，约定jpys==20，方便以后程序移植
/* 变量声明------------------------------------------------------------------*/


/* 函数声明 ------------------------------------------------------------------*/
void scn_Keyboard4X4_init(void);
uint8 ReadKeyBorad (uint8 mode);
#endif
