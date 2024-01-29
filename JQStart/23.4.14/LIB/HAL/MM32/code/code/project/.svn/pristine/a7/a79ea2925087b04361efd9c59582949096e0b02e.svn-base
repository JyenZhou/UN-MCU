#ifndef __SYS_H
#define __SYS_H 

#include <string.h>
#include <stdio.h>
#include "stdlib.h"
#include "math.h"
#include "mm32_device.h"
#include "hal_conf.h"

#define USART_RX_LEN 	100
#define USART_TX_LEN	150

#include "delay.h"
//#include "exint.h"
#include "wwdg.h"
#include "led.h"
#include "bsp_tim.h"
#include "bsp_io.h"
#include "sim_eeprom.h"
#include "adc.h"
#include "ldo.h"
#include "uart.h"

#include "module.h"

#define MAXTASKS 	12

extern	volatile uint32_t timers[MAXTASKS];

extern	volatile uint32_t UERROR;

#define _SS static unsigned char _lc=0; switch(_lc){default: 
#define _EE ;}; _lc=0; return 0xFFFFFFFF;

#define WaitX(tickets)  do { _lc=(__LINE__%255)+1; return (tickets) ;case (__LINE__%255)+1:;} while(0);
#define WaitUntil(A)    do { while(!(A)) WaitX(1);} while(0);

#define RunTask(TaskName,TaskID)  do { if (timers[TaskID]==0) { uint32_t d=TaskName(); while(timers[TaskID]!=d) timers[TaskID]=d;} }  while(0); 
#define RunTaskA(TaskName,TaskID) do { if (timers[TaskID]==0) {uint32_t d=TaskName(); while(timers[TaskID]!=d) timers[TaskID]=d;   continue;} }while(0);   //前面的任务优先保证执行

#define CallSub(SubTaskName) do {uint32_t currdt; _lc=(__LINE__%255)+1; return 0; case (__LINE__%255)+1:  currdt=SubTaskName(); if(currdt!=0xFFFFFFFF) return currdt;} while(0);
#define InitTasks() do {unsigned char i; for(i=MAXTASKS;i>0 ;i--) timers[i-1]=0; } while(0);
#define UpdateTimers() do{unsigned char i; for(i=MAXTASKS;i>0 ;i--){if((timers[i-1]!=0)&&(timers[i-1]!=0xFFFFFFFF)) timers[i-1]--;}} while(0);

#define SEM unsigned int 
//初始化信号量
#define InitSem(sem) do{sem=0;}while(0);
//等待信号量
#define WaitSem(sem) do{ sem=1; WaitX(0); if (sem>0) return 1;} while(0);
//发送信号量
#define SendSem(sem)  do {sem=0;} while(0);

#define SuspendTask return 0xFFFFFFFF;

#define ExitTask	_lc=0; return 0xFFFFFFFF;

#define SoftReset	__set_FAULTMASK(1);NVIC_SystemReset()

#endif
