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
//���������core_cm3  �ں˰汾������Ҫ��ͨ�����bootload����ʱ����תָ���ж�
#include "stm32f103xe.h"
 
 #elif defined STM32F103xB 
 
// #include "stm32f101xb.h"
 

#endif


#endif
//ȫ�ֱ���ʱ����Բ��ó�ʼ�����  �ֲ������Է���һ����Ҫ
#define  JHAL_DEV_INIT         .__info.isOpen=false




    u32 JHAL_uidGetHigh(void);
    u32 JHAL_uidGetMiddle(void);
    u32 JHAL_uidGetLow(void);
// 96λUID�൱��3��32λ����  ÿ8λ�����൱��һ���ַ�  ���������Ҫ4*3+1 =13���ַ��ռ� Ϊ�˼��� ��λuid�ȴ�
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




