/**
  ****************************JHAL_IRKeyListeningTask.c******************************************
  * @brief     ������λ�����¼���������
	 ǰ��ʵ�֣�
	 1.GPIO�ж�(����ʵ����Ч��ƽ�����Ӧ�ж� ���ж�ģʽ�������жϲ�׼��ͨ����߾Ͳ���װ��)
	 2.jSystemMs��ʱ
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
  * @��������ʼ����������
  *
  * @������
**	  	 keyInfoList: [����/��]
**			 keyNumber: [����/��]
  *
  * @����ֵ:��
  * @ע:��
  *-----------------------------Jyen-2023-04-10-------------------------------------- */

void JHAL_IRKeyIOListengOpen(   JHAL_EntLisengKeyConfig *keyInfoList,u8 keyNumber)
{
    __JHAL_IRKeyInfoList=keyInfoList;

    __JHAL_IRKeyNumber=keyNumber;
    for(u8 i=0; i<__JHAL_IRKeyNumber; i++)
    { 
					//��ʼ��
				  __JHAL_IRKeyInfoList[i].__keyLog.active=false;
			  
			JHAL_gpioModeSet(__JHAL_IRKeyInfoList[i].port,__JHAL_IRKeyInfoList[i].pin, __JHAL_IRKeyInfoList[i].activeLevel? JHAL_IOMODE_Preset_Exit_High_Rising:JHAL_IOMODE_Preset_Exit_Low_Falling);
				 
			 
    }
}
 

/** ----------------------------JHAL_IRKeyIOChangeListening4Interrupt-----------------------------------
  * @�����������仯�жϼ���  �ú������Զ������жϻص���
  *
  * @������
**	  	 port: [����/��]
**			 pin: [����/��]
  *
  * @����ֵ:��
  * @ע:��
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
				//���¼�
			if(!__JHAL_IRKeyInfoList[i].__keyLog.active)
			{
				 continue;
			}
			
			
			bool isRelease=true;
			 	//������ǰ�ס�� ���°���ʱ��
			 if(	JHAL_gpioReadPin(__JHAL_IRKeyInfoList[i].port,__JHAL_IRKeyInfoList[i].pin)==__JHAL_IRKeyInfoList[i].activeLevel)
        {
					   __JHAL_IRKeyInfoList[i].__keyLog. durationPressTime=jsystemMs-  __JHAL_IRKeyInfoList[i].__keyLog.pressTime;
					isRelease=false;
					
				} 			
		//���ΰ���ʱ������жϳ�����
			if(  __JHAL_IRKeyInfoList[i].__keyLog.pressTime-__JHAL_IRKeyInfoList[i].__keyLog.lastPressTime<JHAL_IRKeyEvent_DoubleClick )
			{		 
				event= JHAL_IRKeyEvent_DoubleClick;
				
			}else	if( __JHAL_IRKeyInfoList[i].__keyLog.durationPressTime>JHAL_IRKeyEvent_DoubleClick)
			{
				
					event= JHAL_IRKeyEvent_LongPress;
				
			}
			//���ʱ������İ���������
			else if(isRelease&&jsystemMs- __JHAL_IRKeyInfoList[i].__keyLog.pressTime>JHAL_IRKeyEvent_DoubleClick){
				event =JHAL_IRKeyEvent_Click;
			}
			
			//������������Ӧ ����˫���ж�ʱ��
			else{
				 continue;
			}
		//����¼�
			     __JHAL_IRKeyInfoList[i].__keyLog.active=false;
		
		
				
				
 JHAL_IRKeyIOEventCallBack(i,event);
		}
		
		
}

OS_WEAK  void	JHAL_IRKeyIOEventCallBack( u8 keyIndex,JHAL_IRKeyEventType event)
	{
		
		
	}
 

void  JHAL_gpio4IRKeyInterruptCallback()

{
    __JHAL_IRKeyIOChangeListening4Interrupt();
}



 



