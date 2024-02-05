#include "../JHAL_CAN.h"



/*MSP相关初始化*/
void JHAL_canMSPInit(CAN_Type* CANx)
{

#if defined(_AC780X_H)

    if(CANx==CAN0)
    {
        GPIO_SetFunc(GPIOB, GPIO_PIN0, GPIO_FUN1);

        GPIO_SetFunc(GPIOB, GPIO_PIN1, GPIO_FUN1);

        NVIC_SetPriority(CAN0_IRQn, 0);
    }
#elif defined (KF32A140INP)

#else

    需要新增相关型号引脚映射
#endif

}


/*
 canBaudRat:波特率
filterRxIDList：只接受指定ID发来的数据的数组 支持多个 最大16个  其他例如一段帧等功能需要项目中自行重写
filterRxIDNumber： 过滤帧的长度  0不过滤 这里最大16
 */
void JHAL_canConfig(CAN_Type* CANx,JHAL_CANBaudRate canBaudRate,u32  *filterRxIDList,u8 filterRxIDNumber)
{
    /* USER CODE BEGIN CAN_Init 0 */

    /* USER CODE END CAN_Init 0 */

    CAN_BitrateConfigType normalBitrate;
    memset(&normalBitrate, 0, sizeof(normalBitrate));
    CAN_ConfigType canConfig;
    memset(&canConfig, 0, sizeof(canConfig));

    /* USER CODE BEGIN CAN_Init 1 */
//注意自动生成器的默认是全部使能导致若不赋值不需要的DISABLE会过滤失败的 另外再给下面赋值  canConfig.filterNum = 16;
    CAN_FilterControlType filterList[]=
    {
        {0, ENABLE, 0x86, 0x00},
        {1, DISABLE, 0x00000500, 0x000000FF},
        {2, DISABLE, 0x00000400, 0x00000000},    ///<只接收0x400 ID
        {3, DISABLE, 0x00000500, 0x000000FF},    ///<接收0x500~0x5FF 的ID
        {4, DISABLE, 0x00000400, 0x00000000},    ///<只接收0x400 ID
        {5, DISABLE, 0x00000500, 0x000000FF},    ///<接收0x500~0x5FF 的ID
        {6, DISABLE, 0x00000400, 0x00000000},    ///<只接收0x400 ID
        {7, DISABLE, 0x00000500, 0x000000FF},    ///<接收0x500~0x5FF 的ID
        {8, DISABLE, 0x00000400, 0x00000000},    ///<只接收0x400 ID
        {9, DISABLE, 0x00000500, 0x000000FF},    ///<接收0x500~0x5FF 的ID
        {10, DISABLE, 0x00000400, 0x00000000},    ///<只接收0x400 ID
        {11, DISABLE, 0x00000500, 0x000000FF},    ///<接收0x500~0x5FF 的ID
        {12, DISABLE, 0x00000400, 0x00000000},    ///<只接收0x400 ID
        {13, DISABLE, 0x00000500, 0x000000FF},    ///<接收0x500~0x5FF 的ID
        {14, DISABLE, 0x00000400, 0x00000000},    ///<只接收0x400 ID
        {15, DISABLE, 0x00000500, 0x000000FF},    ///<接收0x500~0x5FF 的ID
    };
    /* USER CODE END CAN_Init 1 */
    for(u8 i=0; i<filterRxIDNumber; i++)
    {
        filterList[i].enable=ENABLE;
        filterList[i].code=filterRxIDList[i];

    }



    JHAL_canMSPInit(CANx);
//这里是方便用来动态切换比特率的
    if(canBaudRate==JHAL_CANBaudRate500K)
    {
        normalBitrate.PRESC = 11;
        normalBitrate.SEG_1 = 4;
        normalBitrate.SEG_2 = 1;
        normalBitrate.SJW = 2;
    } else if(canBaudRate==JHAL_CANBaudRate250K)
    {
        normalBitrate.PRESC = 23;
        normalBitrate.SEG_1 = 4;
        normalBitrate.SEG_2 = 1;
        normalBitrate.SJW = 2;
    } else {
        while(1);
    }

    canConfig.canMode = CAN_MODE_NORMAL;
    canConfig.errorWarningLimit = 1;
    canConfig.clockSrc = CAN_CLKSRC_AHB;
    canConfig.tsMode = CAN_TSMODE_FIFO;
    canConfig.tsAmount = CAN_TRANSMIT_ALL;
    canConfig.interruptEn = ENABLE;
    canConfig.tpss = DISABLE;
    canConfig.tsss = DISABLE;
    canConfig.timeStampEn = DISABLE;
    canConfig.rom = CAN_RECV_OVER_WRITE;
    canConfig.selfAckEn = DISABLE;
    canConfig.fdModeEn = DISABLE;
    canConfig.normalBitrate = &normalBitrate;
    canConfig.callback = CAN0_IRQHandler_Callback;
    canConfig.filterNum = 16;
    canConfig.filterList = filterList;
    canConfig.interruptMask = CAN_CTRL1_RIE_Msk;
    CAN_Init(CANx, &canConfig);
}


/*默认使用CAN0
 canBaudRat:波特率
filterRxIDList：只接受指定ID发来的数据的数组 支持多个 最大16个  其他例如一段帧等功能需要项目中自行重写
filterRxIDNumber： 过滤帧的长度  0不过滤 这里最大16
*/


__attribute__((weak)) void JHAL_canInit(bool  isEN,JHAL_CANBaudRate canBaudRate,u32  *filterRxIDList,u8 filterRxIDNumber)
{

    static bool sEN= {false};
    if(sEN!=isEN)
    {
        sEN=isEN;

        if(isEN)
        {
            JHAL_canConfig(CAN0,canBaudRate,filterRxIDList,filterRxIDNumber);
            CAN_SetTransmitAmount(CAN0,CAN_TRANSMIT_ALL);
        } else {

            CAN_DeInit(CAN0);
        }
    }



}





int32_t JHAL_canSendData(JHAL_CANBox canBox)
{
    CAN_MsgInfoType t_caninfo;
    memset(&t_caninfo, 0, sizeof(t_caninfo));
    t_caninfo.ID = canBox.id;
    t_caninfo.DLC =canBox.length ;
    t_caninfo.IDE = (u8)canBox.isExtendFrame;
		t_caninfo.RTR=(u8)canBox.isRemoteFrame;
    t_caninfo.DATA = canBox.data;
    return  CAN_TransmitMessage(CAN0, &t_caninfo, CAN_TRANSMIT_SECONDARY);
}

//多帧数据赋值函数并发送数据
int32_t JHAL_canSendDatas(JHAL_CANBox canBox,FuncPtr  sendOneFrameCallback )
{
		
    u16 num=0,length= canBox.length;
    u8 remainder=canBox.length%8;
    u32 retValue=0;
    for(num = 0; num < length/8; num++)
    {
				canBox.length=8;
        if( (retValue=JHAL_canSendData(canBox))!=0)
        {
            return retValue;
        }
		canBox.data+=8;
        sendOneFrameCallback();

    }
    if(remainder != 0)
    {
        uint8 tData[8]= {0};
        for(uint8 i = 0; i<remainder; i++)
        {
            tData[i]=	canBox.data[i];
        }
				canBox.length=remainder;
				canBox.data=tData;
        if( (retValue=JHAL_canSendData(canBox))!=0)
        {
            return retValue;
        }

    }
    return retValue;
}
static u8 dataBuff[8];
static JHAL_CANBox jcanBox= {.data=dataBuff};
bool  jcanRxFinsh=false;
/*在中断中接收一帧后缓存起来直到用户调用后才清除 主要是用于用户定期轮询
 * 返回值true值有效  false值无效
注意：1.用户若长时间没使用始终不会更新新来的值 2.不适用连续帧 及响应高的场景 需要自行从接收中断处理*/
bool JHAL_canFrameGetRxBuff(JHAL_CANBox* canBox)
{
    if(!jcanRxFinsh)
    {
        return false;
    }
    canBox->id=jcanBox.id;
    canBox->data=jcanBox.data;
		jcanRxFinsh=false;
    return true;

}
//更新接收区的值当用户没有使用值时会保持旧值 与上面配合用
void JHAL_canFrameRxBuffUpdate(JHAL_CANBox canBox)
{
    if(!jcanRxFinsh)
    {
        jcanBox.id=canBox.id;
        memcpy(jcanBox.data, canBox.data, canBox.length );
        jcanBox.length=canBox.length;
        jcanRxFinsh=true;
    }

}


