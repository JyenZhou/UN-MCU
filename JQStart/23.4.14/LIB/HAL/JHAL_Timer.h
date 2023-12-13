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



    typedef struct {
        bool isOpen;

    } __JHAL_Timer_therInfo;


    typedef struct {

        u8 dev;
//中断的时间单位
        JHAL_TimeUnits itTimeUnit;
//itTimeValue 中断时间值 0-1000
        u16 itTimeValue;
        __JHAL_Timer_therInfo __info;
			//			如果内部使用了这个变量一定要先赋值
        void*  timer;
    } JHAL_Timer;



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

    bool JHAL_timerOpen(JHAL_Timer *timer );
    bool  JHAL_timerDeInit(JHAL_Timer *timer);
//中断回调  注意这里对应的是uart实例 id 不是传进去的dev  因为两个dev可能实际上是一个uart 只是引脚不一样
    void JHAL_timerInterruptCallBack( u8 realityId);

#ifdef CplusPlus
}
#endif

#endif








