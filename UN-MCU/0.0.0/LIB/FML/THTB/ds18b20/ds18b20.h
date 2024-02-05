/**
  ******************************************************************************
  * @file    传感器模块测试程序
  * @author  dotod
  * @version V1.0
  * @date    10-DEC-2017
  * @brief   Main program body.
  ******************************************************************************
  * @attention	
  *
  ******************************************************************************
  */ 
	
#ifndef __DS18B20_H
#define __DS18B20_H

/* Includes -----------------------------------------------------------------*/

#include "JHAL.h"
/* 宏定义 -------------------------------------------------------------------*/
#define uchar unsigned char
#define uint unsigned int
	
#define DQ_Out 	PEout( 9 )
#define DQ_In		PEin( 9 )
	
/* 变量声明------------------------------------------------------------------*/


/* 函数声明 ------------------------------------------------------------------*/
void init_ds18b20( void );
void check_ds18b20( void );
void init_onewire_out( void );
void init_onewire_in( void );
void ds18b20_write_byte( u8 data ) ;
u8 ds18b20_read_bit( void );
u8 ds18b20_read_byte( void );
#endif

