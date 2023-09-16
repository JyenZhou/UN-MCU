/**
  ****************************JHAL_BootLoader.h******************************************
  * @brief     
	
	使用说明：
	   APP中
		 1.使用 JHAL_versionsCreatInfo(JHAL_VersionsInfo  *versions, u16 productID,bool isApp  ); 创建版本信息
       使用    JHAL_versionsEncode( uint8_t *data, JHAL_VersionsInfo *versions );  将版本信息加密，这里加密主要是通过debug去得到值数组值给BIN文件命名上位机下发用
void  showVersionsData()
{
    static  bool isok;
    static JHAL_VersionsInfo versions;
    static	JHAL_VersionsInfo  versions2;
    static uint8_t  data[8];

    JHAL_versionsCreatInfo(&versions,   ProductID,  true );
    JHAL_versionsEncode( data, &versions );


    isok =  JHAL_versionsDecode(  &versions2,   data )	;


}
			 
			 
		 2.串口/CAN接收中解析上位机发的上面的值 传给   JHAL_bootParseUpdateInfo4App( u8 *data,u8 productID,JHAL_BootStatus bootStatus) ;自动触发后续升级
		 
		 
		 3. 初始化时使用如下代码完成boot和app信息同步  防止有时候改了波特率boot不同步 ，这样不需要更新整个boot
 
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
		 
		 Boot中
		  1.    JHAL_bootLoader(JHAL_BootInfo info);  配置好boot信息传入即可
		 
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
#include "JUTIL.h"
#ifdef __CplusPlus
extern "C" {
#endif
#include "HAL\JHAL_CAN.h"
#include "HAL\JHAL_Uart.h"

//flash起始地址 如果未定义需要在flash中定义
#define JHAL_BOOT_BootStartAddr   FlashStartAddr

#if defined  XL6600A402L6
// 16K
#define JHAL_BOOT_BootMaxSize   0x4000

#elif defined STM32F103xE


//16K

#define JHAL_BOOT_BootMaxSize   0x4000
#else
#erroor 未定义该单片机的
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


		
		
  typedef struct  __attribute__((packed))
   {
		 
	  u16 productID  : 15;
	  u16 isApp : 1;   //  1位
	  u16 year : 7;   // 年份的后两位，最大值为99，占用7位
    u16 month : 4;  // 月份，最大值为12，占用4位
    u16 day : 5;    // 日，最大值为31，占用5位
    u8 hour : 6;   // 时，最大值为23，占用6位
	 
		 
  
}JHAL_VersionsInfo;

 

    //后面增加注意 不能指针类型 数组要注明长度
    typedef struct
    {
        //设置boot初始状态
        JHAL_BootStatus bootStatus;
        JHAL_CAN  can;
        JHAL_CANBaudRate canBaudRate;
        bool canIsExtendFrame;
        //发送命令的上位机ID
        u32  pcCanId;
        u16  bootCanId;
        JHAL_UART uart;
        u32  uartBaudRate;
			JHAL_VersionsInfo  versionsInfo;
    } JHAL_BootInfo;




    //boot中判断升级及跳转主程序操作
    void JHAL_bootLoader(JHAL_BootInfo info);


 
    //用于app修改同步boot的配置信息 例如波特率变了
    bool JHAL_bootGetInfo( JHAL_BootInfo *bootFlashInfo);
    bool JHAL_bootSetInfo(JHAL_BootInfo info);


    //跳转目标地址运行  一般用于多APP合在一起  单个内部直接跳转
    void JHAL_bootJump2App(uint32_t appAddr);
   
 
/** ----------------------------JHAL_versionsCreatLocalInfo----------------------------------- 
  * @描述：创建 版本信息  productID isApp:ture=app false=boot
  *
  * @参数：
**	  	 versions: [输 出] 生成的 版本信息
**			 productID: [输入/出]  产品编号15位的0-32768
**			 isApp: [输入/出] trueapp false=boot
  *
  * @返回值:无
  * @注:无
  *-----------------------------Jyen-2023-09-14-------------------------------------- */

void JHAL_versionsCreatInfo(JHAL_VersionsInfo  *versions, u16 productID,bool isApp  );


/** ----------------------------JHAL_bootParseUpdateInfo4App----------------------------------- 
  * @描述：由APP调用，传入远程来的数据判断是否要升级，升级的是APP还是Boot
  *
  * @参数：
**	  	 data: [输入 ] 远程来的信息 7个字节 要和编码一致
**			 productID: [输入 ] 产品ID
**			 bootStatus: [输入 ] 哪种方式升级 can uart
  *
  * @返回值:无
  * @注:无
  *-----------------------------Jyen-2023-09-14-------------------------------------- */

void JHAL_bootParseUpdateInfo4App( u8 *data,u8 productID,JHAL_BootStatus bootStatus) ;

/** ----------------------------JHAL_versionsEncode-----------------------------------
  * @描述：版本信息转码成7byte数组
  *
  * @参数：
**	  	 data: [输 出]  起始地址 长度为7
**			 versions: [输入 ]
  *
  * @返回值:无
  * @注:无
  *-----------------------------Jyen-2023-09-14-------------------------------------- */

void JHAL_versionsEncode( uint8_t *data, JHAL_VersionsInfo *versions );
 bool JHAL_versionsDecode(JHAL_VersionsInfo  *versions, uint8_t *data );

#ifdef CplusPlus
}
#endif

#endif








