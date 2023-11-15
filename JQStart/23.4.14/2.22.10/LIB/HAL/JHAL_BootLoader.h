/*************Jyen*************************Jyen********************Jyen***********************Jyen********************
   *                                                                *
 *                                                                   *
*
 		CANĬ��ͨ��HAL����
  ʹ��CAN0 ֡���� Ĭ������֡��Զ��֡         �����Ӳ�ͬ�ͺŵ�Ƭ�� ��Ҫ�ڸ�ƽ̨��Ӧ.c�������������ӳ��
*                                                                     *
  *                                                                 *
   *                                                              *
*********Jyen******************Jyen************************Jyen*****************************************/


#ifndef __JHAL_BOOTOLADER__H__
#define __JHAL_BOOTOLADER__H__
#include "JUTIL.h"
#ifdef __CplusPlus
extern "C" {
#endif
#include "HAL\JHAL_CAN.h"

#if defined  XL6600A402L6
//boot��ʼ��ַ��0
#define JHAL_BOOT_BootStartAddr   0x0
//bootС��8K
#define JHAL_BOOT_BootMaxSize   0x2000

#else
//Ϊ�˷�ֹ��ʹ�øù���ʱ����ʹ��0  ����ʱ�������������
#define JHAL_BOOT_BootStartAddr   0x0
#define JHAL_BOOT_BootMaxSize   0x00
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

    typedef enum
    {
        JHAL_BootCom_Uart=1,
        JHAL_BootCom_CAN=2,

    } JHAL_BootCom;


    //��������ע�� ����ָ������ ����Ҫע������
    typedef struct
    {
        //����boot��ʼ״̬
        JHAL_BootStatus bootStatus;
        JHAL_CANBaudRate canBaudRate;
        JHAL_CAN  can;
        bool canIsExtendFrame;
        //�����������λ��ID
        u32  pcCanId;
        u16  bootCanId;

    } JHAL_BootInfo;


    //app�е�����״̬ΪBootStatus_Update ����������Boot��ͨ��״̬�ȴ�����������ǰһ��Ҫ���÷��� �����������״̬�����Ҫ���������
    void JHAL_bootStatusSet(JHAL_BootStatus status);
    //����app�޸�boot��������Ϣ
    bool JHAL_bootGetInfo( JHAL_BootInfo *bootFlashInfo);
    bool JHAL_bootSetInfo(JHAL_BootInfo info);
    //boot���ж���������ת���������
    void JHAL_bootLoader(JHAL_BootInfo info);



    //��תĿ���ַ����  һ�����ڶ�APP����һ��  ֱ����ת
    void JHAL_bootJump2App(uint32_t appAddr);
    //ͨ��can��������  isUpdateApp true����App false����boot
    void  JHAL_bootUpdating4Can(JHAL_BootInfo bootInfo,bool isUpdateApp);

#ifdef CplusPlus
}
#endif

#endif







