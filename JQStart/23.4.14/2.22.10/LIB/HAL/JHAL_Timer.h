/*************Jyen*************************Jyen********************Jyen***********************Jyen********************
   *                                                                *
 *                                                                   *
*
 		CAN默认通用HAL驱动
  使用CAN0 帧类型 默认数据帧非远程帧         若增加不同型号单片机 需要在该平台对应.c中增加相关引脚映射
*                                                                     *
  *                                                                 *
   *                                                              *
*********Jyen******************Jyen************************Jyen*****************************************/


#ifndef __JHAL_Timer__H__
#define __JHAL_Timer__H__
#include "JUTIL.h"
#ifdef __CplusPlus
extern "C" {
#endif


    typedef enum
    {
        JHAL_Timer0=0,
        JHAL_Timer1,
        JHAL_Timer_Number
    } JHAL_Timer;




    typedef struct {
//中断的时间单位
        JHAL_TimeUnits itTimeUnit;
//itTimeValue 中断时间值 0-1000
        u16 itTimeValue;

    } JHAL_TimerConfig;



    /* ----------------------------JHAL_timerInit----------------------------------- */
    /**
      * 描述:
      *
      *
      *
      * 参数:
    **		  	 timer: [输入/出]
    **			 config: [输入/出]
      *
      * 返回值:无
      *
     **/
    /* -----------------------------Jyen-2022-11-07-------------------------------------- */

    void JHAL_timerInit(JHAL_Timer timer,JHAL_TimerConfig config);
    void  JHAL_timerDeInit(JHAL_Timer timer);
//中断回调
    void JHAL_timerInterruptCallBack(JHAL_Timer timer);

#ifdef CplusPlus
}
#endif

#endif








