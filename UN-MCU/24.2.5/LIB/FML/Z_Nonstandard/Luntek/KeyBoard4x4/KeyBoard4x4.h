/**
  ******************************************************************************
  * @file    ������ģ����Գ���
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

/* �궨�� -------------------------------------------------------------------*/
#define GPIOX GPIOB

	
#define Key_R1 GPIO_PIN_15					//������ģ��stm32 
#define Key_R2 GPIO_PIN_13					//������ģ��stm32 
#define Key_R3 GPIO_PIN_11  			//������ģ��stm32 
#define Key_R4 GPIO_PIN_9					//������ģ��stm32 

#define Key_C1 GPIO_PIN_7					//������ģ��stm32 
#define Key_C2 GPIO_PIN_5					//������ģ��stm32 
#define Key_C3 GPIO_PIN_3				//������ģ��stm32 
#define Key_C4 GPIO_PIN_1					//������ģ��stm32

#define  jpys 1   //�궨�壬Լ��jpys==20�������Ժ������ֲ
/* ��������------------------------------------------------------------------*/


/* �������� ------------------------------------------------------------------*/
void scn_Keyboard4X4_init(void);
uint8 ReadKeyBorad (uint8 mode);
#endif
