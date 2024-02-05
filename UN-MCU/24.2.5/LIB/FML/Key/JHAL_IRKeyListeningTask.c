/**
  ****************************JHAL_IRKeyListeningTask.c******************************************
  * @brief     独立复位按键事件监听任务
	 前提实现：
	 1.GPIO中断(必须实现有效电平的相对应中断 非中断模式对连击判断不准不通用这边就不封装了)
	 2.jSystemMs计时
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




#include "JHAL_IRKeyListeningTask.h"
#ifdef RTOS_EN
#include <rtthread.h>
#endif

volatile JHAL_EntLisengKeyConfig *__JHAL_IRKeyInfoList;
u8 __JHAL_IRKeyNumber=0;

/** ----------------------------JHAL_IRKeyIOListengInit-----------------------------------
  * @描述：初始化按键参数
  *
  * @参数：
**	  	 keyInfoList: [输入/出]
**			 keyNumber: [输入/出]
  *
  * @返回值:无
  * @注:无
  *-----------------------------Jyen-2023-04-10-------------------------------------- */

void JHAL_IRKeyIOListengOpen(   JHAL_EntLisengKeyConfig *keyInfoList,u8 keyNumber)
{
    __JHAL_IRKeyInfoList=keyInfoList;

    __JHAL_IRKeyNumber=keyNumber;
    for(u8 i=0; i<__JHAL_IRKeyNumber; i++)
    { 
					//初始化
				  __JHAL_IRKeyInfoList[i].__keyLog.active=false;
			  
			JHAL_gpioModeSet(__JHAL_IRKeyInfoList[i].port,__JHAL_IRKeyInfoList[i].pin, __JHAL_IRKeyInfoList[i].activeLevel? JHAL_IOMODE_Preset_Exit_High_Rising:JHAL_IOMODE_Preset_Exit_Low_Falling);
				 
			 
    }
}
 

/** ----------------------------JHAL_IRKeyIOChangeListening4Interrupt-----------------------------------
  * @描述：按键变化中断监听  该函数已自动放在中断回调中
  *
  * @参数：
**	  	 port: [输入/出]
**			 pin: [输入/出]
  *
  * @返回值:无
  * @注:无
  *-----------------------------Jyen-2023-04-10-------------------------------------- */

void __JHAL_IRKeyIOChangeListening4Interrupt ()
{
    for(u8 i=0; i<__JHAL_IRKeyNumber; i++)
    {

        if(	JHAL_gpioReadPin(__JHAL_IRKeyInfoList[i].port,__JHAL_IRKeyInfoList[i].pin)==__JHAL_IRKeyInfoList[i].activeLevel)
        {
						__JHAL_IRKeyInfoList[i].__keyLog.lastPressTime =   __JHAL_IRKeyInfoList[i].__keyLog.pressTime;						
            __JHAL_IRKeyInfoList[i].__keyLog.pressTime=jsystemMs;
					  __JHAL_IRKeyInfoList[i].__keyLog.durationPressTime=0;
            __JHAL_IRKeyInfoList[i].__keyLog.active=true;

        } 

    }

}

void JHAL_IRKeyIOEventListening()
{

	JHAL_IRKeyEventType event;
    for(u8 i=0; i<__JHAL_IRKeyNumber; i++)
    {
				//无事件
			if(!__JHAL_IRKeyInfoList[i].__keyLog.active)
			{
				 continue;
			}
			
			
			bool isRelease=true;
			 	//如果还是按住的 更新按下时长
			 if(	JHAL_gpioReadPin(__JHAL_IRKeyInfoList[i].port,__JHAL_IRKeyInfoList[i].pin)==__JHAL_IRKeyInfoList[i].activeLevel)
        {
					   __JHAL_IRKeyInfoList[i].__keyLog. durationPressTime=jsystemMs-  __JHAL_IRKeyInfoList[i].__keyLog.pressTime;
					isRelease=false;
					
				} 			
		//两次按下时间符合判断成连击
			if(  __JHAL_IRKeyInfoList[i].__keyLog.pressTime-__JHAL_IRKeyInfoList[i].__keyLog.lastPressTime<JHAL_IRKeyEvent_DoubleClick )
			{		 
				event= JHAL_IRKeyEvent_DoubleClick;
				
			}else	if( __JHAL_IRKeyInfoList[i].__keyLog.durationPressTime>JHAL_IRKeyEvent_DoubleClick)
			{
				
					event= JHAL_IRKeyEvent_LongPress;
				
			}
			//间隔时间过长的按单击处理
			else if(isRelease&&jsystemMs- __JHAL_IRKeyInfoList[i].__keyLog.pressTime>JHAL_IRKeyEvent_DoubleClick){
				event =JHAL_IRKeyEvent_Click;
			}
			
			//单击不立即响应 留给双击判断时间
			else{
				 continue;
			}
		//清楚事件
			     __JHAL_IRKeyInfoList[i].__keyLog.active=false;
		
		
				
				
 JHAL_IRKeyIOEventCallback(i,event);
		}
		
		
}

OS_WEAK  void	JHAL_IRKeyIOEventCallback( u8 keyIndex,JHAL_IRKeyEventType event)
	{
		
		
	}
 

void  JHAL_gpio4IRKeyInterruptCallback()

{
    __JHAL_IRKeyIOChangeListening4Interrupt();
}



 



