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
#include "Util/JHAL_JSON.h"

#define JSMN_STATIC
#include "Util/jsmn/jsmn.h"


#ifdef  USE_HAL_DRIVER

#include "main.h"


#if    defined STM32F103xE 
//���������core_cm3  �ں˰汾������Ҫ��ͨ�����bootload����ʱ����תָ���ж�
#include "stm32f103xe.h"
 
 #elif defined STM32F103xB 
 
// #include "stm32f101xb.h"
 

#endif


#endif
//ȫ�ֱ���ʱ����Բ��ó�ʼ�����  �ֲ������Է���һ����Ҫ
#define  JHAL_DEV_INIT         .__info.isOpen=false



    void JHAL_systemReset(void);



    void  JHAL_enableInterrupts(void);
    void  JHAL_disableInterrupts(void);
    void JHAL_getSFVersions( uint8_t *date);
    extern volatile u64 jsystemMs;


#ifdef __cplusplus
}
#endif

#endif




