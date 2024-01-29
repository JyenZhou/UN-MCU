#ifndef __IS31FL3733__H__
#define __IS31FL3733__H__
#include "JHAL.h"
#ifdef __CplusPlus
extern "C" {
#endif

// 101 00 00 X
#define DeviceAddress 0x50
#define Write 1
#define Rade 1

//配置命令 寄存器
#define ConfigureCommandRegister 0xFD
//命令寄存器写锁
#define CommandRegisterWriteLock  0xFE
//中断屏蔽寄存器
#define InterruptMaskRegister  0xF0
//中断状态寄存器
#define InterruptStatusRegister   0xF1


//LED控制 寄存器 
#define LEDCOntrolRegister 0x00
//PWM 寄存器
#define PWMRegister 0x01
//自动呼吸模式 寄存器
#define AutoBreathModeRegister 0x02
//功能 寄存器
#define FunctionRegister 0x03



typedef enum
{
	FirstWrite=0xFF,
	Nthof_Write=0xFE,
	Last_Write=0xEE,
	Once_Write=0xEF
}WhichoneWrite_TypeDefE;
//用于确定是第几次写入数据    第一次写入该类数据  第一次要拉起IIC和写地址  后面便能直接写数据
//Nthof_Write 代表还要写
//Once_Write 代表只写一次
//最后一次则Stop

typedef enum
{
	ON=0x01,
	OFF=0x00
}ONOrOFF_TypeDefE;
//用于独立控制的开关

typedef enum
{
	Left,
	Reight
}Direction_TypeDefE;
//用于行半边方向控制位置控制的枚举 


void IS31FL3733_Testmain (void);
#ifdef CplusPlus
}
#endif

#endif


