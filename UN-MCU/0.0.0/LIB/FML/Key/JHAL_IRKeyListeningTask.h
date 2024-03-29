/**
  ****************************JHAL_IRKeyListeningTask.c******************************************
  * @brief     独立复位按键事件监听任务 非中断模式对连击判断不准不通用这边就不封装了
  *
    ******************************************************************************
  * @file     JHAL_IRKeyListeningTask.c
  * @author   Jyen
  * @version  v1.0
  * @date     2023-04-11
  * @attention
  * This software is supplied under the terms of a license
  * agreement or non-disclosure agreement.
  * Passing on and copying of this document,and communication
  * of its contents is not permitted without prior written
  * authorization.
  *
  *         (C) Copyright 2023,Jyen,China. All Rights Reserved
  ********************************************************************************
  */

#ifndef __JHAL_IRKEYLISTINGTASK__H__
#define  __JHAL_IRKEYLISTINGTASK__H__
#include "JHAL.h"
#ifdef __cplusPlus
extern "C" {
#endif

    typedef enum
    {

        JHAL_IRKeyEvent_Click=30,
        JHAL_IRKeyEvent_DoubleClick =300,
        JHAL_IRKeyEvent_LongPress=1000,
//按下的事件及对应的时间
    } JHAL_IRKeyEventType;


    typedef struct
    {

        volatile  u64 pressTime ;             // 按下时间
        volatile  u16 durationPressTime;  //持续按下时间
        volatile  u64 lastPressTime ;             // 上次按下时间

        bool active;

        JHAL_IRKeyEventType event;


    } __JHAL_EntLisengKeyLog;






    typedef struct
    {

        JHAL_IO_Port  port ;
        u8  pin;
        //有效电平
        bool activeLevel;


        volatile   __JHAL_EntLisengKeyLog __keyLog;

    } JHAL_EntLisengKeyConfig;


    /** ----------------------------JHAL_IRKeyIOListengInit-----------------------------------
      * @描述：初始化按键参数
      *
      * @参数：
    **	  	 keyInfoList: 多个按键则以数组形式传进去
    **			 keyNumber: [输入/出]
      *
      * @返回值:无
      * @注:无
      *-----------------------------Jyen-2023-04-10-------------------------------------- */

    void JHAL_IRKeyIOListengOpen(   JHAL_EntLisengKeyConfig *keyInfoList,u8 keyNumber);





    /** ----------------------------JHAL_IRKeyIOEventListening-----------------------------------
    * @描述： 按键事件监听  触发JHAL_IRKeyIOEventCallback回调
    *裸机扫描建议放在定时器 OS放在任务中

    * @参数：
    **	  	 port: [输入/出]
    **			 pin: [输入/出]
    *
    * @返回值:无
    * @注:任务扫描时间需要小于按键单击时间 否则会造成整个事件时间偏移 导致双击单击检测不准等
    *-----------------------------Jyen-2023-04-10-------------------------------------- */

    void JHAL_IRKeyIOEventListening (void);

    /** ----------------------------JHAL_IRKeyIOEventCallback-----------------------------------
      * @描述：按键事件回调函数
      *
      * @参数：
    **	  	 keyIndex: [输入/出]
    **			 event: [输入/出]
      *
      * @返回值:无
      * @注:如果双击的第二次时间过长判断为双击
      *-----------------------------Jyen-2023-04-10-------------------------------------- */

    void	JHAL_IRKeyIOEventCallback( u8 keyIndex,JHAL_IRKeyEventType event);


#ifdef CplusPlus
}
#endif

#endif


