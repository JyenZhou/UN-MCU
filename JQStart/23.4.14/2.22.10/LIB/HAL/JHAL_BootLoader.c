

#include "JHAL_BootLoader.h"


//TODO ��������boot�����󱣴����ROM CRC���Լ�  �������̶�ƫ�Ƶ�ַ�Ĵ����� ʵ�ִ���ص�
/*�ж�ƫ��*/
void __JHAL_interruptOffset(uint32_t appxaddr)
{

#ifdef XL6600A402L6
    //��ȫ���ж�
    __asm("CPSID I");
    NVIC_SetVectorTable(NVIC_VectTab_FLASH,appxaddr);
#else
#error δ�����ж�ƫ��
#endif

}

void __JHAL_booJjump2App(uint32_t appxaddr)
{
    if ((((*(__IO uint32_t*)appxaddr) & 0x20000000 ) == 0x20000000) || (((*(__IO uint32_t*)appxaddr) & 0x1FFFE000 ) == 0x1FFFE000))
    {
        __JHAL_interruptOffset(appxaddr);
        FuncPtr	app=(FuncPtr)*(__IO uint32_t*)(appxaddr+4);
        __set_MSP(*(__IO uint32_t*) appxaddr);
        app();
    }
}





bool JHAL_bootGetInfo( JHAL_BootInfo *bootFlashInfo)
{
    return JHAL_flashRead(JHAL_BOOT_FlashInfoPage,bootFlashInfo, sizeof(JHAL_BootInfo));

}

bool JHAL_bootSetInfo(JHAL_BootInfo bootFlashInfo)

{
    return  JHAL_flashWirte(JHAL_BOOT_FlashInfoPage,&bootFlashInfo, sizeof(bootFlashInfo));
}



//���������е�����״̬ΪBootStatus_Update ����������Boot��ͨ��״̬�ȴ�����
void JHAL_bootStatusSet(JHAL_BootStatus status)
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
    //����ʧ�ܺ���ÿ����¿�ʼ����
    JHAL_BootCMD_Update=0,
    //���ô�С������
    JHAL_BootCMD_SetSizeAndErasure=1,
} JHAL_BootCMD;

typedef enum
{
    //APP�����ɹ�
    BootResponse_AppUpdateFinshAndRun=0,
    //�ȴ�ͨ��CAN����
    BootResponse_AppUpdateWiat4CAN,
    //App������ʱ�˳�
    BootResponse_AppUpdateTimeOutAndeExit,
    //����APP��Сʧ��-��С����
    BootResponse_SetSizeFailure4SizeOut,
    //����APP��С�������ɹ�
    BootResponse_SetSizeAndErasurecSucceed,
    //����APP��Сʧ��-crcУ��ʧ��
    BootResponse_SetSizeFailure4CrcError,
    //����APP��Сʧ��-����ʧ��
    BootResponse_SetSizeFailure4ErasurecError,
    //APP����ʧ��-�ļ���С����Ԥ��
    BootResponse_ReceivedSizeOut,
    //APP����ʧ��-�ļ�У��ʧ��
    BootResponse_AppUpdateFailure4CrcError,
} JHAL_BootUpdateResponse;


void __JHAL_bootAppUpdateResponse(JHAL_BootInfo bootInfo,JHAL_BootUpdateResponse status,u32 info)
{

    u8 data[8]= {0xa5};
    JHAL_CANBox box= {.data=data,.id=bootInfo.bootCanId,.length=8,.isExtendFrame= bootInfo.canIsExtendFrame,.isRemoteFrame=false};
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
    JHAL_canSendDatas(bootInfo.can,box,NULL);




}

//  isUpdateApp true����App false����boot
void  JHAL_bootUpdating4Can(JHAL_BootInfo bootInfo,bool isUpdateApp)
{
    JHAL_CANBox canBox= {0};
    u32 appUpdateTimeout=0;
    //APP���ܴ�С
    u32 appHexSize=0;
    u32 appHexCrc=0;
    //��Ϊcan������������ ���ղ���������
    u8	bootStep=0;
//�յ���Hex��С
    u32 appHexReceivedSize=0;
    JHAL_CANConfig canConfig;
    canConfig.canBaudRate=bootInfo.canBaudRate;
    canConfig.filterRxIDList=&(bootInfo.pcCanId);
    canConfig.filterRxIDNumber=1;
    JHAL_canInit(bootInfo.can,&canConfig);
    __JHAL_bootAppUpdateResponse( bootInfo,BootResponse_AppUpdateWiat4CAN,0);
    while(true)
    {
        if(JHAL_canGetRxBuff(bootInfo.can,&canBox))
        {
            appUpdateTimeout=0;
            switch (bootStep)
            {
            case 0:
                if(JHAL_crcCheak( JHAL_CRC_Mode_16_Modbus,canBox.data,8))
                {
                    appHexCrc=(canBox.data[0]<<8)|canBox.data[1];
                    appHexSize=  (canBox.data[2]<<24) | (canBox.data[3]<<16) | (canBox.data[4]<<8) | (canBox.data[5]);
                    //JHAL_flashGetPageSize() ���һҳ��Ҫ��boot��Ϣ��
                    if(isUpdateApp?appHexSize> JHAL_flashGetEndAddr()-JHAL_BOOT_BootStartAddr-JHAL_flashGetPageSize():appHexSize>JHAL_BOOT_BootMaxSize)
                    {
                        __JHAL_bootAppUpdateResponse( bootInfo,BootResponse_SetSizeFailure4SizeOut,isUpdateApp?JHAL_flashGetEndAddr()-JHAL_flashGetPageSize()-JHAL_BOOT_BootStartAddr:JHAL_BOOT_BootMaxSize);
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

                JHAL_flashWriteNByte(isUpdateApp?JHAL_BOOT_AppStartAddr+appHexReceivedSize:JHAL_BOOT_BootStartAddr+appHexReceivedSize,canBox.data,canBox.length);


                appHexReceivedSize+=canBox.length;

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
                        JHAL_bootStatusSet(BootStatus_Run);
                    } else {
                        __JHAL_bootAppUpdateResponse( bootInfo,BootResponse_AppUpdateFailure4CrcError,0);
                    }
                }
                break;
            }
        } else {
            if(++appUpdateTimeout>=0x2FFFFF)
            {
                appUpdateTimeout=0;
                bootStep=0;
                __JHAL_bootAppUpdateResponse( bootInfo,BootResponse_AppUpdateWiat4CAN,0);
            }
        }
    }
}


void JHAL_bootLoader(JHAL_BootInfo bootInfo)
{
//���flash��û��boot������Ϣ��ô�Ͱ�Ĭ�ϵ���Ϣд��ȥ
    if(!  JHAL_bootGetInfo(&bootInfo))
    {
        JHAL_bootSetInfo(bootInfo);
    }



    if (bootInfo.bootStatus==BootStatus_Run)
    {
        __JHAL_booJjump2App(JHAL_BOOT_AppStartAddr);
    }

    else if( BootStatus_Update4CAN==bootInfo.bootStatus)
    {
        JHAL_bootUpdating4Can(bootInfo,true);
    }



    else if( BootStatus_Update4Uart==bootInfo.bootStatus)
    {
    } else
    {
        while(1);
    }

}









