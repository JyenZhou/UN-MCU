#ifndef __JUTIL_INCLUDES__H__
#define __JUTIL_INCLUDES__H__
#ifdef __cplusPlus
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



//全局变量时候可以不用初始化这个  局部变量/默认是打开的必须要适配  (内部尽量实现默认关闭)
#define  JHAL_DEV_INIT         .__info.isOpen=false
#define  JHAL_DEV_DEINIT         .__info.isOpen=true




 #define UpdateDateTime void JHAL_getDateTimeString( char dateStr[12], char timeStr[9]){strncpy(dateStr, __DATE__, 12 - 1);dateStr[12 - 1] = '\0';strncpy(timeStr, __TIME__, 9 - 1); timeStr[9 - 1] = '\0';}
 void JHAL_getDateTimeString( char dateStr[12], char timeStr[9]);



    void JHAL_systemReset(void);
    void JHAL_error(char  * msg);
    void  JHAL_enableInterrupts(void);
    void  JHAL_disableInterrupts(void);
    extern volatile u64 jsystemMs;


#ifdef __cplusplus
}
#endif

#endif




