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


#ifdef  USE_STDPERIPH_DRIVER
#include "HAL/MM32/Manufacturer/hal_conf.h"
#include "HAL/MM32/Manufacturer/mm32_device.h"


 
#endif

//全局变量时候可以不用初始化这个  局部变量/默认是打开的必须要  内部实现时尽量默认关闭
#define  JHAL_DEV_INIT         .__info.isOpen=false
#define  JHAL_DEV_DEINIT         .__info.isOpen=true


 void JHAL_getDateTimeString( char dateStr[12], char timeStr[9]);

 #define UpdateDateTime void JHAL_getDateTimeString( char dateStr[12], char timeStr[9]){strncpy(dateStr, __DATE__, 12 - 1);dateStr[12 - 1] = '\0';strncpy(timeStr, __TIME__, 9 - 1); timeStr[9 - 1] = '\0';}
 

    u32 JHAL_uidGetHigh(void);
    u32 JHAL_uidGetMiddle(void);
    u32 JHAL_uidGetLow(void);
// 96位UID相当于3个32位变量  每8位变量相当于一个字符  所以最大需要4*3+1 =13个字符空间 为了兼容 低位uid先存
    void uid2string(char* string,int buffSize );

    void JHAL_systemReset(void);


   void JHAL_Fault_Handler(char  * msg);

    void  JHAL_enableInterrupts(void);
    void  JHAL_disableInterrupts(void);
    void JHAL_getSFVersions( uint8_t *date);
    extern volatile u64 jsystemMs;


#ifdef __cplusplus
}
#endif

#endif




