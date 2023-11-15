/**
  ****************************JHAL_BootLoader.h******************************************
  * @brief

	ʹ��˵����
	   APP��



		 1. APP��ʱʹ�����´������boot��app��Ϣͬ��  ��ֹ��ʱ����˲�����boot��ͬ�� ����������Ҫ��������boot

void bootInfoSync()
{
    JHAL_BootInfo bootInfo;
    JHAL_bootGetInfo(&bootInfo);
    if(bootInfo.canBaudRate!=flashInfo. canBaudRate ||bootInfo.bootCanId!=flashInfo.canSendID ||bootInfo.pcCanId!=PC_CAN_ID||bootInfo.canIsExtendFrame!=flashInfo.canIsExtendFrame)
    {
        bootInfo.canBaudRate=flashInfo. canBaudRate ;
        bootInfo.bootCanId=flashInfo.canSendID ;
        bootInfo.pcCanId=PC_CAN_ID;
        bootInfo.canIsExtendFrame=flashInfo.canIsExtendFrame;
        JHAL_bootSetInfo(bootInfo);
    }

}
  2.����/CAN�����н�����λ�����������ֵ ����   JHAL_bootParseUpdateInfo4App( u8 *data,u8 productID,JHAL_BootStatus bootStatus) ;�Զ�������������  ע�����ο��Ź�
		 Boot��
		  1.    JHAL_bootLoader(JHAL_BootInfo info);  ���ú�boot��Ϣ���뼴��

  *
    ******************************************************************************
  * @file     JHAL_BootLoader.h
  * @author   Jyen
  * @version  v1.0
  * @date     2023-09-15
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



#ifndef __JHAL_BOOTOLADER__H__
#define __JHAL_BOOTOLADER__H__
#include "JHAL.h"
#ifdef __CplusPlus
extern "C" {
#endif
#include "HAL\JHAL_CAN.h"
#include "HAL\JHAL_Uart.h"

//flash��ʼ��ַ ���δ������Ҫ��flash�ж���
#define JHAL_BOOT_BootStartAddr   FlashStartAddr

#if defined  XL6600A402L6
// 16K
#define JHAL_BOOT_BootMaxSize   0x4000

#elif defined STM32F103xE


//16K

#define JHAL_BOOT_BootMaxSize   0x4000
#else
// δ���ⶨ��õ�Ƭ���� ʹ��Ԥ���С
#define JHAL_BOOT_BootMaxSize   0x4000
#endif



//App��ʼ��ַ
#define JHAL_BOOT_AppStartAddr   (JHAL_BOOT_BootStartAddr+JHAL_BOOT_BootMaxSize)
//boot��Ϣռ�õ�flashҳ��ַ
#define JHAL_BOOT_FlashInfoPage   1


    typedef enum
    {

        BootStatus_Run=0,
//ͨ��CAN����
        BootStatus_Update4CAN,
//ͨ����������
        BootStatus_Update4Uart,

    } JHAL_BootStatus;





    typedef struct  __attribute__((packed))
    {

        u16 productID  : 15;
        u16 isApp : 1;   //  1λ
        u16 year : 7;   // ��ݵĺ���λ�����ֵΪ99��ռ��7λ
        u16 month : 4;  // �·ݣ����ֵΪ12��ռ��4λ
        u16 day : 5;    // �գ����ֵΪ31��ռ��5λ
        u8 hour : 6;   // ʱ�����ֵΪ23��ռ��6λ



    }
    __JHAL_VersionsInfo;



    //��������ע�� ����ָ������ ����Ҫע������
    typedef struct
    {
        //����boot��ʼ״̬
        JHAL_BootStatus bootStatus;
        u16	productID:15;
        u8  canDev :3;
        u8  uartDev:3;
        bool canIsExtendFrame:1;
        JHAL_CANBaudRate canBaudRate;

        //�����������λ��ID
        u32  pcCanId;
        u16  bootCanId;

        u32  uartBaudRate;


        __JHAL_VersionsInfo __info;

    } JHAL_BootInfo;







    //boot���ж���������ת���������
    void JHAL_bootLoader(JHAL_BootInfo info);

    //����app�޸�ͬ��boot��������Ϣ ����App��̬������
    bool JHAL_bootGetInfo( JHAL_BootInfo *bootFlashInfo);
    bool JHAL_bootSetInfo(JHAL_BootInfo info);

    /** ----------------------------JHAL_bootParseUpdateInfo4App-----------------------------------
    * @��������APP���ã�����Զ�����������ж��Ƿ�Ҫ��������������APP����Boot
    *
    * @������
    **	  	 data: [���� ] Զ��������Ϣ 8���ֽ� Ҫ�ͱ���һ��
    **			 productID: [���� ] ��ƷID
    **			 bootStatus: [���� ] ��Ҫ���ַ�ʽ���� can uart
    *
    * @����ֵ:��
    * @ע: ���APP��ʹ���˿��Ź�����bootʱ����Ҫ�ر�
    *-----------------------------Jyen-2023-09-14-------------------------------------- */

    void JHAL_bootParseUpdateInfo4App( u8 *data,u8 productID,JHAL_BootStatus bootStatus) ;





    //��תĿ���ַ����  һ�����ڶ�APP����һ��  �����ڲ�ֱ����ת
    void JHAL_bootJump2App(uint32_t appAddr);




#define BOOT_DataHead 0xA5

#ifdef CplusPlus
}
#endif

#endif








