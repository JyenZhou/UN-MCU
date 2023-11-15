

#include "JHAL_BootLoader.h"




#ifndef __UART_Number



#define JBoot_DisUart

#endif



#ifndef __JHAL_CAN_Exist

#define JBoot_DisCAN

#endif

#ifndef JBoot_DisCAN
JHAL_CAN  __boot_canConfig= {JHAL_DEV_INIT};

#endif


#ifndef JBoot_DisUart
JHAL_UART   __boot_uartConfig= {JHAL_DEV_INIT};
#endif

#if   ( defined FlashStartAddr  && (!defined JBoot_DisCAN ||!defined JBoot_DisUart))






//TODO 后面增加boot升级后保存程序ROM CRC做自检  另外做固定偏移地址的错误函数 实现错误回调
/*中断偏移*/
void __JHAL_interruptOffset(uint32_t appxaddr)
{
    JHAL_disableInterrupts();
    //ARM Cortex-M0和Cortex-M0+处理器架构
#if defined __ARM_ARCH_6M__
#if defined USE_HAL_DRIVER
    //st的SRAM起始地址是0x20000000   这里向量表FF理论上都够了 还要改app IRAM1起始+0xFF 大小-0xFF 防止被覆盖
    memcpy((void*)0x20000000, (void*)appxaddr, 0xFF);
#else
#error 未设置中断偏移
#endif
    //__ARM_ARCH_7M__ Cortex-M3内核，__ARM_ARCH_7EM__表示ARM Cortex-M4和Cortex-M7处理器架构
#elif (defined __ARM_ARCH_7M__) ||(defined __ARM_ARCH_7EM__)
    //使用SCB->VTOR 就可以使用flash+偏移地址  和上面的不一样

    SCB->VTOR =appxaddr  ;

#else

#error 未设置中断偏移
#endif

}





void __JHAL_booJjump2App(uint32_t appxaddr)
{

    __JHAL_interruptOffset(appxaddr);
    FuncPtr	app=(FuncPtr)*(__IO uint32_t*)(appxaddr+4);
    __set_MSP(*(__IO uint32_t*) appxaddr);
    app();

}



bool JHAL_bootGetInfo( JHAL_BootInfo *bootFlashInfo)
{
    return JHAL_flashRead(JHAL_BOOT_FlashInfoPage,bootFlashInfo, sizeof(JHAL_BootInfo));

}

bool JHAL_bootSetInfo(JHAL_BootInfo bootFlashInfo)

{
    return  JHAL_flashWirte(JHAL_BOOT_FlashInfoPage,&bootFlashInfo, sizeof(bootFlashInfo));
}



//程序运行中调设置状态为BootStatus_Update 会软重启到Boot中通过状态等待升级
void __JHAL_bootStatusSet(JHAL_BootStatus status)
{
    JHAL_BootInfo bootFlashInfo;
    JHAL_bootGetInfo(&bootFlashInfo);
    bootFlashInfo.bootStatus=status;
    JHAL_bootSetInfo(bootFlashInfo);
    NVIC_SystemReset();
}

JHAL_BootStatus JHAL_bootStatusGet()
{
    JHAL_BootInfo bootFlashInfo;
    JHAL_bootGetInfo(&bootFlashInfo);
    return bootFlashInfo.bootStatus;
}


typedef enum
{
    //升级失败后调用可重新开始升级
    JHAL_BootCMD_Update=0,
    //设置大小并擦除
    JHAL_BootCMD_SetSizeAndErasure=1,
} JHAL_BootCMD;

typedef enum
{
    //APP升级成功
    BootResponse_AppUpdateFinshAndRun=0,
    //等待通过CAN升级
    BootResponse_AppUpdateWiat,
    //App升级超时退出
    BootResponse_AppUpdateTimeOutAndeExit,
    //设置APP大小失败-大小超出
    BootResponse_SetSizeFailure4SizeOut,
    //设置APP大小并擦除成功
    BootResponse_SetSizeAndErasurecSucceed,
    //设置APP大小失败-crc校验失败
    BootResponse_SetSizeFailure4CrcError,
    //设置APP大小失败-擦除失败
    BootResponse_SetSizeFailure4ErasurecError,
    //APP升级失败-文件大小超出预计
    BootResponse_ReceivedSizeOut,
    //APP升级失败-文件校验失败
    BootResponse_AppUpdateFailure4CrcError,
} JHAL_BootUpdateResponse;


void __JHAL_bootAppUpdateResponse(JHAL_BootInfo bootInfo,JHAL_BootUpdateResponse status,u32 info)
{

    u8 data[8]= {BOOT_DataHead};

    data[1]=(u8)status;
    if(BootResponse_SetSizeFailure4SizeOut==status)
    {
        data[2]=(u8)(info>>24);
        data[3]=(u8)(info>>16);
        data[4]=(u8)(info>>8);
        data[5]=(u8)(info);
    } else {

        data[2]=(u8)(JHAL_uidGetLow()>>24);
        data[3]=(u8)(JHAL_uidGetLow()>>16);
        data[4]=(u8)(JHAL_uidGetLow()>>8);
        data[5]=(u8)(JHAL_uidGetLow());

    }

    JHAL_crcAutoWirte(JHAL_CRC_Mode_16_Modbus,data,8);
#ifndef JBoot_DisCAN
    if(bootInfo. bootStatus==BootStatus_Update4CAN) {

        JHAL_CANBox box= {.data=data,.id=bootInfo.bootCanId,.length=8,.isExtendFrame= bootInfo.canIsExtendFrame,.isRemoteFrame=false};

        JHAL_canSendDatas(&__boot_canConfig,box,NULL);



    }
#endif


#if  ((!defined JBoot_DisCAN)  && (!defined JBoot_DisUart))
    else
#endif


        if(bootInfo. bootStatus==BootStatus_Update4Uart) {
#ifndef JBoot_DisUart
            JHAL_uartSendDatas(&__boot_uartConfig,data,8,  JHAL_CRC_Mode_16_Modbus);
#endif

        }


}

//  isUpdateApp true升级App false升级boot
void   __JHAL_bootUpdating(JHAL_BootInfo bootInfo,bool isUpdateApp)
{

    u32 appUpdateTimeout=0;
    //APP的总大小
    u32 appHexSize=0;
    u32 appHexCrc=0;
    //因为can接收数据有限 按照步骤来升级
    u8	bootStep=0;

//收到的Hex大小
    u8  dataBuff[8];
    u8 rxDataLenth= 0;
    u32 appHexReceivedSize=0;

#ifndef JBoot_DisCAN

    if(bootInfo. bootStatus==BootStatus_Update4CAN) {


        __boot_canConfig.baudRate=bootInfo.canBaudRate;
        __boot_canConfig.filterRxIDList=&(bootInfo.pcCanId);
        __boot_canConfig.filterRxIDNumber=1;
        __boot_canConfig.rxDataBuff.data= dataBuff;
        if(isUpdateApp) {
            JHAL_canClose( &__boot_canConfig);
        }
        JHAL_canOpen(  &__boot_canConfig);




    }
#endif
#if  ((!defined JBoot_DisCAN)  && (!defined JBoot_DisUart))
    else
#endif

#ifndef JBoot_DisUart

        if(bootInfo. bootStatus==BootStatus_Update4Uart) {

            __boot_uartConfig.dev=bootInfo.uartDev;
            __boot_uartConfig.baudRate=bootInfo.uartBaudRate;
            JHAL_UART_RXConfig rxConfig;
            //定义8个长度是为了方便兼容can还节省空间

            rxConfig.receiveMode=JHAL_UART_ReceiveITMode_IT_DMA_IDLE;
            rxConfig.dataBuff=dataBuff;
            __boot_uartConfig.rxConfig= rxConfig;

            if(isUpdateApp) {
                JHAL_uartClose( &__boot_uartConfig);
            }
            JHAL_uartOpen  ( &__boot_uartConfig);



        }
#endif

    __JHAL_bootAppUpdateResponse( bootInfo,BootResponse_AppUpdateWiat,0);
    while(true)
    {
#ifndef JBoot_DisCAN
        rxDataLenth=JHAL_canRxFinsh(&__boot_canConfig   );



#endif


#ifndef JBoot_DisUart
        if(rxDataLenth==0) {
            rxDataLenth=JHAL_uartRxFinsh(&__boot_uartConfig);
        }
#endif
        if(rxDataLenth==0)
        {


            if(++appUpdateTimeout>=0x2FFFFF)
            {
                appUpdateTimeout=0;
                bootStep=0;
                __JHAL_bootAppUpdateResponse( bootInfo,BootResponse_AppUpdateWiat,0);
            }
            continue;
        }


        appUpdateTimeout=0;
        switch (bootStep)
        {
        case 0:
            if(JHAL_crcCheak( JHAL_CRC_Mode_16_Modbus,dataBuff,8))
            {

                appHexCrc=(dataBuff[0]<<8)|dataBuff[1];
                appHexSize=  (dataBuff[2]<<24) | (dataBuff[3]<<16) | (dataBuff[4]<<8) | (dataBuff[5]);
                //FlashPageSize 最后一页是要存boot信息的
                if(isUpdateApp?appHexSize> FlashEndAddr-JHAL_BOOT_BootStartAddr-FlashPageSize:appHexSize>JHAL_BOOT_BootMaxSize)
                {
                    __JHAL_bootAppUpdateResponse( bootInfo,BootResponse_SetSizeFailure4SizeOut,isUpdateApp?JHAL_flashGetEndAddr()-FlashPageSize-JHAL_BOOT_BootStartAddr:JHAL_BOOT_BootMaxSize);
                    break;
                }
                if(isUpdateApp?JHAL_flashErasePage(JHAL_BOOT_AppStartAddr,JHAL_BOOT_AppStartAddr+appHexSize):JHAL_flashErasePage(JHAL_BOOT_BootStartAddr,JHAL_BOOT_BootStartAddr+appHexSize))
                {
                    __JHAL_bootAppUpdateResponse( bootInfo,BootResponse_SetSizeAndErasurecSucceed,0);
                    bootStep++;
                    appHexReceivedSize=0;
                } else {
                    __JHAL_bootAppUpdateResponse( bootInfo,BootResponse_SetSizeFailure4ErasurecError,0);
                }
            } else {
                __JHAL_bootAppUpdateResponse( bootInfo,BootResponse_SetSizeFailure4CrcError,0);
            }
            break;
        case 1:

            JHAL_flashWriteNByte(isUpdateApp?JHAL_BOOT_AppStartAddr+appHexReceivedSize:JHAL_BOOT_BootStartAddr+appHexReceivedSize,dataBuff,rxDataLenth);


            appHexReceivedSize+=rxDataLenth ;

            if(appHexReceivedSize>appHexSize)
            {
                bootStep=0;
                __JHAL_bootAppUpdateResponse( bootInfo,BootResponse_ReceivedSizeOut,0);
            } else if(appHexReceivedSize==appHexSize)
            {
                bootStep=0;
                if(JHAL_crc(JHAL_CRC_Mode_16_Modbus,isUpdateApp?(u8 *)JHAL_BOOT_AppStartAddr:(u8 *)JHAL_BOOT_BootStartAddr, appHexSize)==appHexCrc)
                {
                    __JHAL_bootAppUpdateResponse( bootInfo,BootResponse_AppUpdateFinshAndRun,0);
                    __JHAL_bootStatusSet(BootStatus_Run);
                } else {
                    __JHAL_bootAppUpdateResponse( bootInfo,BootResponse_AppUpdateFailure4CrcError,0);
                }
            }
            break;
        }


    }
}








// 定义宏来提取年后两位、月、日、时、分
#define GET_YEAR_2Bit(dateStr)  (dateStr[9] - '0') * 10 + (dateStr[10] - '0')
#define GET_MONTH(dateStr) \
    (dateStr[0] == 'J' && dateStr[1] == 'a' ? 1 : \
     dateStr[0] == 'F' && dateStr[1] == 'e' ? 2 : \
     dateStr[0] == 'M' && dateStr[1] == 'a' && dateStr[2] == 'r' ? 3 : \
     dateStr[0] == 'A' && dateStr[1] == 'p' ? 4 : \
     dateStr[0] == 'M' && dateStr[1] == 'a' && dateStr[2] == 'y' ? 5 : \
     dateStr[0] == 'J' && dateStr[1] == 'u' && dateStr[2] == 'n' ? 6 : \
     dateStr[0] == 'J' && dateStr[1] == 'u' && dateStr[2] == 'l' ? 7 : \
     dateStr[0] == 'A' && dateStr[1] == 'u' ? 8 : \
     dateStr[0] == 'S' && dateStr[1] == 'e' ? 9 : \
     dateStr[0] == 'O' && dateStr[1] == 'c' ? 10 : \
     dateStr[0] == 'N' && dateStr[1] == 'o' ? 11 : 12)

#define GET_DAY(dateStr)   (dateStr[4] - '0') * 10 + (dateStr[5] - '0')
#define GET_HOUR(timeStr)  (timeStr[0] - '0') * 10 + (timeStr[1] - '0')
#define GET_MINUTE(timeStr) (timeStr[3] - '0') * 10 + (timeStr[4] - '0')






/** ----------------------------__JHAL_versionsEncode-----------------------------------
  * @描述：版本信息转码成8byte数组
  *
  * @参数：
**	  	 data: [输 出]  起始地址 长度为7
**			 versions: [输入 ]
  *
  * @返回值:无
  * @注:无
  *-----------------------------Jyen-2023-09-14-------------------------------------- */

void __JHAL_versionsEncode( uint8_t *data, __JHAL_VersionsInfo *versions )
{




    data[0]= BOOT_DataHead;



// 使用memcpy复制source数组到destination数组效率更高但是要上位机做大端兼容
//    memcpy(data+1, versions, sizeof(__JHAL_VersionsInfo) ); // 需要复制的字节数要包括字符串结束符 '\0'

    /*   小端在前*/

    data[1] = versions->productID & 0xFF;
    data[2] = ((versions->productID &0xFE) >>8) | ((versions->isApp & 0x01) << 7);
    data[3] = ( versions->year & 0x7F)    | ((versions->month & 0x01) <<7);
    data[4] = ((versions->month & 0x0F)>> 1) | ((versions->day & 0x1F)<<3);
    data[5] = versions->hour & 0x3F;

    JHAL_crcAutoWirte(JHAL_CRC_Mode_16_Modbus,data,8);

}



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

void __JHAL_versionsCreatInfo(__JHAL_VersionsInfo  *versions, u16 productID,bool isApp  )
{
    const char* dateStr = __DATE__;
    const char* timeStr = __TIME__;

    versions->year = GET_YEAR_2Bit(dateStr);
    versions->month = GET_MONTH(dateStr);
    versions->day =  GET_DAY(dateStr);
    versions->hour = GET_HOUR(timeStr);

    versions->productID =productID;
    versions->isApp=isApp;

}
/** ----------------------------__JHAL_versionsDecode-----------------------------------
  * @描述：8byte数组解码成版本信息
  *
  * @参数：
**	  	 versions: [输 出]
**			 data: [输入 ]
  *
  * @返回值:是否解码成功
  * @注:无
  *-----------------------------Jyen-2023-09-14-------------------------------------- */

bool __JHAL_versionsDecode(__JHAL_VersionsInfo  *versions, uint8_t *data )
{

    if(data[0]!= BOOT_DataHead||!JHAL_crcCheak(JHAL_CRC_Mode_16_Modbus,data,8)) {
        return false ;
    }

    //  memcpy(  versions,data+1, sizeof(__JHAL_VersionsInfo) ); // 需要复制的字节数要包括字符串结束符 '\0'


    versions->productID = data[1] | ((data[2] & 0x7F) << 8);
    versions->isApp = (data[2] >> 7) & 0x01;
    versions->year = data[3] & 0x7F;
    versions->month = ((data[3] >> 7) & 0x01) | ((data[4] & 0x07) << 1);
    versions->day = (data[4] >> 3) & 0x1F;
    versions->hour = data[5] & 0x3F;


    if(versions->year >99||
            versions->month >12||
            versions->day>31 ||
            versions->hour>23) {
        return false;
    }

    return true;
}

//版本对比是否需要更新 true 需要更新 false 已是最新版本
bool  __JHAL_versionsShouldUpdated( __JHAL_VersionsInfo  *remoteVersions,__JHAL_VersionsInfo *localversions )
{
    if(remoteVersions->year> localversions->year)
    {
        return true;
    } else if(remoteVersions->month> localversions->month)
    {
        return true;
    } else if(remoteVersions ->day> localversions ->day)
    {
        return true;
    } else if(remoteVersions ->hour> localversions ->hour)
    {
        return true;
    } else {
        return false;
    }

}




void JHAL_bootLoader(JHAL_BootInfo bootInfo)
{
//如果flash中没有boot配置信息那么就把默认的信息写进去
    if(!  JHAL_bootGetInfo(&bootInfo))
    {

        __JHAL_VersionsInfo localversions;


        __JHAL_versionsCreatInfo(&localversions,  bootInfo.productID,  true  );

        bootInfo.__info=localversions;
        JHAL_bootSetInfo(bootInfo);


    }



    if (bootInfo.bootStatus==BootStatus_Run)
    {
        __JHAL_booJjump2App(JHAL_BOOT_AppStartAddr);
    }

    else
    {
        __JHAL_bootUpdating(bootInfo,true);
    }



}










/** ----------------------------JHAL_bootParseUpdateInfo4App-----------------------------------
  * @描述：由APP调用，传入远程来的数据判断是否要升级，升级的是APP还是Boot
  *
  * @参数：
**	  	 data: [输入 ] 远程来的信息 8个字节 要和编码一致
**			 productID: [输入 ] 产品ID
**			 bootStatus: [输入 ] 将要哪种方式升级 can uart
  *
  * @返回值:无
  * @注:无
  *-----------------------------Jyen-2023-09-14-------------------------------------- */

void JHAL_bootParseUpdateInfo4App( u8 *data,u8 productID,JHAL_BootStatus bootStatus)
{

    __JHAL_VersionsInfo remoteVersions;
    __JHAL_VersionsInfo localversions;

    if(!   __JHAL_versionsDecode(&remoteVersions,data ))
    {
        return;
    }

    if(remoteVersions.productID!=productID)
    {
        return;
    }

    if(remoteVersions.isApp)
    {
        __JHAL_versionsCreatInfo(&localversions,  productID,  true  );
        if(__JHAL_versionsShouldUpdated(&remoteVersions,&localversions))
        {
            __JHAL_bootStatusSet(bootStatus);
        }

    } else {
        JHAL_BootInfo bootInfo ;

//如果flash中没有boot配置信息那么就直接退出
        if(!  JHAL_bootGetInfo(&bootInfo))
        {
            return;
        }

        if(__JHAL_versionsShouldUpdated(&remoteVersions,&bootInfo.__info))
        {
            bootInfo.bootStatus=bootStatus;
            __JHAL_bootUpdating (bootInfo,false);
        }


    }



}


#endif

