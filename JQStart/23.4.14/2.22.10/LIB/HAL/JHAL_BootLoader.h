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


#ifndef __JHAL_BOOTOLADER__H__
#define __JHAL_BOOTOLADER__H__
#include "JUTIL.h"
#ifdef __CplusPlus
extern "C" {
#endif
#include "HAL\JHAL_CAN.h"

#if defined  XL6600A402L6
//boot起始地址是0
#define JHAL_BOOT_BootStartAddr   0x0
//boot小于8K
#define JHAL_BOOT_BootMaxSize   0x2000

#else
//为了防止不使用该功能时报错使用0  运行时会产生错误提醒
#define JHAL_BOOT_BootStartAddr   0x0
#define JHAL_BOOT_BootMaxSize   0x00
#endif




//App起始地址
#define JHAL_BOOT_AppStartAddr   (JHAL_BOOT_BootStartAddr+JHAL_BOOT_BootMaxSize)

//boot信息占用的flash页地址
#define JHAL_BOOT_FlashInfoPage   1


    typedef enum
    {

        BootStatus_Run=0,
//通过CAN升级
        BootStatus_Update4CAN,
//通过串口升级
        BootStatus_Update4Uart,

    } JHAL_BootStatus;

    typedef enum
    {
        JHAL_BootCom_Uart=1,
        JHAL_BootCom_CAN=2,

    } JHAL_BootCom;


    //后面增加注意 不能指针类型 数组要注明长度
    typedef struct
    {
        //设置boot初始状态
        JHAL_BootStatus bootStatus;
        JHAL_CANBaudRate canBaudRate;
        JHAL_CAN  can;
        bool canIsExtendFrame;
        //发送命令的上位机ID
        u32  pcCanId;
        u16  bootCanId;

    } JHAL_BootInfo;


    //app中调设置状态为BootStatus_Update 会软重启到Boot中通过状态等待升级（调用前一定要做好防误触 否则进入升级状态后必须要完成升级）
    void JHAL_bootStatusSet(JHAL_BootStatus status);
    //用于app修改boot的配置信息
    bool JHAL_bootGetInfo( JHAL_BootInfo *bootFlashInfo);
    bool JHAL_bootSetInfo(JHAL_BootInfo info);
    //boot中判断升级及跳转主程序操作
    void JHAL_bootLoader(JHAL_BootInfo info);



    //跳转目标地址运行  一般用于多APP合在一起  直接跳转
    void JHAL_bootJump2App(uint32_t appAddr);
    //通过can升级程序  isUpdateApp true升级App false升级boot
    void  JHAL_bootUpdating4Can(JHAL_BootInfo bootInfo,bool isUpdateApp);

#ifdef CplusPlus
}
#endif

#endif








