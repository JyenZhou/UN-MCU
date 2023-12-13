#ifndef __JUTIL_INCLUDES__H__
#define __JUTIL_INCLUDES__H__
#ifdef __CplusPlus
extern "C" {
#endif

#include "JSYS.h"

#include "Util/JHAL_CRC.h"
#include "Util/JHAL_Math.h"
#include "Util/JHAL_NumberConverter.h"
#include "Util/zdmalloc.h"
 

#define JSMN_STATIC
#include "Util/jsmn/jsmn.h"


#ifdef  USE_HAL_DRIVER

#include "main.h"


#if    defined STM32F103xE 
//里面包含了core_cm3  内核版本是我需要的通过这个bootload升级时候跳转指令判断
#include "stm32f103xe.h"
 
 #elif defined STM32F103xB 
 
// #include "stm32f101xb.h"
 

#endif


#endif
//全局变量时候可以不用初始化这个  局部变量以防万一必须要
#define  JHAL_DEV_INIT         .__info.isOpen=false




    u32 JHAL_uidGetHigh(void);
    u32 JHAL_uidGetMiddle(void);
    u32 JHAL_uidGetLow(void);
// 96位UID相当于3个32位变量  每8位变量相当于一个字符  所以最大需要4*3+1 =13个字符空间 为了兼容 低位uid先存
    void uid2string(char* string,int buffSize );

    void JHAL_systemReset(void);


   void JHAL_Fault_Handler(void);

    void  JHAL_enableInterrupts(void);
    void  JHAL_disableInterrupts(void);
    void JHAL_getSFVersions( uint8_t *date);
    extern volatile u64 jsystemMs;


#ifdef __cplusplus
}
#endif

#endif




