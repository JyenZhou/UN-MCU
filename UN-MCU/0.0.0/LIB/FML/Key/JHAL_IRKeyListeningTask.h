/**
  ****************************JHAL_IRKeyListeningTask.c******************************************
  * @brief     ������λ�����¼��������� ���ж�ģʽ�������жϲ�׼��ͨ����߾Ͳ���װ��
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
//���µ��¼�����Ӧ��ʱ��
    } JHAL_IRKeyEventType;


    typedef struct
    {

        volatile  u64 pressTime ;             // ����ʱ��
        volatile  u16 durationPressTime;  //��������ʱ��
        volatile  u64 lastPressTime ;             // �ϴΰ���ʱ��

        bool active;

        JHAL_IRKeyEventType event;


    } __JHAL_EntLisengKeyLog;






    typedef struct
    {

        JHAL_IO_Port  port ;
        u8  pin;
        //��Ч��ƽ
        bool activeLevel;


        volatile   __JHAL_EntLisengKeyLog __keyLog;

    } JHAL_EntLisengKeyConfig;


    /** ----------------------------JHAL_IRKeyIOListengInit-----------------------------------
      * @��������ʼ����������
      *
      * @������
    **	  	 keyInfoList: �����������������ʽ����ȥ
    **			 keyNumber: [����/��]
      *
      * @����ֵ:��
      * @ע:��
      *-----------------------------Jyen-2023-04-10-------------------------------------- */

    void JHAL_IRKeyIOListengOpen(   JHAL_EntLisengKeyConfig *keyInfoList,u8 keyNumber);





    /** ----------------------------JHAL_IRKeyIOEventListening-----------------------------------
    * @������ �����¼�����  ����JHAL_IRKeyIOEventCallback�ص�
    *���ɨ�轨����ڶ�ʱ�� OS����������

    * @������
    **	  	 port: [����/��]
    **			 pin: [����/��]
    *
    * @����ֵ:��
    * @ע:����ɨ��ʱ����ҪС�ڰ�������ʱ�� �������������¼�ʱ��ƫ�� ����˫��������ⲻ׼��
    *-----------------------------Jyen-2023-04-10-------------------------------------- */

    void JHAL_IRKeyIOEventListening (void);

    /** ----------------------------JHAL_IRKeyIOEventCallback-----------------------------------
      * @�����������¼��ص�����
      *
      * @������
    **	  	 keyIndex: [����/��]
    **			 event: [����/��]
      *
      * @����ֵ:��
      * @ע:���˫���ĵڶ���ʱ������ж�Ϊ˫��
      *-----------------------------Jyen-2023-04-10-------------------------------------- */

    void	JHAL_IRKeyIOEventCallback( u8 keyIndex,JHAL_IRKeyEventType event);


#ifdef CplusPlus
}
#endif

#endif


