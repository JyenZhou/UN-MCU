/*************Jyen*************************Jyen********************Jyen***********************Jyen********************
   *                                                                *
 *                                                                   *
*
 		ADC默认通用HAL驱动
  使用ADC0 若增加不同型号单片机 需要在该平台对应.c中增加相关引脚映射
*                                                                     *
  *                                                                 *
   *                                                              *
*********Jyen******************Jyen************************Jyen*****************************************/
#ifndef __JHAL_Delay__H__
#define  __JHAL_Delay__H__

#ifdef __CplusPlus
extern "C" {
#endif

#include "JUTIL.h"

    typedef struct {
//主频
        u8 basicFrequency;

    } JHAL_Delay;



    u32 JHAL_uidGetHigh(void);
    u32 JHAL_uidGetMiddle(void);
    u32 JHAL_uidGetLow(void);
// 96位UID相当于3个32位变量  每8位变量相当于一个字符  所以最大需要4*3+1 =13个字符空间 为了兼容 低位uid先存
    void uid2string(char* string,int buffSize );



    //  JHAL_delayOpen((JHAL_Delay){NULL});
    void  JHAL_delayOpen(JHAL_Delay config);
    void  JHAL_delayMs(u32 ms);
    void  JHAL_delayUs(u32 us);
    void  JHAL_delay(JHAL_TimeUnits timeUntis,u16 value);

    //异常检查 这里主要防止未初始化调用
    void  JHAL_delayTryCheckedExceptions(void);




#ifdef CplusPlus
}
#endif

#endif








