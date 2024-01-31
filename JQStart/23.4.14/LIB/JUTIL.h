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
#include "Util/FontLib/jfontLib.h"

#define JSMN_STATIC
#include "Util/jsmn/jsmn.h"



//ȫ�ֱ���ʱ����Բ��ó�ʼ�����  �ֲ�����/Ĭ���Ǵ򿪵ı���Ҫ����  (�ڲ�����ʵ��Ĭ�Ϲر�)
#define  JHAL_DEV_INIT         .__info.isOpen=false
#define  JHAL_DEV_DEINIT         .__info.isOpen=true


 void JHAL_getDateTimeString( char dateStr[12], char timeStr[9]);

 #define UpdateDateTime void JHAL_getDateTimeString( char dateStr[12], char timeStr[9]){strncpy(dateStr, __DATE__, 12 - 1);dateStr[12 - 1] = '\0';strncpy(timeStr, __TIME__, 9 - 1); timeStr[9 - 1] = '\0';}
 

    u32 JHAL_uidGetHigh(void);
    u32 JHAL_uidGetMiddle(void);
    u32 JHAL_uidGetLow(void);
// 96λUID�൱��3��32λ����  ÿ8λ�����൱��һ���ַ�  ���������Ҫ4*3+1 =13���ַ��ռ� Ϊ�˼��� ��λuid�ȴ�
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




