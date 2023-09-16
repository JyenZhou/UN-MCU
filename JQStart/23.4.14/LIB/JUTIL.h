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


#ifdef   STM32F103xE
//���������core_cm3  �ں˰汾������Ҫ��
  #include "stm32f103xe.h"  
#endif
#endif




void JHAL_systemReset(void);
u32 JHAL_uidGetHigh(void);
u32 JHAL_uidGetMiddle(void);
u32 JHAL_uidGetLow(void);
extern char  UIDString[];

void  JHAL_enableInterrupts(void);
void  JHAL_disableInterrupts(void);
void JHAL_getSFVersions( uint8_t *date);
extern volatile u64 jsystemMs;


#ifdef __cplusplus
}
#endif

#endif




